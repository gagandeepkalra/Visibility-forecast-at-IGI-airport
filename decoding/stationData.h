#ifndef STATION_DATA
#define STATION_DATA

//structure to hold the weather station data

#include <sstream>
#include <vector>

std::vector<std::string> split (const std::string &s, char delim, bool skipEmptyParts = true);

struct stationData {
    std::string station_code, icao_location_indicator, place_name, state, country_name, wmo_region, lat, lon, upper_air_latitude, upper_air_longitude, station_elevation, upper_air_elevation, RBSN_indicator;

    bool operator < (const stationData &b) const;

    friend std::ofstream &operator<<(std::ofstream &out, const stationData &obj);

    friend void operator>>(std::istringstream &stream, stationData &obj);

    friend std::ofstream &operator<<(std::ofstream &out, const std::vector<stationData> &dataVector);

    friend std::ifstream &operator>>(std::ifstream &in, std::vector<stationData> &dataVector);
};

#endif // STATION_DATA
