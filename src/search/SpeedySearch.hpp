#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class SpeedySearch : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

public:
    SpeedySearch(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    ~SpeedySearch()
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
        open.swapComparator(Node::compareNodesDHat);

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

            // debug speedy
            cerr << "{";
            //cerr << "\"g\":" << cur->getGValue() << ", ";
            cerr << "\"f\":" << cur->getFValue() << ", ";
            //cerr << "\"h\":" << cur->getHValue() << ", ";
            //cerr << "\"d\":" << cur->getDValue() << ", ";
            //cerr << "\"fhat\":" << cur->getFHatValue() << ", ";
            cerr << "\"dhat\":" << cur->getDHatValue() << ", ";
            cerr << "\"expansion\":" << res.nodesExpanded << ", ";
            //cerr << "\"open size\":" << open.size() << ", ";
            //cerr << "}\n";

            cerr << "\"state\":" << cur->getState().key() << "\n";


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

            for (State child : children) {

                auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
                auto newH = this->domain.heuristic(child);
                auto newD = this->domain.distance(child);

                Node* childNode =
                  new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                           this->domain.epsilonDGlobal(),
                           this->domain.epsilonHVarGlobal(), child, cur);

                bool dup = duplicateDetection(childNode);
            
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
