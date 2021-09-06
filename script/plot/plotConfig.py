#!/usr/bin/env python
'''
python3 script
plotting config code for generate bounded cost search related plots

Author: Tianyi Gu
Date: 01/12/2021
'''

from collections import OrderedDict


class Configure:
    def __init__(self):

        self.algorithms = OrderedDict(
            {
                #ees compendium
                # "ees": "EES",
                # "ees95": "EES95",
                # "eesdoylew": "EES++",
                # "ees95doylew": "EES95++",
                # "eesli": "REES",
                #regular compendium
                "ees": "EES",
                "dps": "DPS",
                "roundrobind1": "RR-d",
                "wastar": "WA*",
                "roundrobin1": "RR-DXES",
                # "roundrobin1-dhatvar": "RR-DXES-dhatvar",
                # "dxes-withdhatvar": "DXES-dhatvar",
                # "dxes": "DXES",
                # "dxes95-withdhatvar": "DXES95-dhatvar",
                # "dxes95": "DXES95",
                #backup algorithms
                # "dbees": "DBEES",
                # "speedy": r"Speedy-$\hat{d}$",
                # "dxes-0.8": "DXES-0.8",
                # "dxes-0.95": "DXES",
                # "dxes": "DXES",
                # "roundrobin8": "RR811",
                # "roundrobind8": "RRD811",
                # "roundrobind8": "RRDX11",
                # "dxes-xes-no-rev": "DXES-NRE",
                # "dxes-xes-rev": "DXES-RE",
                # "ptshhat": r"$\widehat{\mathrm{PTS}}$",
                # "wastar-slowheap": "WA*-slow",
            }
        )

        self.algorithmPalette = {

            #ees compendium
            "EES":"royalblue",
            "EES95":"orangered",
            "EES++": "maroon",
            "EES95++": "magenta",
            "REES": "darkgreen",
            #regular compendium
            "EES":"royalblue",
            "WA*": "limegreen",
            "DPS": "tan",
            "RR-DXES": "darkgreen",
            # "RR-DXES-dhatvar": "orangered",
            "RR-d": "maroon",
            #backup algorithms
            # "DBEES": "yellowgreen",
            # r"Speedy-$\hat{d}$": "tan",
            # "EES-slow":"orangered",
            # r"$\widehat{\mathrm{PTS}}$": "orangered",
            # "WA*-slow": "orangered",
            # "BEES-LBUG": "maroon",
            # "BEES - EpsLocal": "deepskyblue",
            # "DXES-0.8": "magenta",
            # "DXES": "maroon",
            # "DXES-NRE": "magenta",
            # "DXES-RE": "maroon",
            # "XES-LBUG": "maroon",
            # "XES-cp05": "maroon",
            # "XES-sp100": "maroon",
            # "XES-c05s100": "maroon",
            # "BEES95": "tan",
            # "RR811": "yellowgreen",
            # "RRD811": "maroon",
            # "RRDX11": "maroon",
            # "BEES95-cp05": "darkgreen",
            # "BEES95-sp100": "darkgreen",
            # "BEES95-c05s100": "darkgreen",
            # "XES-bf": "darkgreen",
            # "XES-OV": "maroon",
            # "PTS-OV": "deepskyblue",
            # "BEES95-OV": "gold",
            # "XES-OV-SI": "grey",
            # "PTS-OV-SI": "yellowgreen",
            # "BEES95-OV-SI": "mediumblue",
            # "BEES95-OV-SI-LBUG": "yellowgreen",
        }

        self.showname = {"nodeGen": "Total Nodes Generated",
                         "nodeExp": "Total Nodes expanded",
                         "nodeGenDiff": "Algorithm Node Generated /  baseline Node Generated",
                         "fixedbaseline":
                         "log10 (Algorithm Node Generated /  baseline Node Generated)",
                         "cpu": "Raw CPU Time",
                         "solved": "Number of Solved Instances (Total=totalInstance)",
                         "boundValues": "Suboptimality",
                         }

        self.totalInstance = {"tile": "100", "pancake": "100",
                              "racetrack": "100", "vacuumworld": "60"}
# "racetrack": "25"

        self.domainBoundsConfig = {
                                   "avaiableBoundPercent": {
                                       "tile":{
                                           "uniform":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "heavy":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "inverse":[
                                               # 2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,10,20,40,1000],
                                               2,2.5,3,4,7,20,40],
                                       },
                                       "pancake": {
                                           "regular":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "heavy":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "sumheavy":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                       },
                                       "vacuumworld": {
                                           "uniform":[
                                               1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                           "heavy-easy":[
                                               1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                       },
                                       "racetrack": {
                                           "barto-bigger":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "hansen-bigger":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                           "den520d":[
                                               # 1.1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,4.5,8,1000],
                                               1.1,1.2,1.6,2,3,4.5,8],
                                       },
                                    },
                                   }
        self.domains = {"tile":["uniform", "heavy"],
                        "pancake":["regular"]}

    def getAlgorithms(self, removeAlgorithm):
        if removeAlgorithm:
            for rmAlg in removeAlgorithm:
                if rmAlg in self.algorithms:
                    self.algorithms.pop(rmAlg)
        return self.algorithms

    def getShowname(self):
        return self.showname

    def getTotalInstance(self):
        return self.totalInstance

    def getDomainBoundsConfig(self):
        return self.domainBoundsConfig

    def getAlgorithmColor(self):
        return self.algorithmPalette

    def getDomains(self):
        return self.domains
