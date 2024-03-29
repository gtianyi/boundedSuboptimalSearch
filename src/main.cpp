#include "BoundedSuboptimalSearch.hpp"

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>

using namespace std;

int main(int argc, char** argv)
{

    cxxopts::Options options("./bss",
                             "This is a bounded suboptimal search benchmark");

    auto optionAdder = options.add_options();

    optionAdder(
      "d,domain",
      "domain type: randomtree, tile, pancake, racetrack, vacuumworld",
      cxxopts::value<std::string>()->default_value("vacuumworld"));

    optionAdder("s,subdomain",
                "puzzle type: uniform, inverse, heavy, reverse, sqrt; "
                "pancake type: regular, heavy, sumheavy; "
                "racetrack map : barto-big, barto-bigger, hanse-bigger-double, "
                "vacuumworld: uniform, heavy; ",
                cxxopts::value<std::string>()->default_value("uniform"));

    optionAdder("a,alg",
                "baseline algorithm: dps, ees;"
                "new algorithm: dxes, ees95, roundrobin1, roundrobin8,"
                "               roundrobind1, roundrobind8, eesdoyle,"
                //"               ees95doyle, eesli"
                //"               ees95doyle"
                "other algorithm: astar, wastar, speedy;",
                cxxopts::value<std::string>()->default_value("dxes"));

    optionAdder("f,heuristicType",
                "racetrack type : euclidean, dijkstra;"
                "pancake: gap,gapm1, gapm2",
                cxxopts::value<std::string>()->default_value("gap"));

    optionAdder("i,instance", "instance file name",
                cxxopts::value<std::string>()->default_value("2-4x4.st"));

    optionAdder("w,weight", "suboptimal bound factor",
                cxxopts::value<double>()->default_value("2"));

    optionAdder("o,performenceOut", "performence Out file",
                cxxopts::value<std::string>());

    optionAdder("v,pathOut", "path Out file", cxxopts::value<std::string>());

    optionAdder("h,help", "Print usage");

    auto args = options.parse(argc, argv);

    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    auto d      = args["domain"].as<std::string>();
    auto sd     = args["subdomain"].as<std::string>();
    auto alg    = args["alg"].as<std::string>();
    auto weight = args["weight"].as<double>();
    auto hType  = args["heuristicType"].as<string>();

    Search* searchPtr;

    // create domain world and search algorithm
    if (d == "tile") {
        SlidingTilePuzzle* world;

        if (sd == "uniform") {
            world = new SlidingTilePuzzle(cin);
        } else if (sd == "heavy") {
            world = new HeavyTilePuzzle(cin);
        } else if (sd == "inverse") {
            world = new InverseTilePuzzle(cin);
        } else if (sd == "reverse") {
            world = new ReverseTilePuzzle(cin);
        } else if (sd == "sqrt") {
            world = new SqrtTilePuzzle(cin);
        } else {
            cout << "unknown tile type!\n";
            exit(1);
        }

        searchPtr =
          new BoundedSuboptimalSearch<SlidingTilePuzzle>(*world, alg, weight);
    } else if (d == "pancake") {
        PancakePuzzle* world;

        world = new PancakePuzzle(cin);

        if (sd == "heavy") {
            world->setVariant(1);
        } else if (sd == "sumheavy") {
            world->setVariant(2);
        }

        if (hType == "gap") {
            world->setHeuristicVariant(0);
        } else if (hType == "gapm1") {
            world->setHeuristicVariant(1);
        } else if (hType == "gapm2") {
            world->setHeuristicVariant(2);
        } else {
            cout << "unknown heuristic type for " << d << ": " << hType;
            exit(1);
        }

        searchPtr =
          new BoundedSuboptimalSearch<PancakePuzzle>(*world, alg, weight);

    } else if (d == "racetrack") {
        RaceTrack* world;

        string mapFile = "/home/aifs1/gu/phd/research/workingPaper/"
                         "realtime-nancy/worlds/racetrack/map/" +
                         sd + ".track";

        ifstream map(mapFile);

        if (!map.good()) {
            cout << "map file not exist: " << mapFile << endl;
            exit(1);
        }

        world = new RaceTrack(map, cin);

        if (hType == "euclidean") {
            world->setVariant(1);
        } else if (hType == "dijkstra") {
            world->setVariant(0);
        } else {
            cout << "unknown heuristic type for " << d << ": " << hType;
            exit(1);
        }

        searchPtr = new BoundedSuboptimalSearch<RaceTrack>(*world, alg, weight);

    } else if (d == "vacuumworld") {
        Vacuumworld* world;

        world = new Vacuumworld(cin);

        if (sd == "heavy") {
            world->setVariant(1);
        }

        searchPtr =
          new BoundedSuboptimalSearch<Vacuumworld>(*world, alg, weight);

    } else {
        cout << "unknow domain!\n";
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // perform search
    auto res = searchPtr->doSearch();

    // dumpout result and observed states
    if (args.count("performenceOut")) {
        ofstream out(args["performenceOut"].as<std::string>());

        nlohmann::json record;

        record["node expanded"]  = res.nodesExpanded;
        record["node generated"] = res.nodesGenerated;
        record["solution found"] = res.solutionFound;
        record["solution cost"]  = res.solutionCost;
        record["cpu time"]       = res.totalCpuTime;
        record["instance"]       = args["instance"].as<std::string>();
        record["algorithm"]      = args["alg"].as<std::string>();
        record["weight"]         = args["weight"].as<double>();
        record["domain"]         = args["domain"].as<std::string>();
        record["subdomain"]      = args["subdomain"].as<std::string>();

        out << record;

        out.close();

    } else {
        cout << res.nodesExpanded << " " << res.solutionFound << " "
             << res.solutionCost << " " << res.initialH << " "
             << res.totalCpuTime << endl;
    }

    // dumpout solution path
    if (args.count("pathOut")) {
        ofstream out(args["pathOut"].as<std::string>());
        out << res.soltuionPath << endl;
        out.close();
    }
}
