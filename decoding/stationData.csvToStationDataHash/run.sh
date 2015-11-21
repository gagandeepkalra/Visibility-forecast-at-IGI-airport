#!/bin/bash

# -u,   --sqlusername
#           username with which to log in sql server. note: if -DSQL was not used during compilation of the program, it will give error
#
# -p,   --sqlpassword
#          password of username provided. note: if -DSQL was not used during compilation of the program, it will give error
#
# -p,   --sqldbname
#           database name in which relevant data is stored. note: if -DSQL was not used during compilation of the program, it will give error
#
# -t    --truncate
#           truncate the stationData sql table before insertion of new data. note: if -DSQL was not used during compilation of the program, it will give error
#
# -o    --toDB
#           push the station data to database too


# check for existence of sqlDetails.config file

enableSQL=0

if [ ! -e "./a.out" ] ;then
    echo "./a.out not found. first run ./build.sh";
    exit 1
fi

if [ $enableSQL -eq 1 ] ;then
    if [ ! -e "../sqlDetails.config" ] ;then
        echo "sqlDetails.config not found"
        exit 1
    fi
    
    #read config
    source ../sqlDetails.config
    ./a.out --truncate --toDB --sqlusername=$sqlusername --sqlpassword=$sqlpassword --sqldbname=$sqldbname
else
    ./a.out
fi
