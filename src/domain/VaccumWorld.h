#pragma once
#include "../utility/SlidingWindow.h"
#include "../utility/debug.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <bitset>

using namespace std;

struct pair_hash
{
    template<class T1, class T2>
    std::size_t operator()(std::pair<T1, T2> const& pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 ^ h2;
    }
};

class VaccumWorld
{
    using Location = pair<size_t, size_t>;

public:
    typedef double        Cost;
    static constexpr Cost COST_MAX = std::numeric_limits<Cost>::max();
    static constexpr int  INT_MAX  = std::numeric_limits<int>::max();

    class State
    {
    public:
        State() {}

        State(int x_, int y_, std::unordered_set<Location, pair_hash>&& dirts_,
              unsigned int cleanedDirtsCount_)
            : x(x_)
            , y(y_)
            , dirts(dirts_)
            , cleanedDirtsCount(cleanedDirtsCount_)
        {
            generateKey();
        }

        friend std::ostream& operator<<(std::ostream&             stream,
                                        const VaccumWorld::State& state)
        {
            stream << "x: " << state.x << " ";
            stream << "y: " << state.y << " ";

            int dirtCount = 0;
            for (const auto& dirt : state.dirts) {
                stream << "dirt" << dirtCount << ": " << dirt.first << " "
                       << dirt.second << " ";
                dirtCount += 1;
            }

            stream << "\n";

            return stream;
        }

        bool operator==(const State& state) const
        {
            return theKey == state.key();
        }

        bool operator!=(const State& state) const
        {
            return theKey != state.key();
        }

        void generateKey()
        {
            // This will provide a unique hash for every state in the racetrack,
            unsigned long long val        = 0xCBF29CE484222325;
            unsigned int       x_unsigned = static_cast<unsigned int>(x);

            val = val ^ x_unsigned ^ (static_cast<size_t>(y) << 8);

            unsigned int shift = 8;
            for (const auto& dirt : dirts) {
                shift *= 2;
                val = val ^ (static_cast<size_t>(dirt.first) << shift);
                shift *= 2;
                val = val ^ (static_cast<size_t>(dirt.second) << shift);
            }

            theKey = val;
        }

        unsigned long long key() const { return theKey; }

        int getX() const { return x; }

        int getY() const { return y; }

        std::unordered_set<Location, pair_hash> getDirts() const
        {
            return dirts;
        }

        unsigned int getCleanedDirtsCount() const { return cleanedDirtsCount; }

        bool isNoDirt() const { return dirts.empty(); }

        bool hasDirtOnLocation(const Location& loc) const
        {
            return dirts.find(loc) != dirts.end();
        }

        std::string toString() const
        {
            std::string s = to_string(x) + " " + to_string(y) + " ";

            int dirtCount = 0;
            for (const auto& dirt : dirts) {
                s = s + "dirt" + to_string(dirtCount) + ": " +
                    to_string(dirt.first) + " " + to_string(dirt.second) + " ";
                dirtCount += 1;
            }

            s += "\n";

            return s;
        }

        /*void dumpToProblemFile(ofstream& f)*/
        //{
        // f << "x y dx dy for racetrack:\n";
        // f << x << " " << y << " " << dx << " " << dy << "\n";
        // f << "goal positions are in the map";
        /*}*/

    private:
        int                                     x, y;
        std::unordered_set<Location, pair_hash> dirts;
        unsigned int                            cleanedDirtsCount;
        unsigned long long                      theKey =
          std::numeric_limits<unsigned long long>::max();
    };

    struct HashState
    {
        std::size_t operator()(const State& s) const { return s.key(); }
    };

    struct Action
    {
        int  moveX;
        int  moveY;
        bool vaccum;
    };

    VaccumWorld(std::istream& input)
    {
        parseInput(input);
        initilaizeActions();
        costVariant = 0; // default
    }

    void setVariant(int variant) { costVariant = variant; }

