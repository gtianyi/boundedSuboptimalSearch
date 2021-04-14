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
#include "search/WAstarSearch.hpp"
#include "utility/SearchResultContainer.h"

class Search
{
public:
    virtual SearchResultContainer doSearch() = 0;
    virtual ~Search(){};
};
