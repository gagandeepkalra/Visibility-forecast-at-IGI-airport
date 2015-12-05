#!/bin/bash

# add  following line to compilation command to enable sql support
# -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs` 

# debug = 0 disables debugging, debug = 1 enables debugging
debug=0

# enableSQL=0 disables sql support, enableSQL=1 enables sql support
enableSQL=1

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

if [ $enableSQL -eq 1 ] ;then
    if [ ! -e ../sql/pushDB.cpp ] ;then
        echo "bash: ../sql/pushDB.cpp not present in dir"; exit 1;
    elif [ ! -e ../sql/sqlSession.cpp ] ;then
        echo "bash: ./sql/sqlSession.cpp not present in dir"; exit 1;        
    fi
fi


if [ $debug -ne 0 ] ;then
    bison -d --graph --debug yacc.y && #ALSO UNCOMMENT yydebug = 1; IN yacc.y
    flex -d lex.l
    
    if [ $enableSQL -eq 1 ] ;then
        c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp ../sql/pushDB.cpp ../sql/sqlSession.cpp -std=c++11 -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs`
    else
        c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp -std=c++11
    fi
    
    dot -Tjpeg yacc.dot -o parser.jpeg &&
    sync &&
    rm lex.yy.c yacc.tab.c yacc.tab.h &&
    echo "debugging enabled. build complete" || echo "build failed"
else #debugging off
    bison -d yacc.y &&
    flex lex.l
    
    if [ $enableSQL -eq 1 ] ;then
        c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp ../sql/pushDB.cpp ../sql/sqlSession.cpp -std=c++11 -DSQL -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs`
    else
        c++ lex.yy.c yacc.tab.c decode.cpp stationData.cpp -std=c++11
    fi
    
    sync &&
    rm lex.yy.c yacc.tab.c yacc.tab.h || echo "build failed"
fi
