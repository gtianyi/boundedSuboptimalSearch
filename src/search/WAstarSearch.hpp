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

    ~WAstarSearch()
    {

        // Empty OPEN and CLOSED
        open.clear();

        // delete all of the nodes from the last expansion phase
        for (typename unordered_map<State, Node*, Hash>::iterator it =
               closed.begin();
             it != closed.end(); it++)
            delete it->second;

        closed.clear();
    }

    double run(SearchResultContainer& res)
    {
        sortOpen();

        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        open.push(initNode);
        res.initialH = inith;

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

                bool dup = duplicateDetection(childNode);

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
    void sortOpen()
    {
        if (this->sortingFunction == "wastar") {
            open.swapComparator(Node::compareNodesWeightedF);
            /* } else if (this->sortingFunction == "ptshhat") {*/
            // open.swapComparator(Node::compareNodesPTSHHat);
            //} else if (this->sortingFunction == "ptsnancy") {
            // open.swapComparator(Node::compareNodesPTSNancy);
            //} else if (this->sortingFunction == "ptsnancyonlyprob" ||
            // this->sortingFunction == "ptsnancyonlyprob-olv") {
            // open.swapComparator(Node::compareNodesPTSNancyOnlyProb);
            //} else if (this->sortingFunction == "ptsnancyonlyeffort") {
            // open.swapComparator(Node::compareNodesD);
            //} else if (this->sortingFunction == "ptsnancyonlyeffort-dhat") {
            // open.swapComparator(Node::compareNodesDHat);
            //} else if (this->sortingFunction == "ptsnancywithdhat" ||
            // this->sortingFunction == "ptsnancywithdhatandbf" ||
            // this->sortingFunction == "ptsnancywithdhat-olv") {
            /*open.swapComparator(Node::compareNodesPTSNancyWithDhat);*/
        } else {
            cout << "Unknown algorithm!\n";
            exit(1);
        }
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

                // This state has been generated before, check if its node is on
                // OPEN
                if (it->second->onOpen()) {
                    // This node is on OPEN, keep the better g-value
                    open.update(it->second);
                } else {
                    // cout << "reopen\n";
                    it->second->reopen();
                    open.push(it->second);
                }
            }
            return true;
        }

        return false;
    }

    PriorityQueue<Node*>              open;
    unordered_map<State, Node*, Hash> closed;
};
