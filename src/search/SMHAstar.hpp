#pragma once
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class SMHAstar : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    enum class Qtype
    {
        UNDEFINED,
        D,
        FHAT,
        ANCHOR
    };

public:
    SMHAstar(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    ~SMHAstar()
    {
        // delete all of the nodes from the last expansion phase
        for (typename unordered_map<State, Node*, Hash>::iterator it =
               closedAnchor.begin();
             it != closedAnchor.end(); it++)
            delete it->second;

        closedAnchor.clear();
        closedInadmissible.clear();
    }

    double run(SearchResultContainer& res)
    {
        fhatQueue.swapComparator(Node::compareNodesFHat);
        dQueue.swapComparator(Node::compareNodesD);
        anchorQueue.swapComparator(Node::compareNodesF);

        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        dQueue.push(initNode);
        fhatQueue.push(initNode);
        anchorQueue.push(initNode);

        res.initialH = inith;

        // Expand until find the goal
        while (!anchorQueue.empty()) {
            auto dquueueResult = processQueue(dQueue, res, Qtype::D);
            if (dquueueResult > 0) {
                return dquueueResult;
            }

            auto fhatQueueResult = processQueue(fhatQueue, res, Qtype::FHAT);
            if (fhatQueueResult > 0) {
                return fhatQueueResult;
            }
        }

        return -1.0;
    }

private:
    double processQueue(PriorityQueue<Node*>&  pqueue,
                        SearchResultContainer& res, const Qtype qType)
    {
        // debug SMHAstar
        // cout << "focal q: " << focal.size() << "\n";
        // cout << "open q: " << open.getSize() << "\n";
        // cout << "cleanup q: " << cleanup.size() << "\n";

        Node* cur = pqueue.top();

        /*cerr << "{\"g\":" << cur->getGValue() << ", ";*/
        // cerr << "\"f\":" << cur->getFValue() << ", ";
        // cerr << "\"h\":" << cur->getHValue() << ", ";
        // cerr << "\"d\":" << cur->getDValue() << ", ";
        // cerr << "\"fhat\":" << cur->getFHatValue() << ", ";
        // cerr << "\"expansion\":" << res.nodesExpanded << ", ";
        // cerr << "\"fmin\":" << fmin << ", ";
        // cerr << "\"open size\":" << open.getSize() << ", ";
        // cerr << "\"focal size\":" << focal.size() << ", ";
        // cerr << "\"fhatmin\":" << fhatmin << "}\n";

        // cout << "{\"g\":" << cur->getGValue() << ", ";
        // cout << "\"f\":" << cur->getFValue() << ", ";
        // cout << "\"h\":" << cur->getHValue() << ", ";
        // cout << "\"d\":" << cur->getDValue() << ", ";
        // cout << "\"fhat\":" << cur->getFHatValue() << ", ";
        // cout << "\"expansion\":" << res.nodesExpanded << ", ";
        // cout << "\"fmin\":" << fmin << ", ";
        // cout << "\"open size\":" << open.getSize() << ", ";
        // cout << "\"focal size\":" << focal.size() << ", ";
        // cout << "\"fhatmin\":" << fhatmin << "}\n";

        if (qType != Qtype::ANCHOR &&
            cur->getFValue() < Node::weight * anchorQueue.top()->getFValue()) {
            return processQueue(anchorQueue, res, Qtype::ANCHOR);
        }

        // Check if current node is goal
        if (this->domain.isGoal(cur->getState())) {
            this->getSolutionPath(res, cur);
            return cur->getFValue();
        }

        res.nodesExpanded++;

        dQueue.remove(cur);
        fhatQueue.remove(cur);
        anchorQueue.remove(cur);

        vector<State> children = this->domain.successors(cur->getState());
        res.nodesGenerated += children.size();

        State bestFChildState;
        Cost  bestF = numeric_limits<double>::infinity();

        for (State child : children) {

            auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
            auto newH = this->domain.heuristic(child);
            auto newD = this->domain.distance(child);

            Node* childNode =
              new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                       this->domain.epsilonDGlobal(),
                       this->domain.epsilonHVarGlobal(), child, cur);

            if (closedAnchor.find(cur->getState()) != closedAnchor.end()) {
                delete childNode;
                continue;
            }

            anchorQueue.push(childNode);

            if (childNode->getFValue() < bestF) {
                bestF           = childNode->getFValue();
                bestFChildState = child;
            }

            if (closedInadmissible.find(cur->getState()) !=
                closedInadmissible.end()) {
                continue;
            }

            dQueue.push(childNode);
            fhatQueue.push(childNode);
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

        if (qType == Qtype::ANCHOR) {
            closedAnchor[cur->getState()] = cur;
        } else {
            closedInadmissible[cur->getState()] = cur;
        }

        return -1;
    }

    PriorityQueue<Node*> dQueue;
    PriorityQueue<Node*> fhatQueue;
    PriorityQueue<Node*> anchorQueue;

    unordered_map<State, Node*, Hash> closedAnchor;
    unordered_map<State, Node*, Hash> closedInadmissible;

    int dExp;
};
