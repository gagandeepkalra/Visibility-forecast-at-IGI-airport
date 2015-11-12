#!/bin/bash

#: Title        : concatenate files
#: Date         : 2015-11-12
#: Author       : Diwakar
#: Version      : 0.1
#: Description  : concatenate multiple csv files into one file
#: Options      : None

outputDir="../Input-Output/catCSV"
initialInputDir="../Input-Output/CSV"
outFileName="DEMS.csv"

rm -Rf "$outputDir" && mkdir -p "$outputDir"

subdirs=$(ls "$initialInputDir")

for currDate in $subdirs ;do

    mkdir -p "$outputDir/$currDate"
    fileList=$(ls -v $initialInputDir/$currDate)
    for x in $fileList ;do
        filename="$initialInputDir/$currDate/$x"

        lines=$(wc -l < $filename)
        if [ $lines ] ;then
            tail -n $((lines-1)) < $filename >> "$outputDir/$currDate/$outFileName"
        fi
        i=$((i+1))
    done
    
    tmpFile=$(date +%d%m%Y)
    
    #sort according to station code
    cat "$outputDir/$currDate/$outFileName" | tr "," " " | sort -n -k1 | sed s/"\" \""/"\",\""/g >> "$outputDir/$currDate/$tmpFile"
    mv "$outputDir/$currDate/$tmpFile" "$outputDir/$currDate/$outFileName"
done
