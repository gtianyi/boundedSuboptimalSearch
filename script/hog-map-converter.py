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
import os
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

    parser.add_argument(
        '-l',
        action='store',
        dest='lowerBound',
        help='lower bound on optimal path: default(0)',
        default='0')

    parser.add_argument(
        '-u',
        action='store',
        dest='upperBound',
        help='upper bound on optimal path: default(9999)',
        default='9999')

    return parser

def main():

    parser = parseArugments()
    args = parser.parse_args()
    print(args)

    if args.is_scenario_file_converter:
        createInstancesFromScen(args.mapName, args.lowerBound, args.upperBound)
    else:
        convertMap(args.mapName)

def createInstancesFromScen(mapName, optLowerBound, optUpperBound):
    out_instance_dir = researchHome + \
        "/realtime-nancy/worlds/racetrack-"+mapName+"/"
    CHECK_FOLDER = os.path.isdir(out_instance_dir)

    if not CHECK_FOLDER:
        os.makedirs(out_instance_dir)
        print("created folder : ", out_instance_dir)

    in_hog_scen_file = researchHome + \
        "/realtime-nancy/worlds/racetrack/map/hog-maps/"+\
        mapName+".map.scen"

    map_array = loadMap(mapName)

    if len(map_array) == 0:
        return

    with open(in_hog_scen_file, "r") as inputFile:
        lineNum = -1
        instanceID = 0

        for line in inputFile:

            lineNum += 1

            lineValues = line.split()

            if len(lineValues) < 9:
                continue

            if float(lineValues[8]) < float(optLowerBound) or \
                    float(lineValues[8]) > float(optUpperBound):
                continue

            out_inst_file_name = out_instance_dir + mapName +\
                "-"+str(instanceID)+".init"
            instanceID += 1

            with open(out_inst_file_name, "w") as outFile:
                outFile.write("x y dx dy for racetrack (from " +\
                              mapName+".map.scen "+"line "+str(lineNum)+"):\n")
                outFile.write(str(lineValues[4])+" "+str(lineValues[5])+" 0 0\n")
                outFile.write("cross goal positions created from hog scenario:\n")

                goal_center_x = lineValues[6]
                goal_center_y = lineValues[7]
                outFile.write(goal_center_x+" "+goal_center_y+" 0 0\n")

                dx = [0,0,0,0,0,0,1,2,3,-1,-2,-3]
                dy = [1,2,3,-1,-2,-3,0,0,0,0,0,0]

                for i in range(12):
                    new_goal_x = int(goal_center_x) + dx[i]
                    new_goal_y = int(goal_center_y) + dy[i]

                    if not isValid(new_goal_x, new_goal_y, map_array):
                        continue

                    outFile.write(str(new_goal_x)+" "+str(new_goal_y)+" 0 0\n")


def isValid(x, y, map_array):
    return 0 <= x < len(map_array[0]) and\
           0 <= y < len(map_array) and\
           map_array[y][x] == '_'

def loadMap(mapName):
    map_file_name = researchHome + \
        "/realtime-nancy/worlds/racetrack/map/"+\
        mapName+".track"

    if not os.path.isfile(map_file_name):
        print("map file does not exist, please create track map first",
              map_file_name)
        return []

    map_array = []

    with open(map_file_name, "r") as inputFile:
        for line in inputFile:
            if line[0] != '#' or line[0] == '_':
                continue
            map_array.append(line)

    return map_array

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
