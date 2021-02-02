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
        cleanup.push(initNode);
        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {

            // debug ees
            cout << "focal q: " << focal.size() << "\n";
            cout << "open q: " << open.getSize() << "\n";
            cout << "cleanup q: " << cleanup.size() << "\n";

            assert(open.getSize() == cleanup.size());

            Node* cur = selectNode();

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
                    bestF      = childNode->getFValue();
                    bestFChildState = child;
                }

                if (!dup && childNode->getFHatValue() < bestFHat) {
                    bestFHat      = childNode->getFHatValue();
                    bestFHatChildNode = childNode;
                }

                if (!dup) {
                    open.insert(childNode);
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
                Cost epsD =
                  (1 + this->domain.distance(bestFChildState)) - cur->getDValue();
                Cost epsH = (this->domain.getEdgeCost(bestFChildState) +
                             this->domain.heuristic(bestFChildState)) -
                            cur->getHValue();

                this->domain.pushEpsilonHGlobal(epsH);
                this->domain.pushEpsilonDGlobal(epsD);

                this->domain.updateEpsilons();
            }

            // update fmin and fhatmin
            if (fmin > bestF) {
                fmin = bestF;
            }

            if (fhatmin > bestFHat) {
                fhatmin = bestFHat;
                bool isIncrease;
                auto itemsNeedUpdate =
                  open.updateCursor(bestFHatChildNode, isIncrease);
                if(!isIncrease && !itemsNeedUpdate.empty()) {
                    cout << "fhatmin decreased!\n";
                    exit(1);
                }
                for(auto item : itemsNeedUpdate){
                    focal.push(item);
                }
            }
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

    Node* selectNode()
    {
        Node* cur;

        if (!focal.empty() &&
            focal.top()->getFHatValue() <= Node::weight * fmin) {
            cur = focal.top();
            focal.pop();
            open.deleteNode(cur);
            cleanup.remove(cur);
            return cur;
        }

        cur = open.getMinItem();
        if (cur->getFHatValue() <= Node::weight * fmin) {
            focal.remove(cur);
            open.deleteNode(cur);
            cleanup.remove(cur);
            return cur;
        }

        cur = cleanup.top();
        focal.remove(cur);
        open.deleteNode(cur);
        cleanup.remove(cur);
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
                it->second->setGValue(node->getGValue());
                it->second->setParent(node->getParent());
                it->second->setHValue(node->getHValue());
                it->second->setDValue(node->getDValue());
                it->second->setEpsilonH(node->getEpsilonH());
                it->second->setEpsilonHVar(node->getEpsilonHVar());
                it->second->setEpsilonD(node->getEpsilonD());
                it->second->setState(node->getState());

                // This state has been generated before,
                // check if its node is on OPEN
                if (it->second->onOpen()) {
                    // This node is on OPEN and cleanup, keep the better g-value
                    open.deleteNode(it->second);
                    open.insert(it->second);
                    cleanup.update(it->second);
                    focal.update(it->second);
                } else {
                    // cout << "reopen\n";
                    it->second->reopen();
                    open.insert(it->second);
                    cleanup.push(it->second);
                    if (it->second->getFHatValue() <= Node::weight * fhatmin) {
                        focal.push(it->second);
                    }
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
