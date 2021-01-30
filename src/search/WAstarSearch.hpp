#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class WAstarSearch : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

public:
    WAstarSearch(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    double run(PriorityQueue<Node*>&              open, PriorityQueue<Node*>&,
               unordered_map<State, Node*, Hash>& closed,
               unordered_map<State, Node*, Hash>&,
               std::function<bool(Node*, unordered_map<State, Node*, Hash>&,
                                  PriorityQueue<Node*>&)>
                                      duplicateDetection,
               SearchResultContainer& res)
    {
        sortOpen(open);

        // Expand until find the goal
        while (!open.empty()) {
            // Pop lowest fhat-value off open
            Node* cur = open.top();

            /*cout << "d " << cur->getDValue() << " dhat " <<
             * cur->getDHatValue()*/
            //<< " p " << cur->getPotentialNancyValue() << "\n";

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getGValue();
            }

            res.nodesExpanded++;

            // cout << cur->getState();

            open.pop();
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

                bool dup = duplicateDetection(childNode, closed, open);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF     = childNode->getFValue();
                    bestChild = child;
                }

                // Duplicate detection
                if (!dup) {
                    open.push(childNode);
                    closed[child] = childNode;
                } else
                    delete childNode;
            }
        }

        return -1.0;
    }

private:
    void sortOpen(PriorityQueue<Node*>& open)
    {
        if (this->sortingFunction == "wastar") {
            open.swapComparator(Node::compareNodesWeightedF);
       /* } else if (this->sortingFunction == "ptshhat") {*/
            //open.swapComparator(Node::compareNodesPTSHHat);
        //} else if (this->sortingFunction == "ptsnancy") {
            //open.swapComparator(Node::compareNodesPTSNancy);
        //} else if (this->sortingFunction == "ptsnancyonlyprob" ||
                   //this->sortingFunction == "ptsnancyonlyprob-olv") {
            //open.swapComparator(Node::compareNodesPTSNancyOnlyProb);
        //} else if (this->sortingFunction == "ptsnancyonlyeffort") {
            //open.swapComparator(Node::compareNodesD);
        //} else if (this->sortingFunction == "ptsnancyonlyeffort-dhat") {
            //open.swapComparator(Node::compareNodesDHat);
        //} else if (this->sortingFunction == "ptsnancywithdhat" ||
                   //this->sortingFunction == "ptsnancywithdhatandbf" ||
                   //this->sortingFunction == "ptsnancywithdhat-olv") {
            /*open.swapComparator(Node::compareNodesPTSNancyWithDhat);*/
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
    }
};