#!/usr/bin/ python
'''
python3 script
python script code for fixing json result format.

Author: Tianyi Gu
Date: 09/29/2020
'''

__author__ = 'TianyiGu'

import argparse
import os
import re
import json

researchHome = "/home/aifs1/gu/phd/research/workingPaper"
# researchHome = "/home/aifs1/gu/Downloads"


def parseArugments():

    parser = argparse.ArgumentParser(description='fixJson')

    parser.add_argument(
        '-d',
        action='store',
        dest='domain',
        help='domain: tile(default), pancake, racetrack, vacuumworld',
        default='pancake')

    parser.add_argument(
        '-s',
        action='store',
        dest='subdomain',
        help='subdomain: tile: uniform(default), heavy, inverse; \
        pancake: regular, heavy; \
        racetrack : barto-big,uniform-small, barto-bigger, hanse-bigger-double',
        default='regular')

    parser.add_argument(
        '-a',
        action='append',
        dest='algorithms',
        help='algorithms: ees, ees95 default(all)',
        default=['ees'])

    parser.add_argument(
        '-ht',
        action='store',
        dest='heuristicType',
        help='heuristicType: racetrack: euclidean(default), dijkstra; pancake: gap, gapm2',
        default='gap')

    parser.add_argument('-z',
    action='store',
    dest='size',
    help='domain size (default: 50)',
    default='50')

    parser.add_argument('-b',
    action='store',
    dest='bound',
    help='suboptimality bound (default: 8)',
    default='8')


    return parser


def main():

    parser = parseArugments()
    args = parser.parse_args()
    print(args)

    algorithms = ['ees', 'speedy']

    if len(args.algorithms) != 0:
        algorithms = args.algorithms


    fileDir = getResultFullPath(args)

    resultDict = {}

    parseAlgDataAddToDict(args, "speedy", fileDir, resultDict)

    for algorithm in algorithms:
        parseAlgDataAddToDict(args, algorithm, fileDir, resultDict)

def getResultFullPath(args):
    resultDir = "results"

    fileDir = researchHome + "/boundedSuboptimalSearch/" + resultDir + "/" + \
        args.domain+"/"+args.subdomain+"/"

    if args.domain in ["racetrack", "pancake"]:
        fileDir += args.heuristicType+"/"

    if not os.path.exists(fileDir):
        print("not found domain configuration, skip ", \
              args.domain, args.subdomain, args.heuristicType)
        return ""

    return fileDir

def parseAlgDataAddToDict(args, algorithm, fileDir, resultDict):

    print("processing ", algorithm)

    fileDir += algorithm+"/"

    if not os.path.exists(fileDir):
        print("not found algorithm, skip ", algorithm)
        return

    resultDict[algorithm] = {}
    maxDiff = 0
    maxDiffInst = ""
    for fileName in os.listdir(fileDir):

        # print("processing ", fileName)
        if fileName[-5:] != ".json":
            continue

        numbersInFileName = re.findall(r'\d*\.?\d+', fileName)
        sizeStr = numbersInFileName[1]

        if args.domain == "pancake" and sizeStr != args.size:
            continue

        boundValueStr = numbersInFileName[0]

        if algorithm != "speedy" and boundValueStr != args.bound:
            continue

        with open(fileDir + "/" + fileName) as json_data:
            resultData = json.load(json_data)
            resultDict[algorithm][fileName] = resultData
            if algorithm != 'speedy':
                if resultData["node expanded"] != resultDict["speedy"][fileName]["node expanded"]:
                    print("find a not match instance with speedy:", algorithm, fileName)
                    diffVal = abs(int(resultData["node expanded"])-\
                                  int(resultDict["speedy"][fileName]["node expanded"]))
                    if diffVal > maxDiff:
                        maxDiff = diffVal
                        maxDiffInst = fileName
    print("max diff instant: ", maxDiffInst, maxDiff)

if __name__ == '__main__':
    main()
