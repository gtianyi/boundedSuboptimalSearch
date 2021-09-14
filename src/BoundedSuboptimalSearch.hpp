#pragma once

#include "SearchBase.hpp"

#include <boost/math/special_functions/erf.hpp>
#include <cmath>
#include <ctime>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

template<class Domain>
class BoundedSuboptimalSearch : public Search
{
public:
    typedef typename Domain::State     State;
    typedef typename Domain::Cost      Cost;
    typedef typename Domain::HashState Hash;

    struct Node
    {
        Cost  g;
        Cost  h;
        Cost  d;
        Cost  epsH;
        Cost  epsD;
        Cost  epsHVar;
        int   delayCntr;
        State stateRep;
        Node* parent;
        bool  open;

        static double weight;

        Cost   dxesValue;
        Cost   fhatminVar;
        double dxesProbValue;
        Node*  fhatminNode;

    public:
        Cost getGValue() const { return g; }
        Cost getHValue() const { return h; }
        Cost getFValue() const { return g + h; }
        Cost getDValue() const { return d; }
        Cost getEpsilonH() const { return epsH; }
        Cost getEpsilonHVar() const { return epsHVar; }
        Cost getEpsilonD() const { return epsD; }
        Cost getDHatValue() const { return (d / (1.0 - epsD)); }
        Cost getHHatValue() const { return h + getDHatValue() * epsH; }
        Cost getFHatValue() const { return g + getHHatValue(); }

        State getState() const { return stateRep; }
        Node* getParent() const { return parent; }

        // for bounded cost search
        // petrick pts
        // Cost getPTSValue() const { return 1 / (1 - h / (bound + 1 - g)); }
        // original pts
        // Cost getPTSValue() const { return h / (bound - g); }
        // BEES pts
        Cost getPTSValue() const
        {
            cout << "TODO\n";
            assert(false);
            // return h / (1 - (g / bound));
        }
        Cost getPTSHHatValue() const
        {
            // return 1 / (1 - getHHatValue() / (bound + 1 - g));
            // return getHHatValue() / (bound - g);
            // return getHHatValue() / (1 - (g / bound));
            cout << "TODO\n";
            assert(false);
        }

        bool isZeroVarianceCostDist()
        {
            return getHValue() == getHHatValue() || getHValue() == 0;
        }

        bool isZeroVarianceBoundDist() { return fabs(fhatminVar - 0) < 0.001; }

        double computeExpectedEffortProbValue()
        {
            // cout << "is Cost Dist zero Var " << isZeroVarianceCostDist()
            //<< "\n";
            // cout << "is Bound Dist zero Var " << isZeroVarianceBoundDist()
            //<< "\n";

            auto boundMean = weight * fhatminNode->getFHatValue();

            /*cout << "cost mean " << getFHatValue() << "\n";*/
            // cout << "cost var " << (getFHatValue() - getFValue()) / 2 <<
            // "\n"; cout << "bound mean " << boundMean << "\n"; cout << "bound
            // var " << fhatminVar << "\n";

            if (isZeroVarianceCostDist() && isZeroVarianceBoundDist()) {
                auto prob = getFValue() <= boundMean ? 1. : 0.;
                return prob;
            }

            if (isZeroVarianceBoundDist()) {
                auto mean               = getFHatValue();
                auto standard_deviation = std::abs(mean - getFValue()) / 2;
                auto cdf_xi = cumulative_distribution((boundMean + 0.5 - mean) /
                                                      standard_deviation);
                auto cdf_alpha = cumulative_distribution((getFValue() - mean) /
                                                         standard_deviation);

                assert(cdf_xi >= 0 && cdf_xi <= 1);
                assert(cdf_alpha >= 0 && cdf_alpha <= 1);

                auto prob = (cdf_xi - cdf_alpha) / (1 - cdf_alpha);
                return prob;
            }

            auto mean = boundMean - getFHatValue();
            auto variance =
              pow(std::abs(getFHatValue() - getFValue()) / 2, 2.0) +
              // fhatminNode->getDHatValue() * fhatminVar;
              fhatminVar;
            auto standard_deviation = sqrt(variance);

            // compute P(X>0)
            auto prob =
              cumulative_distribution((mean - 0) / standard_deviation);

            return prob;
        }

        void computeExpectedEffortValue(Node* fhatminNode_, double fhatminVar_)
        {
            fhatminNode = fhatminNode_;
            fhatminVar  = fhatminVar_;

            auto prob = computeExpectedEffortProbValue();

            // cout << "prob " << prob << "\n";
            dxesProbValue = prob;

            dxesValue = getDHatValue() / prob;

            // cout << "dxes value " << dxesValue << "\n";
        }

        Cost getDXESValue() const { return dxesValue; }
        Cost getDXESProbValue() const { return dxesProbValue; }

        void setHValue(Cost val) { h = val; }
        void setGValue(Cost val) { g = val; }
        void setDValue(Cost val) { d = val; }
        void setEpsilonH(Cost val) { epsH = val; }
        void setEpsilonHVar(Cost val) { epsHVar = val; }
        void setEpsilonD(Cost val) { epsD = val; }
        void setState(State s) { stateRep = s; }
        void setParent(Node* p) { parent = p; }

