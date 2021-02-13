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
                "ees": "EES",
                # "ptshhat": r"$\widehat{\mathrm{PTS}}$",
                "wastar": "WA*",
            }
        )

        self.algorithmPalette = {
            "EES":"royalblue",
            # r"$\widehat{\mathrm{PTS}}$": "orangered",
            "WA*": "limegreen",
            # "BEES-LBUG": "maroon",
            # "BEES - EpsLocal": "deepskyblue",
            # "XES": "magenta",
            # "XES-LBUG": "maroon",
            # "XES-cp05": "maroon",
            # "XES-sp100": "maroon",
            # "XES-c05s100": "maroon",
            # "BEES95": "tan",
            # "BEES95-LBUG": "darkgreen",
            # "BEES95-cp05": "darkgreen",
            # "BEES95-sp100": "darkgreen",
            # "BEES95-c05s100": "darkgreen",
            # "XES-bf": "darkgreen",
            # "XES-OV": "maroon",
            # "PTS-OV": "deepskyblue",
            # "BEES95-OV": "gold",
            # "XES-OV-SI": "grey",
            # "XES-OV-SI-LBUG": "yellowgreen",
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
                              "racetrack": "25", "vacuumworld": "60"}

        self.domainBoundsConfig = {
                                   "avaiableBoundPercent": {
                                       "tile":{
                                           "uniform":[1.1,1.2,1.5,2.0,5.0,10],
                                           "heavy":[1.1,1.2,1.5,2.0,5.0,10],
                                           "inverse":[2,3,6,10,20,40],
                                       },
                                    "vacuumworld": [],
                                    "pancake": [],
                                    "racetrack": [],
                                    },
                                   }

    def getAlgorithms(self, removeAlgorithm):
        if removeAlgorithm != "NA" and (removeAlgorithm in self.algorithms):
            self.algorithms.pop(removeAlgorithm)
        return self.algorithms

    def getShowname(self):
        return self.showname

    def getTotalInstance(self):
        return self.totalInstance

    def getDomainBoundsConfig(self):
        return self.domainBoundsConfig

    def getAlgorithmColor(self):
        return self.algorithmPalette
