#pragma once
#include "../utility/PairHash.h"
#include "BoundedSuboptimalBase.hpp"
#include <map>

using namespace std;

template<class Domain, class Node>
class DPSRoundRobin : public BoundedSuboptimalBase<Domain, Node>
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    using BucketGH = pair<Cost, Cost>;

    enum class Qtype
    {
        undefined,
        focal,
        open,
        cleanup,
        openAndCleanup
    };

public:
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

        size_t getNodesSize() { return nodes.size(); }
    };

private:
    class BucketOpen
    {
        PriorityQueue<Bucket*>                      bucketPq;
        std::map<Cost, int>                         fCounts;
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
        BucketOpen()
            : curFmin(numeric_limits<double>::max())
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

            auto f = g + h;
            ++fCounts[f];

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

            auto f = topBucket->getG() + topBucket->getH();
            if (--fCounts[f] == 0) {
                fCounts.erase(f);
            }

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

        void updateFminAndReSort()
        {
            curFmin = std::begin(fCounts)->first;

            PriorityQueue<Bucket*> newBucketPq(Bucket::compareBucketDPS);

            while (!bucketPq.empty()) {
                auto curBucket = bucketPq.top();
                curBucket->updateFmin(curFmin);
                newBucketPq.push(curBucket);
                bucketPq.pop();
            }

            bucketPq = newBucketPq;
        }

        bool empty() { return bucketPq.empty(); }

        size_t size() { return bucketPq.size(); }

        double topDPSValue() { return bucketPq.top()->getDPSValue(); }

        double getCurFmin() { return curFmin; }

        bool isFminChanged() { return curFmin != std::begin(fCounts)->first; }

        size_t getTopBucketSize() { return bucketPq.top()->getNodesSize(); }
    };

