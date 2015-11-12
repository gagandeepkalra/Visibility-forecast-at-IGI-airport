#!/bin/bash
#: Title        : concatenate files
#: Date         : 2015-11-07
#: Author       : Diwakar
#: Version      : 0.1
#: Description  : concatenate multiple files into one file
#: Options      : None

rm -Rf ./Input-Output/AfterCat && mkdir -p ./Input-Output/AfterCat #create dir that will store final catOutput
dirs=$(ls "./Input-Output/IMD_input")

for date in $dirs ;do
    i=0
    mkdir -p "./Input-Output/AfterCat/$date" #create corresponding date folder to store that days entries
    while [ $i -le 47 ] ;do
        printf -v ctr "%02d" $i
        fileCount=$(ls "./Input-Output/IMD_input/$date/ASC/ASC$ctr" | wc -l)
        if [ $fileCount -ne 0 ] ;then   #check added to avoid error msg from cat if dir is empty
            cat "./Input-Output/IMD_input/$date/ASC/ASC$ctr/"* > "./Input-Output/AfterCat/$date/DEMS$ctr"
        else
            #echo "$ctr is empty"
            touch "./Input-Output/AfterCat/$date/DEMS$ctr"
        fi
        i=$((i+1))
    done
done
