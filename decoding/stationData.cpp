#include "stationData.h"
#include <fstream>
#include <iostream>

std::vector<std::string> split (const std::string &s, char delim, bool skipEmptyParts)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim)) {
        if(skipEmptyParts) {
            if(item.length())
                elems.push_back(item);
        } else
            elems.push_back(item);
    }

    return elems;
}

bool stationData::operator < (const stationData &b) const
{
    return stoi(this->station_code) <= stoi(b.station_code) ? true : false;
}

void operator>>(std::istringstream &stream, stationData &obj)
{
    std::string buffer;
    std::getline(stream, buffer);
    std::vector<std::string> list = split(buffer, ';', false);

    for(int i = 0; i<list.size(); ++i)
        switch(i) {
        case 0:
            obj.station_code = list[i];
            break;

        case 1:
            obj.icao_location_indicator = list[i];
            break;

        case 2:
            obj.place_name = list[i];
            break;

        case 3:
            obj.state = list[i];
            break;

        case 4:
            obj.country_name = list[i];
            break;

        case 5:
            obj.wmo_region = list[i];
            break;

        case 6:
            obj.lat = list[i];
            break;

        case 7:
            obj.lon = list[i];
            break;

        case 8:
            obj.upper_air_latitude = list[i];
            break;

        case 9:
            obj.upper_air_longitude = list[i];
            break;

        case 10:
            obj.station_elevation = list[i];
            break;

        case 11:
            obj.upper_air_elevation = list[i];
            break;

        case 12:
            obj.RBSN_indicator = list[i];
            break;
        }
}

std::ifstream &operator>>(std::ifstream &in, std::vector<stationData> &dataVector)
{
    dataVector.clear();
    stationData d;
    std::string buffer;

    while(std::getline(in, buffer)) {
        if(buffer.empty())
            continue;

        std::istringstream tmp(buffer);
        tmp>>d;
        dataVector.push_back(d);
    }

    return in;
}
