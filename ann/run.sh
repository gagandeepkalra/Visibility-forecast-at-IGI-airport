#!/bin/bash

# --header
# if supplied, prints the csv header

# =1 prints csv header, =0 disables printing the header

set -e

enableHeader=1
normalizeData=1

outputDir="../Input-Output/ann"
outFilename="output.csv"
minmaxFile="minmaxFile"

#artificial neural network file
nnet="mlp.nnet"

set -e

rm -Rf "$outputDir/"
mkdir -p "$outputDir"
rm -f "$minmaxFile"

if [ ! -e ./a.out ] ;then
    echo "bash: ./a.out not found. run ./build.sh first"
    exit 1    
elif [ ! -e "../sqlDetails.config" ] ;then
    echo "bash: ../sqlDetails.config not found"
    exit 1
fi

source ../sqlDetails.config

flag="--sqlusername=$sqlusername --sqlpassword=$sqlpassword --sqldbname=$sqldbname"

if [ $enableHeader -eq 1 ] ;then
    flag="$flag --header"
fi

./a.out $flag > "$outputDir/$outFilename"

if [ $normalizeData -eq 1 ] ;then
    echo $(python3 normalizeCSV.py --normalize="$outputDir/$outFilename" --enableHeader=$enableHeader --minmaxFile=$minmaxFile) > "$outputDir/$outFilename"
fi

#get output from ann
temp=$RANDOM
cd ./bin &&
java -cp :../jars/* neuroph.Ann ../mlp.nnet ../../Input-Output/ann/output.csv 1 > ../$temp &&
cd .. &&
mv $temp "$outputDir/$outFilename" ||
    (cd .. &&
    rm $temp &&
    echo "bash: error in building/running java program")


#denormalize if it was normalized
if [ $normalizeData -eq 1 ] ;then
    echo $(python3 denormalizeCSV.py --denormalize="$outputDir/$outFilename" --enableHeader=$enableHeader --minmaxFile=$minmaxFile) > "$outputDir/$outFilename"
fi

#build java project
ant -q -S build 1>/dev/null 2>/dev/null