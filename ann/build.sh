#!/bin/bash

g++ getData.cpp ../sql/sqlSession.cpp -std=c++11 -I/usr/local/include/soci -lsoci_core -lsoci_mysql `mysql_config --cflags --libs`