public:
    DPSRoundRobin(Domain& domain_, const string& sorting_, int DPSExp_)
        : BoundedSuboptimalBase<Domain, Node>(domain_, sorting_)
        , DPSExp(DPSExp_)
    {}

    ~DPSRoundRobin()
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

        focal.push(initNode);
        fhatopen.push(initNode);
        cleanup.push(initNode);
        closed[this->domain.getStartState()] = initNode;

        res.initialH = inith;

        // Expand until find the goal
        while (!fhatopen.empty()) {
            // update fmin
            if (focal.isFminChanged()) {
                focal.updateFminAndReSort();
            }

            Qtype nodeFrom = Qtype::undefined;
            Node* cur      = selectNode(res.nodesExpanded, nodeFrom);

            /*cerr << "{\"g\":" << cur->getGValue() << ", ";*/
            // cerr << "\"f\":" << cur->getFValue() << ", ";
            // cerr << "\"h\":" << cur->getHValue() << ", ";
            // cerr << "\"dps\":" << open.topDPSValue() << ", ";
            // cerr << "\"expansion\":" << res.nodesExpanded << ", ";
            // cerr << "\"fmin\":" << open.getCurFmin() << ", ";
            // cerr << "\"bucket size\":" << open.size() << "}\n";

            /*cout << "{\"g\":" << cur->getGValue() << ", ";*/
            // cout << "\"f\":" << cur->getFValue() << ", ";
            // cout << "\"h\":" << cur->getHValue() << ", ";
            // cout << "\"dps\":" << open.topDPSValue() << ", ";
            // cout << "\"expansion\":" << res.nodesExpanded << ", ";
            // cout << "\"fmin\":" << open.getCurFmin() << ", ";
            // cout << "\"bucket size\":" << open.size() << ", ";
            // cout << "\"tb size\":" << open.getTopBucketSize() << "}\n";

            // Check if current node is goal
            if (this->domain.isGoal(cur->getState())) {
                this->getSolutionPath(res, cur);
                return cur->getGValue();
            }

            res.nodesExpanded++;

            cur->close();

            vector<State> children = this->domain.successors(cur->getState());
            res.nodesGenerated += children.size();

            State bestFChildState;
            Cost  bestF = numeric_limits<double>::infinity();

            for (State child : children) {

                auto newG = cur->getGValue() + this->domain.getEdgeCost(child);
                auto newH = this->domain.heuristic(child);
                auto newD = this->domain.distance(child);

                /*if (newG + newH > Node::weight * open.getCurFmin()) {*/
                // continue;
                /*}*/

                Node* childNode =
                  new Node(newG, newH, newD, this->domain.epsilonHGlobal(),
                           this->domain.epsilonDGlobal(),
                           this->domain.epsilonHVarGlobal(), child, cur);

                bool dup = duplicateDetection(childNode);

                if (!dup && childNode->getFValue() < bestF) {
                    bestF           = childNode->getFValue();
                    bestFChildState = child;
                }

                // Duplicate detection
                if (!dup) {
                    focal.push(childNode);
                    fhatopen.push(childNode);
                    cleanup.push(childNode);
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
        }

        return -1.0;
    }

private:
    Qtype roundRobinGetQtype(size_t expansion)
    {
        auto curNum = static_cast<int>(expansion) % (DPSExp + 1 + 1);
        if (curNum <= DPSExp) {
            return Qtype::focal;
        }
        if (curNum == DPSExp + 1) {
            return Qtype::open;
        }
        return Qtype::cleanup;
    }

    Node* selectNode(size_t expansionNum, Qtype& nodeFrom)
    {
        Node* cur;

        auto qtype = roundRobinGetQtype(expansionNum);
        if (qtype == Qtype::focal && !focal.empty()
            //&& focal.top()->getFHatValue() <= Node::weight * fmin
        ) {

            // cout << "pop from focal\n";
            cur = focal.top();

            nodeFrom = Qtype::focal;

            auto isOpenTop    = cur == fhatopen.top();
            auto isCleanupTop = cur == cleanup.top();

            if (isOpenTop && isCleanupTop) {
                nodeFrom = Qtype::openAndCleanup;
            } else if (isOpenTop) {
                nodeFrom = Qtype::open;
            } else if (isCleanupTop) {
                nodeFrom = Qtype::cleanup;
            }

            focal.pop();

            /*if (open.getSize() == 2568) {*/
            // cout << "cur " << cur << "\n";
            // open.prettyPrint();
            // cout << "TNULL " << open.getTNULL() << "\n";
            //// exit(1);
            /*}*/
            // cout << "open size " << open.getSize() << "\n";

            fhatopen.remove(cur);
            // open.checkTreePropertyRedKidsAreRed();
            cleanup.remove(cur);
            return cur;
        }

        if (qtype == Qtype::open && !fhatopen.empty()) {

            // cout << "pop from fhatopen\n";
            cur = fhatopen.top();

            nodeFrom = Qtype::open;
            if (cur == cleanup.top()) {
                nodeFrom = Qtype::openAndCleanup;
            }

            focal.deleteNode(cur);
            fhatopen.remove(cur);
            cleanup.remove(cur);
            return cur;
        }

        // cout << "pop from cleanup\n";
        cur = cleanup.top();
        focal.deleteNode(cur);
        fhatopen.remove(cur);
        cleanup.remove(cur);
        nodeFrom = Qtype::cleanup;
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
                    focal.deleteNode(it->second);
                    it->second->setGValue(node->getGValue());
                    it->second->setParent(node->getParent());
                    it->second->setHValue(node->getHValue());
                    it->second->setDValue(node->getDValue());
                    it->second->setEpsilonH(node->getEpsilonH());
                    it->second->setEpsilonHVar(node->getEpsilonHVar());
                    it->second->setEpsilonD(node->getEpsilonD());
                    it->second->setState(node->getState());

                    focal.push(it->second);
                    fhatopen.update(it->second);
                    cleanup.update(it->second);
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

                    fhatopen.push(it->second);
                    cleanup.push(it->second);
                    focal.push(it->second);
                }
            }
            return true;
        }
        return false;
    }

    BucketOpen           focal;
    PriorityQueue<Node*> fhatopen;
    PriorityQueue<Node*> cleanup;

    unordered_map<State, Node*, Hash> closed;

    int DPSExp;
};
