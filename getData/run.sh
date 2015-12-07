#!/bin/bash

# --header
# if supplied, prints the csv header

# =1 prints csv header, =0 disables printing the header
enableHeader=1

outputDir="../Input-Output/getData"
outFilename="output.csv"

rm -Rf "$outputDir/"
mkdir -p "$outputDir"

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
