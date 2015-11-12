#include <iostream>
#include <istream>
#include <ostream>
#include <unordered_map>
#include "sqlSession.h"

int getValInt(const std::unordered_map<std::string, std::string> &hash, const std::string key, const int defaultValue = -99)
{
    auto it = hash.find(key);

    if(it == hash.end())
        return defaultValue;

    try {
        return std::stoi(it->second);
    } catch(std::exception &e) {
        std::cerr << "Sql error: error converting string to int. ["<<key<<"] = "<<it->second<<'\n';
        return defaultValue;
    }
}

void pushDB(soci::session *sql, std::unordered_map<std::string, std::string> &hash, int readingNo)
{
    if(sql == NULL) {
        std::cerr << "sql error: in function pushDB, sql session is null \n";
        return;
    }

    try {
        //only some of the fields have been used, for now
        int stncode = getValInt(hash, "station code");
        int day = getValInt(hash, "day");
        int month = getValInt(hash, "month");
        int year = getValInt(hash, "year");
        std::string date = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
        int nearest_hour = getValInt(hash, "nearest hour");
        int visibility = getValInt(hash, "Visibility (km)");
        int wind_direction = getValInt(hash, "wind direction");
        int surface_wind_speed = getValInt(hash, "surface wind speed");
        int wind_speed_indicator = getValInt(hash, "wind indicator dig");
        int temperature = getValInt(hash, "temperature (C)");
        int dewpoint = getValInt(hash, "dewpoint (C)");
        int station_pressure = getValInt(hash, "station pressure (hPa)");
        //add a -99.9 function if value does not exists
        int sea_level_pressure = getValInt(hash, "sea level pressure (hPa)");
        int liquid_precipitation_amount_Sec_1 = getValInt(hash, "liquid precipitation amount Sec 1(mm)");
        int liquid_precipitation_amount_Sec_2 = getValInt(hash, "liquid precipitation amount Sec 2 (mm)");
        int precipitation_duration_Sec_2 = getValInt(hash, "precipitation duration Sec 2 (hr)");

        // *INDENT-OFF*
        (*sql)<<"INSERT INTO `data` ("
                    "`station_code`, `date`, `reading_no`, "
                    "`nearest_hour`, `visibility_(km)`, "
                    "`wind_direction`, `surface_wind_speed`, "
                    "`wind_speed_indicator`, `temperature_(C)`, "
                    "`dewpoint_(C)`, `station_pressure_(mb)`, "
                    "`sea_level_pressure_(mb)`, `liquid_precipitation_amount_Sec_1(mm)`, "
                    "`liquid_precipitation_amount_Sec_2_(mm)`, "
                    "`precipitation_duration_Sec_2_(hr)`"
                    ") "
                "VALUES "
                    "("
                        ":station_code, "
                        "STR_TO_DATE(:date, '%d/%m/%Y'), "
                        ":readingNo, "
                        ":nearest_hour, "
                        ":visibility, "
                        ":wind_direction,"
                        ":surface_wind_speed, "
                        ":wind_speed_indicator, "
                        ":temperature, "
                        ":dewpoint, "
                        ":station_pressure, "
                        ":sea_level_pressure, "
                        ":liquid_precipitation_amount_Sec_1, "
                        ":liquid_precipitation_amount_Sec_2, "
                        ":precipitation_duration_Sec_2"
                    ")",
              soci::use(stncode), soci::use(date), soci::use(readingNo), soci::use(nearest_hour), soci::use(visibility), soci::use(wind_direction), soci::use(surface_wind_speed), soci::use(wind_speed_indicator), soci::use(temperature), soci::use(dewpoint), soci::use(station_pressure), soci::use(sea_level_pressure), soci::use(liquid_precipitation_amount_Sec_1), soci::use(liquid_precipitation_amount_Sec_2), soci::use(precipitation_duration_Sec_2);
        // *INDENT-OFF*

    } catch (std::exception const &e) {
        std::cerr << "Sql error: " << e.what() << '\n';
    }
}
