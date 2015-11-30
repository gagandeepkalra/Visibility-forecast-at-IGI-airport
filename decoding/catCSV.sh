#!/bin/bash

#: Title        : concatenate files
#: Date         : 2015-11-12
#: Author       : Diwakar
#: Version      : 0.2
#: Description  : concatenate multiple csv files into one file
#: Options      : None

#catAllCSV=1 enables cat of entire csv files of all dates generated as output into single csv file, catAllCSV=0 disables it
catAllCSV=1

# =1 enables collecting cat csv in separate file for each station. to separate, the first column of csv should be station code
separateFilePerStation=1

# =1 removes station code from csv file if separateFilePerStation=1 note:it is assumed that first column will be always of station code
removeStnCode=1

#enable sorting according to station code
sort=1

#enables field header in csv files
enableHeader=0

#normalize the columns x = (x - min(x) )/( max(x) - min(x))
normalize=1

outputDir="../Input-Output/catCSV"
initialInputDir="../Input-Output/CSV"
outFileName="DEMS.csv"

rm -Rf "$outputDir" && mkdir -p "$outputDir"

csvHeader=""

subdirs=$(ls "$initialInputDir")

tmpFile=$(date +%d%m%Y)
    
for currDate in $subdirs ;do

    mkdir -p "$outputDir/$currDate"
    fileList=$(ls -v $initialInputDir/$currDate)
    for x in $fileList ;do
        filename="$initialInputDir/$currDate/$x"

        lines=$(wc -l < $filename)
        if [ $lines -ne 0 ] ;then #file has some content
            if [ -z "$csvHeader" ] ;then #read csv header for first time only
                csvHeader=$(head -n 1 $filename)
            fi
        
            tail -n $((lines-1)) < $filename >> "$outputDir/$currDate/$outFileName"
        fi
        i=$((i+1))
    done
    
    if [ $sort -eq 1 -a $catAllCSV -eq 0 ] ;then
        #sort according to station code
        cat "$outputDir/$currDate/$outFileName" | tr "," " " | sort -n -k1 | sed s/"\" \""/"\",\""/g >> "$outputDir/$currDate/$tmpFile"
        
        if [ $enableHeader -eq 1 ] ;then
            echo "$csvHeader" > "$outputDir/$currDate/$outFileName"
            cat "$outputDir/$currDate/$tmpFile" >> "$outputDir/$currDate/$outFileName"
        else
            mv "$outputDir/$currDate/$tmpFile" "$outputDir/$currDate/$outFileName"
        fi
    fi
done

if [ $catAllCSV -eq 1 ] ;then
    catCSVsubdirs=$( ls "$outputDir" )
    for currDate in $catCSVsubdirs ; do
        if [ -e $outputDir/$currDate/$outFileName ] ;then #there exists cat file for currdate
            cat $outputDir/$currDate/$outFileName >> $outputDir/$outFileName
        fi
        rm -R $outputDir/$currDate
    done
    
    if [ $sort -eq 1 -a $separateFilePerStation -eq 0 ] ;then
        cat "$outputDir/$outFileName" | tr "," " " | sort -n -k1 | sed s/"\" \""/"\",\""/g >> "$outputDir/$tmpFile"
        mv "$outputDir/$tmpFile" "$outputDir/$outFileName"
    fi
    if [ $enableHeader -eq 1 -a $separateFilePerStation -eq 0 ] ;then
        echo -e "$csvHeader\n$(cat $outputDir/$outFileName)" > $outputDir/$outFileName
    fi
fi

if [ $separateFilePerStation -eq 1 -a $catAllCSV -eq 1 ] ;then
    while IFS='' read -r line || [[ -n "$line" ]]; do
        stnCode=$(echo "$line" | cut -d, -f1 | sed s/"\""/""/g )
        if [ $removeStnCode -eq 1 ] ;then
            line=$(echo "$line" | cut -d, -f2- | sed 's/\"//g') #removes quotes todo: remove later
        fi
        echo "$line" >> "$outputDir/$stnCode.csv"
        
    done < "$outputDir/$outFileName"
    
    rm "$outputDir/$outFileName"
    
    if [ $enableHeader -eq 1 ] ;then
        tempHeader=$(echo "$csvHeader" | cut -d, -f2-)
        files=$(ls $outputDir | grep ".csv")
        for x in $files ;do                             #remove rows having -99 (unknown values) todo:remove later
            echo -e "$tempHeader\n$(cat $outputDir/$x | sed '/-99/d' | uniq)" > $outputDir/$x
        done
    fi
    
    if [ $normalize -eq 1 ] ;then
        files=$(ls $outputDir | grep ".csv")
        for x in $files ;do
            echo $(echo "$outputDir/$x" | python3 normalizeCSV.py) > "$outputDir/$x"
        done
    fi
fi
