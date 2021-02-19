#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class DXES : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    enum class Qtype
    {
        undefined,
        focal,
        open,
        openfhat,
        openAndOpenFhat
    };

public:
    DXES(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
        , fhatminVar(100)
        , fhatminSum(0)
        , fhatminSumSq(0)
        , fhatminCounter(0)
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
        openfhat.push(initNode);
        bool isIncreament;
        open.updateCursor(weightedInitNode, isIncreament);
        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {
            // cout << "open size " << open.getSize() << "\n";
            // cout << "openfhat size " << openfhat.size() << "\n";

            Qtype nodeFrom = Qtype::undefined;
            Node* cur      = selectNode(nodeFrom);

            cerr << "{\"g\":" << cur->getGValue() << ", ";
            cerr << "\"f\":" << cur->getFValue() << ", ";
            cerr << "\"h\":" << cur->getHValue() << ", ";
            cerr << "\"d\":" << cur->getDValue() << ", ";
            cerr << "\"fhat\":" << cur->getFHatValue() << ", ";
            cerr << "\"fhat var\":"
                 << pow((cur->getFHatValue() - cur->getFValue()) / 2, 2)
                 << ", ";
            cerr << "\"dxesValue\":" << cur->getDXESValue() << ", ";
            cerr << "\"dxesProbValue\":" << cur->getDXESProbValue() << ", ";
            cerr << "\"expansion\":" << res.nodesExpanded << ", ";
            cerr << "\"fmin\":" << fmin << ", ";
            cerr << "\"fhatmin var\":" << fhatminVar << ", ";
            cerr << "\"open size\":" << open.getSize() << ", ";
            cerr << "\"focal size\":" << focal.size() << ", ";
            cerr << "\"fhatmin\":" << fhatmin << "}\n";

            cout << "{\"g\":" << cur->getGValue() << ", ";
            cout << "\"f\":" << cur->getFValue() << ", ";
            cout << "\"h\":" << cur->getHValue() << ", ";
            cout << "\"d\":" << cur->getDValue() << ", ";
            cout << "\"fhat\":" << cur->getFHatValue() << ", ";
            cout << "\"fhat var\":"
                 << pow((cur->getFHatValue() - cur->getFValue()) / 2, 2)
                 << ", ";
            cout << "\"dxesValue\":" << cur->getDXESValue() << ", ";
            cout << "\"dxesProbValue\":" << cur->getDXESProbValue() << ", ";
            cout << "\"expansion\":" << res.nodesExpanded << ", ";
            cout << "\"fmin\":" << fmin << ", ";
            cout << "\"fhatmin var\":" << fhatminVar << ", ";
            cout << "\"open size\":" << open.getSize() << ", ";
            cout << "\"focal size\":" << focal.size() << ", ";
            cout << "\"fhatmin\":" << fhatmin << "}\n";

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
            Cost  bestF = numeric_limits<double>::infinity();

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
                    bestF     = childNode->getFValue();
                    bestChild = child;
                }

                // Duplicate detection
                if (!dup) {

                    childNode->computeExpectedEffortValue(fhatminNode,
                                                          fhatminVar);

                    open.insert(childNode);
                    openfhat.push(childNode);
                    // if (childNode->getFValue() <= Node::weight * fmin) {

                    // if (res.nodesExpanded > 100 &&
                    if (childNode->getDXESProbValue() >= 0.8 &&
                        childNode->getFValue() <= Node::weight * fmin) {
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

            // update fhatmin
            // if (nodeFrom == Qtype::openfhat ||
            // nodeFrom == Qtype::openAndOpenFhat) {
            // cout << "update fhatmin fffffffffffffffffffffhatmin" << endl;
            fhatminNode = openfhat.top();
            fhatmin     = fhatminNode->getFHatValue();
            //}

            pushFhatmin();

            // update fmin
            auto fminNode = open.getMinItem();
            // if (nodeFrom == Qtype::open || nodeFrom ==
            // Qtype::openAndOpenFhat) {
            if (fmin != fminNode->getFValue()) {

                fmin = fminNode->getFValue();

                Node* weightedFMinNode = new Node(
                  Node::weight * fminNode->getGValue(),
                  Node::weight * fminNode->getHValue(),
                  Node::weight * fminNode->getDValue(),
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
            // assert(open.getSize() == openfhat.size());
            // cout << "--------------------expansion end-------" << endl;
        }

        return -1.0;
    }

private:
    void sortOpen()
    {
        if (this->sortingFunction == "dxes") {
            open.swapComparator(Node::compareNodesF);
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }

    void sortFocal()
    {
        if (this->sortingFunction == "dxes") {
            focal.swapComparator(Node::compareNodesExpectedEffort);
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

            auto isOpenTop     = cur == open.getMinItem();
            auto isOpenFhatTop = cur == openfhat.top();

            if (isOpenTop && isOpenFhatTop) {
                nodeFrom = Qtype::openAndOpenFhat;
            } else if (isOpenTop) {
                nodeFrom = Qtype::open;
            } else if (isOpenFhatTop) {
                nodeFrom = Qtype::openfhat;
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
            openfhat.remove(cur);
            // open.checkTreePropertyRedKidsAreRed();
            return cur;
        }

        cur = open.getMinItem();

        nodeFrom = Qtype::open;
        if (cur == openfhat.top()) {
            nodeFrom = Qtype::openAndOpenFhat;
        }

        // cout << "pop from open\n";
        focal.remove(cur);
        open.deleteNode(cur);
        openfhat.remove(cur);
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
                    openfhat.update(it->second);
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
                    openfhat.push(it->second);
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

        if (fhatminCounter < 2) {
            fhatminVar = 100;
            return;
        }

        /*if (fhatminCounter < 100) {*/
        // fhatminVar = 100;
        // return;
        /*}*/

        fhatminVar =
          (fhatminSumSq - (fhatminSum * fhatminSum) / fhatminCounter) /
          (fhatminCounter - 1.0);
    }

    RBTree<Node*>                     open;
    Cost                              fhatmin;
    Cost                              fmin;
    PriorityQueue<Node*>              focal;
    PriorityQueue<Node*>              openfhat;
    Node*                             fhatminNode;
    unordered_map<State, Node*, Hash> closed;

    double fhatminVar;
    double fhatminSum;
    double fhatminSumSq;
    double fhatminCounter;
};
