#pragma once
#include "../utility/PairHash.h"
#include "BoundedSuboptimalBase.hpp"

using namespace std;

template<class Domain, class Node>
class DPS : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    using BucketGH = pair<Cost, Cost>;

    class Bucket
    {
        Cost          h;
        Cost          g;
        vector<Node*> nodes;
        Cost          curFmin;

    public:
        Bucket(Cost g_, Cost h_, Node* node_, Cost curFmin_)
            : h(h_)
            , g(g_)
            , nodes(vector<Node*>{node_})
            , curFmin(curFmin_)
        {}

        void updateFmin(Cost fmin_) { curFmin = fmin_; }

        Cost getDPSValue() const
        {
            // cerr << "bucket {\"g\":" << g << ", ";
            // cerr << "\"h\":" << h << ", ";
            // cerr << "\"weight\":" << Node::weight << ", ";
            // cerr << "\"curFmin\":" << curFmin << "}\n";

            // return h / (1 - g / (Node::weight * curFmin));
            return (Node::weight * curFmin - g) / h;
        }

        void deleteNode(Node* node)
        {
            typename std::vector<Node*>::iterator position =
              std::find(nodes.begin(), nodes.end(), node);
            if (position != nodes.end())
                nodes.erase(position);
        }

        void pushNode(Node* node) { nodes.push_back(node); }

        Node* fetchLastNode() const
        {
            assert(!nodes.empty());
            return nodes[nodes.size() - 1];
        }

        void popLastNode() { nodes.pop_back(); }

        Cost getG() const { return g; }

        Cost getH() const { return h; }

        bool empty() const { return nodes.empty(); }

        static bool compareBucketDPS(const Bucket* b1, const Bucket* b2)
        {
            // Tie break on g-value
            if (b1->getDPSValue() == b2->getDPSValue()) {
                return b1->getG() > b2->getG();
            }
            return b1->getDPSValue() > b2->getDPSValue();
        }
    };

    class BucketOpen
    {
        PriorityQueue<Bucket*>                      bucketPq;
        unordered_map<BucketGH, Bucket*, pair_hash> bucketMap;
        Cost                                        curFmin;

    private:
        void fixBucket(Bucket* bucket_)
        {
            if (bucket_->empty()) {
                auto g       = bucket_->getG();
                auto h       = bucket_->getH();
                auto ghValue = make_pair(g, h);

                bucketMap.erase(ghValue);
                bucketPq.remove(bucket_);
                delete bucket_;
            }
        }

    public:
        BucketOpen(Cost fmin_)
            : curFmin(fmin_)
        {
            bucketPq.swapComparator(Bucket::compareBucketDPS);
        }

        ~BucketOpen()
        {
            // delete all of the nodes from the last expansion phase
            for (typename unordered_map<BucketGH, Bucket*, pair_hash>::iterator
                   it = bucketMap.begin();
                 it != bucketMap.end(); it++)
                delete it->second;

            bucketMap.clear();
        }

        void push(Node* node)
        {
            auto g       = node->getGValue();
            auto h       = node->getHValue();
            auto ghValue = make_pair(g, h);

            if (bucketMap.find(ghValue) != bucketMap.end()) {
                auto curBucket = bucketMap[ghValue];
                curBucket->pushNode(node);
                return;
            }

            Bucket* bucket = new Bucket(g, h, node, curFmin);
            bucketPq.push(bucket);
            bucketMap[ghValue] = bucket;
        }

        Node* top()
        {
            auto topBucket = bucketPq.top();
            return topBucket->fetchLastNode();
        }

        void pop()
        {
            if (bucketPq.empty()) {
                return;
            }

            auto topBucket = bucketPq.top();
            topBucket->popLastNode();

            fixBucket(topBucket);
        }

        void deleteNode(Node* node)
        {
            auto g       = node->getGValue();
            auto h       = node->getHValue();
            auto ghValue = make_pair(g, h);

            if (bucketMap.find(ghValue) == bucketMap.end()) {
                return;
            }

            auto curBucket = bucketMap[ghValue];
            curBucket->deleteNode(node);
            fixBucket(curBucket);
        }

        void reSort(Cost fmin_)
        {
            curFmin = fmin_;

            PriorityQueue<Bucket*> newBucketPq;

            while (!bucketPq.empty()) {
                auto curBucket = bucketPq.top();
                curBucket->updateFmin(fmin_);
                newBucketPq.push(curBucket);
                bucketPq.pop();
            }

            bucketPq = newBucketPq;
        }

        bool empty() { return bucketPq.empty(); }

        size_t size() { return bucketPq.size(); }

        double topDPSValue() { return bucketPq.top()->getDPSValue(); }
    };