    bool isGoal(const State& s) const { return s.isNoDirt(); }

    Cost distance(const State& state)
    {
        // Check if the distance of this state has been updated
        if (correctedD.find(state) != correctedD.end()) {
            return correctedD[state];
        }

        Cost d = greedyTraversal(state);

        updateDistance(state, d);

        return correctedD[state];
    }

    /*Cost distanceErr(const State& state)*/
    //{
    //// Check if the distance error of this state has been updated
    // if (correctedDerr.find(state) != correctedDerr.end()) {
    // return correctedDerr[state];
    //}

    // Cost derr = dijkstraMaxH(state);

    // updateDistanceErr(state, derr);

    // return correctedDerr[state];
    /*}*/

    Cost heuristic(const State& state) { return minimumSpanningTree(state); }

    Cost epsilonHGlobal() { return curEpsilonH; }

    Cost epsilonDGlobal() { return curEpsilonD; }

    void updateEpsilons()
    {
        if (expansionCounter == 0) {
            curEpsilonD = 0;
            curEpsilonH = 0;

            return;
        }

        curEpsilonD = epsilonDSum / expansionCounter;

        curEpsilonH = epsilonHSum / expansionCounter;
    }

    void pushEpsilonHGlobal(double eps)
    {
        if (eps < 0)
            eps = 0;
        else if (eps > 1)
            eps = 1;

        epsilonHSum += eps;
        expansionCounter++;
    }

    void pushEpsilonDGlobal(double eps)
    {
        if (eps < 0)
            eps = 0;
        else if (eps > 1)
            eps = 1;

        epsilonDSum += eps;
        expansionCounter++;
    }

    void updateDistance(const State& state, Cost value)
    {
        correctedD[state] = value;
    }

    void updateDistanceErr(const State& state, Cost value)
    {
        correctedDerr[state] = value;
    }

    void updateHeuristic(const State& state, Cost value)
    {
        correctedH[state] = value;
    }

    /*Cost maxT(const State& startState, const Location& endLoc) const {*/
    // return max(abs(startState.getX() - endLoc.first) / maxXSpeed,
    // abs(startState.getY() - endLoc.second) / maxYSpeed);
    //}

    // Cost maxH(const State& state) const {
    // Cost c = COST_MAX;

    // for (const auto goalLoc : finishline) {
    // auto newC = maxT(state, goalLoc);
    // if (c > newC)
    // c = newC;
    //}

    // return c;
    /*}*/

    double getBranchingFactor() const { return 9; }

    bool isLegalLocation(int x, int y) const
    {
        return x >= 0 && y >= 0 && static_cast<size_t>(x) < mapWidth &&
               static_cast<size_t>(y) < mapHeight &&
               blockedCells.find(
                 Location(static_cast<size_t>(x), static_cast<size_t>(y))) ==
                 blockedCells.end();
    }

    std::vector<State> successors(const State& state)
    {
        std::vector<State> successors;

        for (auto action : actions) {

            if (action.vaccum) {

                auto robotLoc = Location(state.getX(), state.getY());
                if (state.hasDirtOnLocation(robotLoc)) {
                    auto newDirts = state.getDirts();
                    newDirts.erase(robotLoc);
                    State succ(state.getX(), state.getY(), std::move(newDirts),
                               state.getCleanedDirtsCount() + 1);
                    successors.push_back(succ);
                }

                continue;
            }

            int newX = state.getX() + action.moveX;
            int newY = state.getY() + action.moveY;

            if (isLegalLocation(newX, newY)) {
                auto  dirts = state.getDirts();
                State succ(newX, newY, std::move(dirts),
                           state.getCleanedDirtsCount());

                successors.push_back(succ);
            }
        }

        /*// recording predecessor*/
        // for (const auto& succ : successors) {
        // predecessorsTable[succ].push_back(state);
        /*}*/

        return successors;
    }

