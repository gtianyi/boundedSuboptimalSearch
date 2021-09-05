#!/usr/bin/ python
'''
python3 script
python script code for
1. converting hog map format to racetrack map format.
2. converting hog map scenario to racetrack instance files.

Author: Tianyi Gu
Date: 09/5/2021
'''

__author__ = 'TianyiGu'

import argparse
# import os
# import re

researchHome = "/home/aifs1/gu/phd/research/workingPaper"

def parseArugments():

    parser = argparse.ArgumentParser(description='hog-map-converter')

    parser.add_argument(
        '-m',
        action='store',
        dest='mapName',
        help='map name: den520d(default)',
        default='den520d')

    parser.add_argument(
        '-s',
        action='store_true',
        default=False,
        dest='is_scenario_file_converter',
        help='Set is_scenario_file_converter to true')

    # parser.add_argument(
        # '-a',
        # action='append',
        # dest='algorithms',
        # help='algorithms: wastar, astar, pts, ptshhat, ptsnancy, bees, bees95 default(all)',
        # default=[])

    # parser.add_argument(
        # '-ht',
        # action='store',
        # dest='heuristicType',
        # help='heuristicType: racetrack: euclidean(default), dijkstra;',
        # default='euclidean')

#     parser.add_argument('-z',
    # action='store',
    # dest='size',
    # help='domain size (default: 4)',
    # default='4')

    return parser


def main():

    parser = parseArugments()
    args = parser.parse_args()
    print(args)

    if args.is_scenario_file_converter:
        print("TODO")
        # createInstancesFromScen(args.mapName)
    else:
        convertMap(args.mapName)

def convertMap(mapName):
    in_hog_map = researchHome + \
        "/realtime-nancy/worlds/racetrack/map/hog-maps/"+\
        mapName+".map"
    out_map_name = researchHome + \
        "/realtime-nancy/worlds/racetrack/map/"+\
        mapName+".track"

    out_map_string = []
    width = ""
    height = ""

    with open(in_hog_map, "r") as inputFile:
        for line in inputFile:
            if line[:3] == 'map' or line[:4] == 'type':
                continue
            if line[:5] == 'width':
                width = line.split()[1]
                continue
            if line[:6] == 'height':
                height = line.split()[1]
                continue
            map_string = ""
            for char in line:
                if char in ['.', 'G']:
                    map_string += '_'
                else:
                    map_string += '#'
            out_map_string.append(map_string)

    with open(out_map_name, "w") as outFile:
        outFile.write(width+"\n")
        outFile.write(height+"\n")
        for stringLine in out_map_string:
            outFile.write(stringLine+"\n")

if __name__ == '__main__':
    main()