public:
    DPS(Domain& domain_, const string& sorting_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
    {}

    ~DPS()
    {
        // delete all of the nodes from the last expansion phase
        for (typename unordered_map<State, Node*, Hash>::iterator it =
               closed.begin();
             it != closed.end(); it++)
            delete it->second;

        closed.clear();
    }

    double run(SearchResultContainer& res)
    {
        auto inith = this->domain.heuristic(this->domain.getStartState());
        auto initD = this->domain.distance(this->domain.getStartState());

        // Get the start node
        Node* initNode = new Node(
          0, inith, initD, this->domain.epsilonHGlobal(),
          this->domain.epsilonDGlobal(), this->domain.epsilonHVarGlobal(),
          this->domain.getStartState(), NULL);

        fmin     = initNode->getFValue();
        fminNode = initNode;

        BucketOpen open(fmin);

        open.push(initNode);

        res.initialH = inith;

        // Expand until find the goal
        while (!open.empty()) {
            // Pop lowest fhat-value off open
            Node* cur = open.top();

            cerr << "{\"g\":" << cur->getGValue() << ", ";
            cerr << "\"f\":" << cur->getFValue() << ", ";
            cerr << "\"h\":" << cur->getHValue() << ", ";
            cerr << "\"dps\":" << open.topDPSValue() << ", ";
            cerr << "\"expansion\":" << res.nodesExpanded << ", ";
            cerr << "\"fmin\":" << fmin << ", ";
            cerr << "\"open size\":" << open.size() << "}\n";

            cout << "{\"g\":" << cur->getGValue() << ", ";
            cout << "\"f\":" << cur->getFValue() << ", ";
            cout << "\"h\":" << cur->getHValue() << ", ";
            cout << "\"dps\":" << open.topDPSValue() << ", ";
            cout << "\"expansion\":" << res.nodesExpanded << ", ";
            cout << "\"fmin\":" << fmin << ", ";
            cout << "\"open size\":" << open.size() << "}\n";

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getGValue();
            }

            res.nodesExpanded++;

            open.pop();
            cur->close();

            vector<State> children = this->domain.successors(cur->getState());
            res.nodesGenerated += children.size();

            Node* bestChildNode;
            Cost  bestF = numeric_limits<double>::infinity();

            for (State child : children) {

                auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
                auto newH = this->domain.heuristic(child);
                auto newD = this->domain.distance(child);

                Node* childNode =
                  new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                           this->domain.epsilonDGlobal(),
                           this->domain.epsilonHVarGlobal(), child, cur);

                bool dup = duplicateDetection(childNode, open);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF         = childNode->getFValue();
                    bestChildNode = childNode;
                }

                // Duplicate detection
                if (!dup) {
                    open.push(childNode);
                    closed[child] = childNode;
                } else
                    delete childNode;
            }

            // update fmin
            if (cur == fminNode) {

                if (bestF != fmin) {
                    fmin = bestF;
                    open.reSort(fmin);
                }

                fminNode = bestChildNode;
            }
        }

        return -1.0;
    }

private:
    bool duplicateDetection(Node* node, BucketOpen& open)
    {
        // Check if this state exists
        typename unordered_map<State, Node*, Hash>::iterator it =
          closed.find(node->getState());

        if (it != closed.end()) {
            // if the new node is better, update it on close
            if (node->getGValue() < it->second->getGValue()) {

                // This state has been generated before,
                // check if its node is on OPEN
                if (it->second->onOpen()) {
                    // This node is on OPEN, keep the better g-value
                    // cout << "dup on open " << it->second << "\n";
                    open.deleteNode(it->second);

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setState(node->getState());

                    open.push(it->second);
                } else {
                    it->second->reopen();

                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setState(node->getState());

                    open.push(it->second);
                }
            }
            return true;
        }
        return false;
    }

    void sortOpen() { return; }

    Cost                              fmin;
    Node*                             fminNode;
    unordered_map<State, Node*, Hash> closed;
};