    /*const std::vector<State> predecessors(const State& state) const*/
    //{
    //// DEBUG_MSG("preds table size: "<<predecessorsTable.size());
    // if (predecessorsTable.find(state) != predecessorsTable.end())
    // return predecessorsTable.at(state);
    // return vector<State>();
    /*}*/

    const State getStartState() const { return startState; }

    Cost getEdgeCost(State state)
    {
        // Variants:
        // 0: uniform cost.
        // 1: heavy cost: one plus the number of dirt piles the robot has
        //    cleaned up (the weight from the dirt drains the battery faster).
        if (costVariant == 1)
            return state.getCleanedDirtsCount() + 1;
        return 1;
    }

    string getDomainInformation()
    {
        string info = "{ \"Domain\": \"vaccum world\", \"widthxheight\": " +
                      std::to_string(mapHeight) + "x" +
                      std::to_string(mapHeight) + " }";
        return info;
    }

    string getDomainName() { return "VaccumWorld"; }

    void initialize(string policy, int la)
    {
        epsilonDSum      = 0;
        epsilonHSum      = 0;
        expansionCounter = 0;
        curEpsilonD      = 0;
        curEpsilonH      = 0;

        expansionPolicy = policy;
        lookahead       = la;
        correctedD.clear();
        correctedH.clear();
        correctedDerr.clear();
        expansionDelayWindow.clear();
    }

    void pushDelayWindow(int val) { expansionDelayWindow.push(val); }

    double averageDelayWindow()
    {
        if (expansionDelayWindow.size() == 0)
            return 1;

        double avg = 0;

        for (auto i : expansionDelayWindow) {
            avg += i;
        }

        avg /= static_cast<double>(expansionDelayWindow.size());

        return avg;
    }

    string getSubDomainName() const { return ""; }

private:
    void parseInput(std::istream& input)
    {
        string line;
        getline(input, line);
        stringstream ss(line);
        ss >> mapWidth;

        getline(input, line);

        stringstream ss2(line);
        ss2 >> mapHeight;

        std::unordered_set<Location, pair_hash> dirts;

        for (size_t y = 0; y < mapHeight; y++) {

            getline(input, line);
            stringstream ss3(line);

            for (size_t x = 0; x < mapWidth; x++) {
                char cell;
                ss3 >> cell;

                switch (cell) {
                    case '#':
                        blockedCells.insert(Location(x, y));
                        break;
                    case '*':
                        dirts.insert(Location(x, y));
                        break;
                    case '@':
                        startLocation = Location(x, y);
                        break;
                }
            }
        }

        cout << "size: " << mapWidth << "x" << mapHeight << "\n";
        cout << "blocked: " << blockedCells.size() << "\n";
        cout << "dirts: " << dirts.size() << "\n";

        startState =
          State(static_cast<int>(startLocation.first),
                static_cast<int>(startLocation.second), std::move(dirts), 0);
    }

    void initilaizeActions()
    {
        // move left, right, up, down, and vaccum
        actions = {Action{-1, 0, false}, Action{1, 0, false},
                   Action{0, -1, false}, Action{0, 1, false},
                   Action{0, 0, true}};
    }

    std::unordered_set<Location, pair_hash> blockedCells;
    vector<Action>                          actions;
    vector<vector<size_t>>                  dijkstraMap;
    size_t                                  mapWidth;
    size_t                                  mapHeight;
    Location                                startLocation;
    State                                   startState;

    int costVariant;

    SlidingWindow<int>                    expansionDelayWindow;
    unordered_map<State, Cost, HashState> correctedH;
    unordered_map<State, Cost, HashState> correctedD;
    unordered_map<State, Cost, HashState> correctedDerr;
    // unordered_map<State, vector<State>, HashState> predecessorsTable;

    double epsilonHSum;
    double epsilonDSum;
    double curEpsilonH;
    double curEpsilonD;
    double expansionCounter;

    string expansionPolicy;
    int    lookahead;
};