        void incDelayCntr() { delayCntr++; }
        int  getDelayCntr() { return delayCntr; }

        Node(Cost g_, Cost h_, Cost d_, Cost epsH_, Cost epsD_, Cost epsHVar_,
             State state_, Node* parent_)
            : g(g_)
            , h(h_)
            , d(d_)
            , epsH(epsH_)
            , epsD(epsD_)
            , epsHVar(epsHVar_)
            , delayCntr(0)
            , stateRep(state_)
            , parent(parent_)
            , open(true)
        {}

        friend std::ostream& operator<<(std::ostream& stream, const Node& node)
        {
            stream << node.getState() << "\n";
            stream << "f: " << node.getFValue() << "\n";
            stream << "g: " << node.getGValue() << "\n";
            stream << "h: " << node.getHValue() << "\n";
            // stream << "pts : " << node.getPTSValue() << "\n";
            stream << "action generated by: \n";
            stream << node.getParent()->getState() << "\n";
            stream << "-----------------------------------------------"
                   << "\n";
            stream << "\n";
            return stream;
        }

        void treePrint(string indent, string color, const Node& node)
        {
            // stream << node.getState() << "\n";
            cout << "\n";
            cout << indent << color << "\n";
            cout << indent << "f: " << node.getFValue() << "\n";
            cout << indent << "g: " << node.getGValue() << "\n";
            cout << indent << "h: " << node.getHValue() << "\n";
            // cout << "action generated by: \n";
            // cout << node.getParent()->getState() << "\n";
            // cout << indent <<
            // "-----------------------------------------------"
            //<< "\n";
            cout << "\n";
        }

        bool onOpen() { return open; }
        void close() { open = false; }
        void reopen() { open = true; }

        static bool compareNodesF(const Node* n1, const Node* n2)
        {
            // Tie break on g-value
            if (n1->getFValue() == n2->getFValue()) {
                return n1->getGValue() > n2->getGValue();
            }
            return n1->getFValue() < n2->getFValue();
        }

        static double getNodeF(const Node* n) { return n->getFValue(); }

        static bool compareNodesFHat(const Node* n1, const Node* n2)
        {
            // Tie break on g-value
            if (n1->getFHatValue() == n2->getFHatValue()) {
                return n1->getGValue() > n2->getGValue();
            }
            return n1->getFHatValue() < n2->getFHatValue();
        }

        static double getNodeFHat(const Node* n) { return n->getFHatValue(); }

        static bool compareNodesWeightedF(const Node* n1, const Node* n2)
        {
            // Tie break on g-value
            auto n1WeightedF = n1->getGValue() + weight * n1->getHValue();
            auto n2WeightedF = n2->getGValue() + weight * n2->getHValue();
            if (n1WeightedF == n2WeightedF) {
                return n1->getGValue() > n2->getGValue();
            }
            return n1WeightedF < n2WeightedF;
        }

        static bool compareNodesDHat(const Node* n1, const Node* n2)
        {
            // Tie break on low fhat, low f, high g-value
            if (n1->getDHatValue() == n2->getDHatValue()) {
                if (n1->getFHatValue() == n2->getFHatValue()) {
                    if (n1->getFValue() == n2->getFValue()) {
                        if (n1->getGValue() == n2->getGValue()) {
                            return n1->getState().key() > n2->getState().key();
                        }
                        return n1->getGValue() > n2->getGValue();
                    }
                    return n1->getFValue() < n2->getFValue();
                }
                return n1->getFHatValue() < n2->getFHatValue();
            }
            return n1->getDHatValue() < n2->getDHatValue();
        }

        static bool compareNodesPTS(const Node* n1, const Node* n2)
        {
            // Tie break on low f, high g-value, low d
            if (n1->getPTSValue() == n2->getPTSValue()) {
                if (n1->getFValue() == n2->getFValue()) {
                    if (n1->getGValue() == n1->getGValue()) {
                        return n1->getDValue() < n2->getDValue();
                    }
                    return n1->getGValue() > n2->getGValue();
                }
                return n1->getFValue() < n2->getFValue();
            }
            return n1->getPTSValue() < n2->getPTSValue();
        }

        static bool compareNodesPTSHHat(const Node* n1, const Node* n2)
        {
            // Tie break on low fhat, high g-value, low d
            if (n1->getPTSHHatValue() == n2->getPTSHHatValue()) {
                if (n1->getFHatValue() == n2->getFHatValue()) {
                    if (n1->getGValue() == n1->getGValue()) {
                        return n1->getDValue() < n2->getDValue();
                    }
                    return n1->getGValue() > n2->getGValue();
                }
                return n1->getFHatValue() < n2->getFHatValue();
            }
            return n1->getPTSHHatValue() < n2->getPTSHHatValue();
        }

