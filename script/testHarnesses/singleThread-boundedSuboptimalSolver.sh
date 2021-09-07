#!/bin/bash
print_usage() {
    echo "./singleThread-boundedCostSolver.sh"
    echo "[-f instance]                    default: 1"
    echo "[-n # of instances to test]      default: 1"
    echo "[-d domain]                      "
    echo "support list,eg: -d tile -d pancake  available: tile, pancake, racetrack, vacuumworld"
    echo "[-st subdomain of tile]          default: uniform, heavy, inverse, heavy-easy, inverse-easy"
    echo "[-sp subdomain of pancake]       default: regular, heavy"
    echo "[-sv subdomain of vacuumworld]   default: uniform, heavy"
    echo "[-sr subdomain of racetrack]     default: barto-bigger, hansen-bigger"
    echo "[-t time limit]                  default: 1800 (seconds)"
    echo "[-m memory limit]                default: 7(GB)"
    echo "[-ht heuristic type]             available: racetrack: euclidean(default), dijkstra"
    echo "                                            pancake: gap, gapm1, gapm2"
    echo "[-ex algorithm name extension]   default: NA"
    echo "[-h help]"
    exit 1
}

if [ "$1" = "-h" ] || [ "$1" = "-help" ] || [ "$1" = "?" ]; then
    print_usage
fi

# Which instance to start testing on
first=1
# The number of instances to test on
n_of_i=1

domain=("tile" "pancake" "racetrack" "vacuumworld")
#domain=("racetrack")
subdomain=()
#subdomainTile=("uniform" "heavy" "inverse" "heavy-easy" "reverse-easy" "sqrt")
subdomainTile=("uniform" "heavy" "inverse")
#subdomainTile=("uniform")
subdomainPancake=("regular" "heavy")
#subdomainPancake=("sumheavy")
subdomainVacuumworld=("uniform" "heavy-easy")
#subdomainVacuumworld=("heavy-easy")
#subdomainVacuumworld=("heavy")
#subdomainRacetrack=("barto-bigger" "hansen-bigger")
subdomainRacetrack=("barto-bigger" "den520d" "ost003d")

heuristicTypes=("NA")
#heuristicTypePancake=("gap" "gapm2")
heuristicTypePancake=("gap" "gapm2")
#heuristicTypeRacetrack=("dijkstra" "euclidean")
heuristicTypeRacetrack=("dijkstra")

n_of_i_Tile=100
#n_of_i_Tile=10
#n_of_i_Tile=1
n_of_i_Pancake=100
#n_of_i_Pancake=1
n_of_i_Racetrack=25
n_of_i_Racetrack_hog=100
n_of_i_Vacuumworld=60

size="4"
sizeOfRegularPancake="50"
sizeOfHeavyPancake="10"
#sizeOfHeavyPancake="16"
sizeOfSumHeavyPancake="10"

#bssSolvers=("eesdoylew" "ees" "ees95" "wastar" "dxes" "dps" "roundrobin1" "roundrobin8" "roundrobind1" "roundrobind8")
#bssSolvers=("eesli" "speedy" "eesdoylew" "ees95doylew" "ees" "roundrobind1" "ees95" "wastar" "dxes" "dps" "roundrobin1")
#bssSolvers=("roundrobin1" "bfsonp" "dxes")
bssSolvers=("dpsroundrobin")
boundPercents=()
#boundPercentsA=(60 80 100 120 140 160 180 200 220 240 260 280 300 400 500 600 800 1000 1300 2000 3000)
#boundPercentsA=(1.1 1.2 1.5 2.0 5.0 10)
#boundPercentsA=(1.1 1.2 1.6 1.8 2 2.2 2.4 2.6 2.8 3 4.5 8 1000)
boundPercentsA=(1.1 1.2 1.6 2 3 4.5 8)
#boundPercentsA=(1.2)
#boundPercentsB=(2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 10 20 40 1000)
#boundPercentsB=(2)
boundPercentsB=(2 2.5 3 4 7 20 40)
timeLimit=600
memoryLimit=7

algorithmNameExtension="NA"

