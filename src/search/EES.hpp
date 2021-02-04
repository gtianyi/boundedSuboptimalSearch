#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class EES : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

public:
    EES(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    double run(SearchResultContainer& res)
    {
        sortOpen();
        sortFocal();
        sortCleanup();

        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        fmin    = initNode->getFValue();
        fhatmin = initNode->getFHatValue();

        open.insert(initNode);
        assert(open.getList().size() == open.getSize());
        cleanup.push(initNode);
        bool isIncreament;
        open.updateCursor(initNode, isIncreament);
        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {

            // debug ees
            // cout << "focal q: " << focal.size() << "\n";
            // cout << "open q: " << open.getSize() << "\n";
            // cout << "cleanup q: " << cleanup.size() << "\n";

            assert(open.getSize() == cleanup.size());
            assert(open.getList().size() == open.getSize());

            int   nodeFrom = -1;
            Node* cur      = selectNode(nodeFrom);

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getFValue();
            }

            res.nodesExpanded++;

            cur->close();

            vector<State> children = this->domain.successors(cur->getState());
            res.nodesGenerated += children.size();

            State bestFChildState;
            Node* bestFHatChildNode;
            Cost  bestF    = numeric_limits<double>::infinity();
            Cost  bestFHat = numeric_limits<double>::infinity();

            for (State child : children) {

                auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
                auto newH = this->domain.heuristic(child);
                auto newD = this->domain.distance(child);

                Node* childNode =
                  new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                           this->domain.epsilonDGlobal(),
                           this->domain.epsilonHVarGlobal(), child, cur);

                // Duplicate detection
                bool dup = duplicateDetection(childNode);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF           = childNode->getFValue();
                    bestFChildState = child;
                }

                if (!dup && childNode->getFHatValue() < bestFHat) {
                    bestFHat          = childNode->getFHatValue();
                    bestFHatChildNode = childNode;
                }

                if (!dup) {
                    open.insert(childNode);
                    assert(open.getList().size() == open.getSize());
                    cleanup.push(childNode);
                    if (childNode->getFHatValue() <= Node::weight * fhatmin) {
                        focal.push(childNode);
                    }
                    closed[child] = childNode;
                } else
                    delete childNode;
            }

            // Learn one-step error
            if (bestF != numeric_limits<double>::infinity()) {
                Cost epsD = (1 + this->domain.distance(bestFChildState)) -
                            cur->getDValue();
                Cost epsH = (this->domain.getEdgeCost(bestFChildState) +
                             this->domain.heuristic(bestFChildState)) -
                            cur->getHValue();

                this->domain.pushEpsilonHGlobal(epsH);
                this->domain.pushEpsilonDGlobal(epsD);

                this->domain.updateEpsilons();
            }

            // update fmin and fhatmin
            if (nodeFrom == 2) {
                fmin = bestF;
            }
            if (nodeFrom == 1) {
                fhatmin = bestFHat;

                Node* weightedFhatMinNode = new Node(
                  Node::weight * bestFHatChildNode->getGValue(),
                  Node::weight * bestFHatChildNode->getHValue(),
                  Node::weight * bestFHatChildNode->getDValue(),
                  this->domain.epsilonHGlobal(), this->domain.epsilonDGlobal(),
                  this->domain.epsilonHVarGlobal(), State(), NULL);

                bool isIncrease;
                auto itemsNeedUpdate =
                  open.updateCursor(weightedFhatMinNode, isIncrease);

                for (auto item : itemsNeedUpdate) {
                    if (isIncrease)
                        focal.push(item);
                    else {
                        focal.remove(item);
                    }
                }
            }

            // assert(open.getSize() == cleanup.size());
        }

        return -1.0;
    }

private:
    void sortOpen()
    {
        if (this->sortingFunction == "ees") {
            open.swapComparator(Node::compareNodesFHat);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    void sortFocal()
    {
        if (this->sortingFunction == "ees") {
            focal.swapComparator(Node::compareNodesDHat);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    void sortCleanup()
    {
        if (this->sortingFunction == "ees") {
            cleanup.swapComparator(Node::compareNodesF);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    Node* selectNode(int& nodeFrom)
    {
        Node* cur;

        if (!focal.empty() &&
            focal.top()->getFHatValue() <= Node::weight * fmin) {
            cur = focal.top();
            focal.pop();
            open.deleteNode(cur);
            if (open.getList().size() != open.getSize()) {
                cout << "open size" << open.getSize() << "\n";
                cout << "open element size" << open.getList().size() << "\n";
                cout << "cleanup size" << cleanup.size() << "\n";
                cout << cur << " fh" << cur->getFHatValue() << " g"
                     << cur->getGValue() << "\n";
                open.prettyPrint();
            }
            assert(open.getList().size() == open.getSize());
            cleanup.remove(cur);
            nodeFrom = 0;
            if (open.getSize() != cleanup.size()) {
                cout << "open size" << open.getSize() << "\n";
                cout << "cleanup size" << cleanup.size() << "\n";
                cout << cur << " fh" << cur->getFHatValue() << " g"
                     << cur->getGValue() << "\n";
                open.prettyPrint();
            }
            assert(open.getSize() == cleanup.size());

            return cur;
        }

        cur = open.getMinItem();
        if (cur->getFHatValue() <= Node::weight * fmin) {
            // cout << "pop from open\n";
            focal.remove(cur);
            open.deleteNode(cur);
            assert(open.getList().size() == open.getSize());
            cleanup.remove(cur);
            nodeFrom = 1;
            assert(open.getSize() == cleanup.size());
            return cur;
        }

        // cout << "pop from cleanup\n";
        cur = cleanup.top();
        focal.remove(cur);
        open.deleteNode(cur);
        assert(open.getList().size() == open.getSize());
        cleanup.remove(cur);
        nodeFrom = 2;
        assert(open.getSize() == cleanup.size());
        return cur;
    }

    bool duplicateDetection(Node* node)
    {
        // Check if this state exists
        typename unordered_map<State, Node*, Hash>::iterator it =
          closed.find(node->getState());

        if (it != closed.end()) {
            /*cout << "dup found \n";*/
            // cout << "new " << node->getState();
            // cout << "old " << it->second->getState();

            // if the new node is better, update it on close
            if (node->getGValue() < it->second->getGValue()) {

                // This state has been generated before,
                // check if its node is on OPEN
                if (it->second->onOpen()) {
                    // This node is on OPEN and cleanup, keep the better g-value
                    cout << "dup on open " << it->second << "\n";
                    open.deleteNode(it->second);
                    assert(open.getList().size() == open.getSize());

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    open.insert(it->second);
                    assert(open.getList().size() == open.getSize());
                    cleanup.update(it->second);
                    focal.update(it->second);
                    assert(open.getSize() == cleanup.size());
                } else {
                    cout << "reopen " << it->second << "\n";
                    it->second->reopen();

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    open.insert(it->second);
                    assert(open.getList().size() == open.getSize());
                    cleanup.push(it->second);
                    if (it->second->getFHatValue() <= Node::weight * fhatmin) {
                        focal.push(it->second);
                    }

                    assert(open.getSize() == cleanup.size());
                }
            }
            return true;
        }
        return false;
    }

    RBTree<Node*>                     open;
    PriorityQueue<Node*>              focal;
    PriorityQueue<Node*>              cleanup;
    Cost                              fmin;
    Cost                              fhatmin;
    unordered_map<State, Node*, Hash> closed;
};
