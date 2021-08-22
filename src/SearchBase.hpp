#pragma once
#include "domain/HeavyTilePuzzle.h"
#include "domain/InverseTilePuzzle.h"
#include "domain/PancakePuzzle.h"
#include "domain/RaceTrack.h"
#include "domain/ReverseTilePuzzle.h"
#include "domain/SlidingTilePuzzle.h"
#include "domain/SqrtTilePuzzle.h"
#include "domain/vacuumWorld/VacuumWorld.h"
#include "search/EES.hpp"
#include "search/DXES.hpp"
#include "search/DPS.hpp"
#include "search/EES95.hpp"
#include "search/RoundRobin.hpp"
#include "search/EESRoundRobin.hpp"
#include "search/WAstarSearch.hpp"
#include "search/SpeedySearch.hpp"
#include "search/EESDoylew.hpp"
#include "search/EES95Doylew.hpp"
#include "utility/SearchResultContainer.h"

class Search
{
public:
    virtual SearchResultContainer doSearch() = 0;
    virtual ~Search(){};
};
