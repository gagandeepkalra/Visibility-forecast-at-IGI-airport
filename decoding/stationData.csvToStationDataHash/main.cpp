#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <getopt.h>

#include "../stationData.h"

#ifdef SQL
#include "../../sql/sqlSession.h"
#endif

int main(int argc, char *argv[])
{
#ifdef SQL
    std::string sqlusername, sqlpassword, sqldbname;
    int truncate_flag = false;
    int toDB_flag = false;
    int c;

    while (1) {
        static struct option long_options[] = {
            {"truncate", no_argument, &truncate_flag, 1},
            {"toDB", no_argument, &toDB_flag, 1},
            {"sqlusername", required_argument, 0, 'u'},
            {"sqlpassword", required_argument, 0, 'p'},
            {"sqldbname", required_argument, 0, 'b'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "u:b:p:ot",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 0: //truncate
        case 't':
            break;

        case 1: //toDB
        case 'o':
            break;

        case 'u':
            sqlusername.assign(optarg);
            break;

        case 'b':
            sqldbname.assign(optarg);
            break;

        case 'p':
            sqlpassword.assign(optarg);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            abort();
            break;

        default:
            abort();
        }
    }

    if(sqlusername.empty()) {
        std::cerr << "sql username not provided\n";
        abort();
    }

    if(sqlpassword.empty()) {
        std::cerr <<"sql password not provided\n";
        abort();
    }

    if(sqldbname.empty()) {
        std::cerr <<"sql dbname not provided\n";
        abort();
    }

    if(toDB_flag == false && truncate_flag == true) {
        std::cerr <<"cannot truncate unless data has to be pushed to database too\nLooks like you forgot to add --toDB flag";
        abort();
    }

    soci::session *sql = SQLSession::getSession(&sqldbname, &sqlusername, &sqlpassword);

#endif

    std::ofstream stationDataHash("stationDataHash");

    if(stationDataHash.is_open() == false) {
        std::cerr<<"could not open output file: stationDataHash\n";
        abort();
    }

    std::ifstream input("stationData.csv");

    if(input.is_open() == false) {
        std::cerr<<"could not open output file: stationData.csv\n";
        exit(EXIT_FAILURE);
    }

    std::vector<stationData> dataVec;
    std::string buffer;
    std::vector<std::string> list;

    std::getline(input, buffer);

    while( input.good()) {
        std::getline(input, buffer);
        list = split(buffer, ';', false);

        stationData d;

        for(int i = 0; i<list.size(); ++i) {
            if(list[i][0] == '\"') {
                list[i].erase(list[0].begin());
                list[i].erase(--list[0].end());
            }

            switch(i) {
            case 0:
                d.station_code = list[0];
                break;

            case 1:
                d.station_code = d.station_code + list[1];
                break;

            case 2:
                d.icao_location_indicator = list[2];
                break;

            case 3:
                d.place_name = list[3];
                break;

            case 4:
                d.state = list[4];
                break;

            case 5:
                d.country_name = list[5];
                break;

            case 6:
                d.wmo_region = list[6];
                break;

            case 7:
                d.lat = list[7];
                std::replace(d.lat.begin(), d.lat.end(), '-', '.');
                break;

            case 8:
                d.lon = list[8];
                std::replace(d.lon.begin(), d.lon.end(), '-', '.');
                break;

            case 9:
                d.upper_air_latitude = list[9];
                break;

            case 10:
                d.upper_air_longitude = list[10];
                break;

            case 11:
                d.station_elevation = list[11];
                break;

            case 12:
                d.upper_air_elevation = list[12];
                break;

            case 13:
                d.RBSN_indicator = list[13];
                break;
            }//switch
        }//for

        if(list.size())
            dataVec.push_back(d);
    }

    sort(dataVec.begin(), dataVec.end());

    stationDataHash<<"station_code; icao_location_indicator; place_name; state; country_name; wmo_region; lat; lon; upper_air_latitude; upper_air_longitude; station_elevation; upper_air_elevation; RBSN_indicator\n";

#ifdef SQL

    if(toDB_flag)
        if(truncate_flag) {
            try {
                *sql << "TRUNCATE `station_code`";
                std::cout << "station_code table truncated\n";
            } catch(std::exception &e) {
                std::cerr << e.what() << '\n';
            }
        }

#endif

    for(std::vector<stationData>::iterator it = dataVec.begin(); it != dataVec.end(); ++it) {
        stationDataHash<<it->station_code<<';'<<it->icao_location_indicator<<';'<<it->place_name<<';'<<it->state<<';'<<it->country_name<<';'<<it->wmo_region<<';'<<it->lat<<';'<<it->lon<<';'<<it->upper_air_latitude<<';'<<it->upper_air_longitude<<';'<<it->station_elevation<<';'<<it->upper_air_elevation<<';'<<it->RBSN_indicator<<'\n';


#ifdef SQL

        if(toDB_flag)
            try {
                *sql << "INSERT INTO `station_code` (station_code, icao_location_indicator, place_name, state, country_name, wmo_region, lat, lon, upper_air_latitude, upper_air_longitude, station_elevation, upper_air_elevation, RBSN_indicator) VALUES (:station_code, :icao_location_indicator, :place_name, :state, :country_name, :wmo_region, :lat, :lon, :upper_air_latitude, :upper_air_longitude, :station_elevation, :upper_air_elevation, :RBSN_indicator)",
                     soci::use(it->station_code), soci::use(it->icao_location_indicator), soci::use(it->place_name), soci::use(it->state), soci::use(it->country_name), soci::use(it->wmo_region), soci::use(it->lat), soci::use(it->lon), soci::use(it->upper_air_latitude), soci::use(it->upper_air_longitude), soci::use(it->station_elevation), soci::use(it->upper_air_elevation), soci::use(it->RBSN_indicator);
            } catch(std::exception &e) {
                std::cerr << e.what() << '\n';
            }

#endif
    }

    stationDataHash.close();
    return EXIT_SUCCESS;
}
