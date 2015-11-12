#!/bin/bash

# add  following line to compilation command to enable sql support
# -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs` 

# debug = 0 disables debugging
# debug = 1 enables debugging
debug=0

# add -DSQL flag to g++ to enable sql support

#tests
if [ ! -e yacc.y ] ;then
    echo "bash: yacc.y not present in dir"; exit 1;
elif [ ! -e lex.l ] ;then
    echo "bash: lex.y not present in dir"; exit 1;
elif [ ! -e decode.cpp ] ;then
    echo "bash: decode.cpp not present in dir"; exit 1;
elif [ ! -e stationData.cpp ] ;then
    echo "bash: station_data.cpp not present in dir"; exit 1;
fi


if [ $debug -ne 0 ] ;then
    bison -d --graph --debug yacc.y && #ALSO UNCOMMENT yydebug = 1; IN yacc.y
    flex -d lex.l &&
    
    c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp ../sql/pushDB.cpp ../sql/sqlSession.cpp -std=c++11 -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs`  &&
    dot -Tjpeg yacc.dot -o parser.jpeg &&
    sync &&
    rm lex.yy.c yacc.tab.c yacc.tab.h &&
    echo "debugging enabled. build complete" || echo "build failed"
else
    bison -d yacc.y &&
    flex lex.l &&
    
    c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp ../sql/pushDB.cpp ../sql/sqlSession.cpp -std=c++11 -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs`  &&
    sync &&
    rm lex.yy.c yacc.tab.c yacc.tab.h || echo "build failed"
fi