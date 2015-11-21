#!/bin/bash

#: Title        : concatenate files
#: Date         : 2015-11-12
#: Author       : Diwakar
#: Version      : 0.2
#: Description  : concatenate multiple csv files into one file
#: Options      : None

#catAllCSV=1 enables cat of entire csv files of all dates generated as output into single csv file, catAllCSV=0 disables it
catAllCSV=1

# =1 enables collecting cat csv in separate file for each station
separateFilePerStation=1

#enable sorting according to station code
sort=1

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
    if [ $sort -eq 1 -a $catAllCSV -eq 0 ] ;then
        cat "$outputDir/$currDate/$outFileName" | tr "," " " | sort -n -k1 | sed s/"\" \""/"\",\""/g >> "$outputDir/$currDate/$tmpFile"
        mv "$outputDir/$currDate/$tmpFile" "$outputDir/$currDate/$outFileName"
    fi
done

if [ $catAllCSV -eq 1 ] ;then
    catCSVsubdirs=$( ls "$outputDir" )
    for currDate in $catCSVsubdirs ; do
        cat $outputDir/$currDate/$outFileName >> $outputDir/$outFileName
        rm -R $outputDir/$currDate
    done
fi

if [ $sort -eq 1 -a $catAllCSV -eq 1 -a $separateFilePerStation -eq 0 ] ;then
    tmpFile=$(date +%d%m%Y)
    cat "$outputDir/$outFileName" | tr "," " " | sort -n -k1 | sed s/"\" \""/"\",\""/g >> "$outputDir/$tmpFile"
    mv "$outputDir/$tmpFile" "$outputDir/$outFileName"
fi

if [ $separateFilePerStation -eq 1 ] ;then
    while IFS='' read -r line || [[ -n "$line" ]]; do
        stnCode=$(echo "$line" | cut -d, -f1 | sed s/"\""/""/g )
        echo "$line" >> "$outputDir/$stnCode.csv"
    done < "$outputDir/$outFileName"
    
    rm "$outputDir/$outFileName"
fi
