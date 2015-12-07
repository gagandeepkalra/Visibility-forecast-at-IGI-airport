#!/bin/bash

# add  following line to compilation command to enable sql support
# -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs` 

enableSQL=1

if [ ! -e "../stationData.cpp" ] ;then
    echo "bash: stationData.cpp not present";
    exit 1;
elif [ ! -e "../../sql/sqlSession.cpp" ] ;then
    echo "bash: lex.y not present";
    exit 1;
fi

if [ $enableSQL -eq 1 ] ;then
    g++ main.cpp ../stationData.cpp ../../sql/sqlSession.cpp -std=c++11 -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs` 
else
    g++ main.cpp ../stationData.cpp -std=c++11
fi