        static bool compareNodesD(const Node* n1, const Node* n2)
        {
            // Tie break on low fhat, low f, high g-value
            if (n1->getDValue() == n2->getDValue()) {
                if (n1->getFHatValue() == n2->getFHatValue()) {
                    if (n1->getFValue() == n2->getFValue()) {
                        return n1->getGValue() > n2->getGValue();
                    }
                    return n1->getFValue() < n2->getFValue();
                }
                return n1->getFHatValue() < n2->getFHatValue();
            }
            return n1->getDValue() < n2->getDValue();
        }

        static bool compareNodesExpectedEffort(const Node* n1, const Node* n2)
        {
            // Tie break on low f, high g-value, low d
            auto n1DXESValue = n1->getDXESValue();
            auto n2DXESValue = n2->getDXESValue();
            if (n1DXESValue == n2DXESValue) {
                /*if (n1->getFValue() == n2->getFValue()) {*/
                // if (n1->getGValue() == n1->getGValue()) {
                // return n1->getDValue() < n2->getDValue();
                //}
                // return n1->getGValue() > n2->getGValue();
                //}
                /*return n1->getFValue() < n2->getFValue();*/
                return compareNodesD(n1, n2);
            }
            return n1DXESValue < n2DXESValue;
        }

        static bool compareNodesOnProb(const Node* n1, const Node* n2)
        {
            // Tie break on low f, high g-value, low d
            auto n1DXESProbValue = n1->getDXESProbValue();
            auto n2DXESProbValue = n2->getDXESProbValue();
            if (n1DXESProbValue == n2DXESProbValue) {
                return compareNodesD(n1, n2);
            }
            return n1DXESProbValue > n2DXESProbValue;
        }

    private:
        double cumulative_distribution(double x) const
        {
            return (1 + boost::math::erf(
                          x / std::sqrt(2.),
                          boost::math::policies::make_policy(
                            boost::math::policies::promote_double<false>()))) /
                   2.;
        }
    };

    BoundedSuboptimalSearch(Domain& domain_, const string& algStr,
                            double weight_)
        : domain(domain_)
    {
        Node::weight = weight_;
        if (algorithms.find(algStr) == algorithms.end()) {
            std::cerr
              << "BoundedSuboptimalSearch.hpp: unknown algorithm name\n";
            exit(0);
        }
        algorithm = algorithms[algStr];
    }

    ~BoundedSuboptimalSearch() { delete algorithm; }

    SearchResultContainer doSearch()
    {
        SearchResultContainer res;

        clock_t startTime = clock();

        // start search
        double solutionCost = algorithm->run(res);

        res.solutionFound = solutionCost != -1.0;
        res.solutionCost  = solutionCost;

        res.totalCpuTime =
          static_cast<double>(clock() - startTime) / CLOCKS_PER_SEC;

        return res;
    }

protected:
    Domain&                                                     domain;
    BoundedSuboptimalBase<Domain, Node>*                        algorithm;
    unordered_map<string, BoundedSuboptimalBase<Domain, Node>*> algorithms{
      {"wastar", new WAstarSearch<Domain, Node>(domain, "wastar")},
      {"speedy", new SpeedySearch<Domain, Node>(domain, "speedy")},
      {"ees", new EES<Domain, Node>(domain, "ees")},
      {"dxes", new DXES<Domain, Node>(domain, "dxes")},
      {"dxes95", new DXES95<Domain, Node>(domain, "dxes95")},
      {"dps", new DPS<Domain, Node>(domain, "dps")},
      {"dpsroundrobin", new DPSRoundRobin<Domain, Node>(domain, "dpsroundrobin", 1)},
      {"bfsonp", new BFSonP<Domain, Node>(domain, "bfsonp")},
      {"ees95", new EES95<Domain, Node>(domain, "ees95")},
      {"eesdoylew", new EESDoylew<Domain, Node>(domain, "eesdoylew")},
      {"ees95doylew", new EES95Doylew<Domain, Node>(domain, "ees95doylew")},
      {"eesli", new EESLi<Domain, Node>(domain, "eesli")},
      {"dbees", new DynamicBEES<Domain, Node>(domain, "dbees")},
      {"smhastar", new SMHAstar<Domain, Node>(domain, "smhastar")},
      {"roundrobin1", new RoundRobin<Domain, Node>(domain, "roundrobin1", 1)},
      {"roundrobin8", new RoundRobin<Domain, Node>(domain, "roundrobin8", 8)},
      {"roundrobind1",
       new EESRoundRobin<Domain, Node>(domain, "roundrobind1", 1)},
      {"roundrobind8",
       new EESRoundRobin<Domain, Node>(domain, "roundrobind8", 8)}
      // let's disable roundrobotinx for now, Node::weight is
      // set after this table.  so might use wrong, but not sure to make
      // it elegently set correctly here.
      /*{"roundrobindx",*/
      // new EESRoundRobin<Domain, Node>(domain, "roundrobindx",
      /*static_cast<int>(Node::weight))},*/
    };
};

template<class Domain>
double BoundedSuboptimalSearch<Domain>::Node::weight;
