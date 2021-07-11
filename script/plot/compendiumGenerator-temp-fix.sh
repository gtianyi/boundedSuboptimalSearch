#!/bin/bash
#author TianyiGu
#date 2021/2/10
#desc bounded suboptimal plot compendimum auto generateor 

TIMESTAMP="20210628-121010"
plotter="boundedSuboptimalPlot.py -ot ${TIMESTAMP}"

pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -b 1.2 -e 2 -os tight -r ees)
echo "$pythonOut"
pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -b 2 -os loose -r ees)
echo "$pythonOut"
pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -t cpu -b 1.2 -e 2 -os tight -r ees)
echo "$pythonOut"
pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -t cpu -b 2 -os loose -r ees)
echo "$pythonOut"
pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -t coverageplt -b 1)
echo "$pythonOut"
pythonOut=$(python ${plotter} -d pancake -s regular -z 50 -ht gapm2 -t par10 -b 1)
echo "$pythonOut"


cp /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/bsscodebase/script/plot/BSS-tex/* /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/

mv /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/main.tex /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}.tex

cd /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}
latex /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}
bibtex /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}
latex /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}
latex /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}
dvips /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}.dvi
ps2pdf /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/bss-compendium-${TIMESTAMP}.ps

mkdir -p /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/cpu
cp /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/tile/*tight*cpu* /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/cpu/
#cp /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/pancake/*tight*cpu* /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/cpu/
#cp /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/racetrack/*tight*cpu* /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/cpu/
#cp /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/vacuumworld/*tight*cpu* /home/aifs1/gu/phd/research/workingPaper/boundedSuboptimalSearch/plots/${TIMESTAMP}/cpu/
