#!/usr/bin/env python
'''
python3 script
plotting code for generate quick data visulization of err message

Author: Tianyi Gu
Date: 02/14/2021
'''

__author__ = 'TianyiGu'

import argparse
import json
import os
# import sys
from datetime import datetime
# import re
# import math
# from scipy.stats import gmean

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
# from pandas.plotting import table
# import numpy as np


def parseArugments():

    parser = argparse.ArgumentParser(description='errPlotter')

    parser.add_argument(
        '-t',
        action='store',
        dest='plotType',
        help='plot type: var',
        default='var')

    return parser


def makeTwoLinePlot(dataframe, outputName):
    sns.set(rc={
        'figure.figsize': (13, 10),
        'font.size': 27,
        'text.color': 'black',
    })
    plt.rcParams["font.family"] = 'serif'
    plt.rcParams["font.serif"] = ['Times New Roman']

    ax1 = sns.lineplot(x="fhatminCounter",
                       y="fhatminVar",
                       data=dataframe,
                       color="red"
                       )

    ax2 = ax1.twinx()
    sns.lineplot(x="fhatminCounter",
                 y="fhatmin",
                 data=dataframe,
                 color="blue",
                 ax=ax2
                 )

    # ax.tick_params(colors='black', labelsize=24)
    ax1.set_ylabel("Variance of f-hat-min", fontsize=16, color="red")
    ax2.set_ylabel("f-hat-min", fontsize=16, color="blue")

    # if showSolvedInstance:
    # ax.legend().texts[0].set_text(
    # 'Solved:' + str(len(dataframe['instance'].unique())))
    # if useLogScale:
    # ax.set_yscale("log")

    # fontSize = 36
    # ax.set_title(title, fontdict={'fontsize': fontSize})

    # plt.ylabel('')
    # plt.xlabel('')
    # plt.xlable("# of f-hat-min samples")
    # ax1.set_ylable("Variance of f-hat-min")
    # ax2.set_ylable("f-hat-min")

    plt.savefig(outputName, bbox_inches="tight", pad_inches=0)
    plt.close()
    plt.clf()
    plt.cla()

def readData():

    fhatmin = []
    fhatminCounter = []
    fhatminVar = []

    print("reading in data...")

    inFile = "../../../../build_debug/tianyi.txt"
    print("reading ", inFile)
    with open(inFile) as f:
        for json_data in f:
            resultData = json.loads(json_data)
            fhatmin.append(resultData["new fhatmin"])
            fhatminCounter.append(resultData["fhatmin counter"])
            fhatminVar.append(resultData["fhatmin var"])

    rawdf = pd.DataFrame({
        "fhatmin": fhatmin,
        "fhatminCounter": fhatminCounter,
        "fhatminVar": fhatminVar,
    })

    # print rawdf
    return rawdf


def createOutFilePrefix(args):

    nowstr = datetime.now().strftime("%d%m%Y-%H%M%S")

    outDirectory = "../../../../plots/errPlots/"

    if not os.path.exists(outDirectory):
        os.makedirs(outDirectory, exist_ok=True)

    outFilePrefix = outDirectory + '/' + args.plotType + "-" + nowstr

    return outFilePrefix


def main():
    parser = parseArugments()
    args = parser.parse_args()
    print(args)

    df = readData()

    makeTwoLinePlot(df, createOutFilePrefix(args) + ".jpg")


if __name__ == '__main__':
    main()
