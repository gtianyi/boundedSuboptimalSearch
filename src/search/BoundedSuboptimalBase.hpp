#pragma once
#include "../utility/PriorityQueue.h"
#include "../utility/RBTree.h"
#include "../utility/SearchResultContainer.h"
#include <functional>
#include <unordered_map>

using namespace std;

template<class Domain, class Node>
class BoundedSuboptimalBase
{
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

public:
    BoundedSuboptimalBase(Domain& domain_, const string& sorting_)
        : domain(domain_)
        , sortingFunction(sorting_)
    {}

    virtual ~BoundedSuboptimalBase(){};

    virtual double run(SearchResultContainer& res) = 0;

protected:
    virtual void sortOpen() = 0;

    void getSolutionPath(SearchResultContainer& res, Node* goal)
    {
        auto cur = goal;

        string p = "";
        while (cur) {
            p   = cur->getState().toString() + p;
            cur = cur->getParent();
        }

        res.soltuionPath = p;
    }

protected:
    Domain&      domain;
    const string sortingFunction;
};
