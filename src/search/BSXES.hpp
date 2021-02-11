#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class BSXES : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    enum class Qtype
    {
        undefined,
        focal,
        open,
        openAndFocal
    };

public:
    BSXES(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    double run(SearchResultContainer& res)
    {
        sortOpen();
        sortFocal();

        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        Node* weightedInitNode =
          new Node(Node::weight * initNode->getGValue(),
                   Node::weight * initNode->getHValue(),
                   Node::weight * initNode->getDValue(),
                   this->domain.epsilonHGlobal(), this->domain.epsilonDGlobal(),
                   this->domain.epsilonHVarGlobal(), State(), NULL);

        fmin        = initNode->getFValue();
        fhatmin     = initNode->getFHatValue();
        fhatminNode = initNode;
        pushFhatmin();

        open.insert(initNode);
        bool isIncreament;
        open.updateCursor(weightedInitNode, isIncreament);
        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {

            Qtype nodeFrom = Qtype::undefined;
            Node* cur      = selectNode(nodeFrom);

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getGValue();
            }

            res.nodesExpanded++;

            cur->close();

            vector<State> children = this->domain.successors(cur->getState());
            res.nodesGenerated += children.size();

            State bestChild;
            Node* bestFChildNode;
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

                bool dup = duplicateDetection(childNode);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF          = childNode->getFValue();
                    bestChild      = child;
                    bestFChildNode = childNode;
                }

                if (!dup && childNode->getFHatValue() < bestFHat) {
                    bestFHat          = childNode->getFHatValue();
                    bestFHatChildNode = childNode;
                }

                // Duplicate detection
                if (!dup) {

                    childNode->computeExpectedEffortValue(fhatmin, fhatminVar);

                    open.insert(childNode);
                    if (childNode->getFValue() <= Node::weight * fmin) {
                        focal.push(childNode);
                    }

                    closed[child] = childNode;
                } else
                    delete childNode;
            }

            // Learn one-step error
            if (bestF != numeric_limits<double>::infinity()) {
                Cost epsD =
                  (1 + this->domain.distance(bestChild)) - cur->getDValue();
                Cost epsH = (this->domain.getEdgeCost(bestChild) +
                             this->domain.heuristic(bestChild)) -
                            cur->getHValue();

                this->domain.pushEpsilonHGlobal(epsH);
                this->domain.pushEpsilonDGlobal(epsD);

                this->domain.updateEpsilons();
            }

            // update fmin and fhatmin
            if (cur == fhatminNode || bestFHat < fhatmin) {
                fhatmin     = bestFHat;
                fhatminNode = bestFHatChildNode;
            }

            pushFhatmin();

            if (nodeFrom == Qtype::open) {
                fmin = bestF;

                Node* weightedFMinNode = new Node(
                  Node::weight * bestFChildNode->getGValue(),
                  Node::weight * bestFChildNode->getHValue(),
                  Node::weight * bestFChildNode->getDValue(),
                  this->domain.epsilonHGlobal(), this->domain.epsilonDGlobal(),
                  this->domain.epsilonHVarGlobal(), State(), NULL);

                bool isIncrease;
                auto itemsNeedUpdate =
                  open.updateCursor(weightedFMinNode, isIncrease);

                for (auto item : itemsNeedUpdate) {
                    if (isIncrease)
                        focal.push(item);
                    else {
                        focal.remove(item);
                    }
                }
            }
        }

        return -1.0;
    }

private:
    void sortOpen()
    {
        if (this->sortingFunction == "bsxes") {
            open.swapComparator(Node::compareNodesExpectedEffort);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    void sortFocal()
    {
        if (this->sortingFunction == "bsxes") {
            focal.swapComparator(Node::compareNodesF);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    Node* selectNode(Qtype& nodeFrom)
    {
        Node* cur;

        if (!focal.empty()) {

            // cout << "pop from focal\n";
            cur = focal.top();

            /*if (open.getSize() == 118) {*/
            // cout << "cur " << cur << "\n";
            // cout << "before delete "
            //<< "\n";
            // open.prettyPrint();
            /*}*/

            nodeFrom = Qtype::focal;

            auto isOpenTop = cur == open.getMinItem();

            if (isOpenTop) {
                nodeFrom = Qtype::open;
            }
            focal.pop();

            /*if (open.getSize() == 2568) {*/
            // cout << "cur " << cur << "\n";
            // open.prettyPrint();
            // cout << "TNULL " << open.getTNULL() << "\n";
            //// exit(1);
            /*}*/
            // cout << "open size " << open.getSize() << "\n";

            open.deleteNode(cur);
            // open.checkTreePropertyRedKidsAreRed();
            return cur;
        }

        cur = open.getMinItem();
        // cout << "pop from open\n";
        focal.remove(cur);
        nodeFrom = Qtype::open;
        open.deleteNode(cur);
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
                    // cout << "dup on open " << it->second << "\n";
                    open.deleteNode(it->second);

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    open.insert(it->second);
                    focal.update(it->second);
                } else {
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
                    if (it->second->getFHatValue() <= Node::weight * fmin) {
                        focal.push(it->second);
                    }
                }
            }
            return true;
        }
        return false;
    }

    void pushFhatmin()
    {
        fhatminSum += fhatmin;
        fhatminSumSq += fhatmin * fhatmin;
        fhatminCounter++;

        if (fhatminCounter < 20) {
            fhatminVar = 0;
            return;
        }

        fhatminVar =
          (fhatminSumSq - (fhatminSum * fhatminSum) / fhatminCounter) /
          (fhatminCounter - 1);
    }

    RBTree<Node*>                     open;
    Cost                              fhatmin;
    Cost                              fmin;
    PriorityQueue<Node*>              focal;
    Node*                             fhatminNode;
    unordered_map<State, Node*, Hash> closed;

    double fhatminVar;
    double fhatminSum;
    double fhatminSumSq;
    double fhatminCounter;
};