solverCleared=false
boundCleared=false
domainCleared=false
subdomainTileCleared=false
subdomainRacetrackCleared=false
subdomainVacuumworldCleared=false
subdomainPancakeCleared=false
#parse arguments
for ((i = 1; i <= "$#"; i++)); do
    if [ ${!i} == "-f" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            var=$((i + 1))
            first=${!var}
        fi
    fi

    if [ ${!i} == "-n" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            var=$((i + 1))
            n_of_i=${!var}
        fi
    fi

    if [ ${!i} == "-d" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $domainCleared; then
                unset domain
                domainCleared=true
            fi
            var=$((i + 1))
            domain+=(${!var})
        fi
    fi

    if [ ${!i} == "-st" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $subdomainTileCleared; then
                unset subdomainTile
                subdomainTileCleared=true
            fi
            var=$((i + 1))
            subdomainTile+=(${!var})
        fi
    fi

    if [ ${!i} == "-sp" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $subdomainPancakeCleared; then
                unset subdomainPancake
                subdomainPancakeCleared=true
            fi
            var=$((i + 1))
            subdomainPancake+=(${!var})
        fi
    fi

    if [ ${!i} == "-sv" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $subdomainVacuumworldCleared; then
                unset subdomainVacuumworld
                subdomainVacuumworldCleared=true
            fi
            var=$((i + 1))
            subdomainVacuumworld+=(${!var})
        fi
    fi

    if [ ${!i} == "-sr" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $subdomainRacetrackCleared; then
                unset subdomainRacetrack
                subdomainRacetrackCleared=true
            fi
            var=$((i + 1))
            subdomainRacetrack+=(${!var})
        fi
    fi

    #if [ ${!i} == "-z" ]; then
    #if [ $((i + 1)) -le "$#" ]; then
    #var=$((i + 1))
    #size=${!var}
    #fi
    #fi

    if [ ${!i} == "-ex" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            var=$((i + 1))
            algorithmNameExtension=${!var}
        fi
    fi

    #if [ ${!i} == "-ht" ]; then
    #if [ $((i + 1)) -le "$#" ]; then
    #var=$((i + 1))
    #heuristicType=${!var}
    #fi
    #fi

    if [ ${!i} == "-u" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            if ! $solverCleared; then
                unset bssSolvers
                solverCleared=true
            fi
            var=$((i + 1))
            bssSolvers+=(${!var})
        fi
    fi

    #if [ ${!i} == "-bp" ]; then
    #if [ $((i + 1)) -le "$#" ]; then
    #if ! $boundCleared; then
    #unset boundPercents
    #boundCleared=true
    #fi
    #var=$((i + 1))
    #boundPercents+=(${!var})
    #fi
    #fi

    if [ ${!i} == "-m" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            var=$((i + 1))
            memoryLimit=${!var}
        fi
    fi

    if [ ${!i} == "-t" ]; then
        if [ $((i + 1)) -le "$#" ]; then
            var=$((i + 1))
            timeLimit=${!var}
        fi
    fi

    if [ ${!i} == "-h" ]; then
        print_usage
    fi

done

echo "first ${first}"
echo "domain ${domain[*]}"
echo "solvers ${bssSolvers[*]}"
echo "time limit ${timeLimit}"
echo "memory limit ${memoryLimit}"

research_home="/home/aifs1/gu/phd/research/workingPaper"

hostname=$(cat /proc/sys/kernel/hostname)
sendSlackNotification.bash "#experiments" "experiment_bot" "Tianyi just started running experiments on ${hostname}; estimated time: 20 hours."
echo "sendSlackNotification.bash \"#experiments\" \"experiment_bot\" \"Tianyi just started running experiments on ${hostname}; estimated time: 20 hours.\""

for curDomainId in "${!domain[@]}"; do
    curDomain=${domain[$curDomainId]}
    echo "running $curDomain"

    if [ "$curDomain" == "tile" ]; then
        subdomain=("${subdomainTile[@]}")
        boundPercents=("${boundPercentsA[@]}")
        n_of_i=$n_of_i_Tile
    fi

    if [ "$curDomain" == "pancake" ]; then
        subdomain=("${subdomainPancake[@]}")
        boundPercents=("${boundPercentsA[@]}")
        heuristicTypes=("${heuristicTypePancake[@]}")
        n_of_i=$n_of_i_Pancake
    fi

    if [ "${curDomain}" == "vacuumworld" ]; then
        subdomain=("${subdomainVacuumworld[@]}")
        boundPercents=("${boundPercentsA[@]}")
        n_of_i=$n_of_i_Vacuumworld
    fi

    if [ "${curDomain}" == "racetrack" ]; then
        subdomain=("${subdomainRacetrack[@]}")
        boundPercents=("${boundPercentsA[@]}")
        heuristicTypes=("${heuristicTypeRacetrack[@]}")
        n_of_i=$n_of_i_Racetrack_hog
    fi

    echo "subdomain ${subdomain[*]}"
    echo "n_of_i ${n_of_i}"

    for curSubdomainId in "${!subdomain[@]}"; do
        curSubdomain=${subdomain[$curSubdomainId]}
        echo "running $curSubdomain"

        if [ "$curDomain" == "tile" ]; then
            if [ "$curSubdomain" == "inverse" ]; then
                boundPercents=("${boundPercentsB[@]}")
            fi
        fi

        echo "boundPercents ${boundPercents[*]}"

        if [ "$curDomain" == "pancake" ]; then
            if [ "$curSubdomain" == "regular" ]; then
                size=$sizeOfRegularPancake
            fi

            if [ "$curSubdomain" == "heavy" ]; then
                size=$sizeOfHeavyPancake
                heuristicTypes=("gap")
            fi

            if [ "$curSubdomain" == "sumheavy" ]; then
                size=$sizeOfSumHeavyPancake
                heuristicTypes=("gap")
            fi

            echo "size ${size}"
        fi

        if [ "$curDomain" == "racetrack" ]; then
            if [ "$curSubdomain" == "barto-bigger" ]; then
                n_of_i=$n_of_i_Racetrack
            fi
            echo "n_of_i ${n_of_i}"
        fi

        for heuristicTypeId in "${!heuristicTypes[@]}"; do
            heuristicType=${heuristicTypes[$heuristicTypeId]}

            echo "heuristicType ${heuristicType}"

            infile=""
            outfile=""

            infile_path="${research_home}/realtime-nancy/worlds/${curDomain}"

            outfile_path="${research_home}/boundedSuboptimalSearch/results/${curDomain}/${curSubdomain}/solverDir"

            infile_name=""

            limitWrapper="${research_home}/boundedSuboptimalSearch/bsscodebase/script/testHarnesses/limitWrapper.py"

            if [ "${curDomain}" == "tile" ]; then

                if [ "${curSubdomain}" == "heavy-easy" ]; then
                    infile_path="${research_home}/realtime-nancy/worlds/slidingTile_tianyi1000-easy-for-heavy"
                fi

                if [ "${curSubdomain}" == "inverse-easy" ]; then
                    infile_path="${research_home}/realtime-nancy/worlds/slidingTile_tianyi1000-easy-for-inverse"
                fi

                if [ "${curSubdomain}" == "reverse-easy" ]; then
                    infile_path="${research_home}/realtime-nancy/worlds/slidingTile_tianyi1000-easy-for-reverse"
                fi

                infile_name="instance-${size}x${size}.st"
                outfile="${outfile_path}/BoundNumber-size-${size}-instance.json"
                infile="${infile_path}/${infile_name}"
            fi

            if [ "${curDomain}" == "pancake" ]; then
                infile_name="instance-${size}.pan"
                outfile_path="${outfile_path/solverDir/$heuristicType}/solverDir"
                outfile="${outfile_path}/BoundNumber-size-${size}-instance.json"
                infile="${infile_path}/${size}/${infile_name}"
            fi

            if [ "${curDomain}" == "racetrack" ]; then
                infile_name="${curSubdomain}-instance.init"
                outfile_path="${outfile_path/solverDir/$heuristicType}/solverDir"
                outfile="${outfile_path}/BoundNumber-instance.json"
                infile="${infile_path}/${infile_name}"
            fi

            if [ "${curDomain}" == "vacuumworld" ]; then

                infile_path="${research_home}/realtime-nancy/worlds/vacuumworld/200x200"

                if [ "${curSubdomain}" == "heavy-easy" ]; then
                    infile_path="${research_home}/realtime-nancy/worlds/vacuumworld/200x200-6"
                fi

                infile_name="instance.vw"
                outfile="${outfile_path}/BoundPercent-BoundNumber-instance.json"
                infile="${infile_path}/${infile_name}"
            fi

            last=$(($first + $n_of_i))

            boundList=("${boundPercents[@]}")

            for solverId in "${!bssSolvers[@]}"; do

                solverName=${bssSolvers[$solverId]}
                echo $solverName

                solverNameInDir=$solverName
                if [ "$algorithmNameExtension" != "NA" ]; then
                    solverNameInDir="${solverName}-${algorithmNameExtension}"
                fi

                outfile_path_alg="${outfile_path/solverDir/$solverNameInDir}"
                mkdir -p ${outfile_path_alg}
                outfile_alg="${outfile/solverDir/$solverNameInDir}"

                executable="${research_home}/boundedSuboptimalSearch/build_release/bin/bss"

                for boundTypeValue in "${boundList[@]}"; do
                    echo "$bound $boundTypeValue"

                    instance=$first
                    while ((instance < last)); do
                        infile_instance="${infile/instance/$instance}"
                        infile_instance="${infile_instance/tile/slidingTile}"
                        outfile_instance="${outfile_alg/instance/$instance}"
                        outfile_instance="${outfile_instance/BoundNumber/$boundTypeValue}"
                        tempfile="${outfile_instance}.temp"

                        curFileName=${infile_name/instance/$instance}

                        weight=$boundTypeValue

                        if [ -f ${outfile_instance} ] || [ -f ${tempfile} ]; then

                            let instance++

                        else

                            realSubdomain="${curSubdomain}"
                            if [ "${curSubdomain}" == "heavy-easy" ]; then
                                realSubdomain="heavy"
                            fi

                            if [ "${curSubdomain}" == "inverse-easy" ]; then
                                realSubdomain="inverse"
                            fi

                            if [ "${curSubdomain}" == "reverse-easy" ]; then
                                realSubdomain="reverse"
                            fi

                            command="${executable} -d ${curDomain} -s ${realSubdomain} -a ${solverName} \
                            -w ${weight} -o ${outfile_instance} -i ${instance} -f ${heuristicType} "

                            command+="< ${infile_instance}"

                            echo "${command}" >${tempfile}

                            executableOut=$(python $limitWrapper -c "${command}" -t $timeLimit -m $memoryLimit)

                            echo "${executableOut}" >>${tempfile}

                            if [ -f ${outfile_instance} ]; then
                                rm ${tempfile}
                            fi

                            let instance++

                        fi

                    done
                done

                fixJson_running_flag="${research_home}/boundedSuboptimalSearch/results/fixJson.${curDomain}.${curSubdomain}.${solverNameInDir}"
                if [ "${curDomain}" == "pancake" ] || [ "${curDomain}" == "racetrack" ]; then
                    fixJson_running_flag="${fixJson_running_flag}.${heuristicType}"
                fi
                fixJsonExecutable="${research_home}/boundedSuboptimalSearch/bsscodebase/script/fixJson.py"

                sleep 1

                if [ ! -f ${fixJson_running_flag} ]; then
                    fixJsonOut=$(python ${fixJsonExecutable} -d ${curDomain} -s ${curSubdomain} -a ${solverNameInDir} -ht ${heuristicType})
                    #echo "$fixJsonOut"
                    echo "$fixJsonOut" >> ${fixJson_running_flag}
                fi
            done
        done
    done
done

sendSlackNotification.bash "#experiments" "experiment_bot" "<@tianyi> 's experiments on ${hostname} finished."
echo "sendSlackNotification.bash \"#experiments\" \"experiment_bot\" \"Tianyi's experiments on ${hostname} finished.\""
