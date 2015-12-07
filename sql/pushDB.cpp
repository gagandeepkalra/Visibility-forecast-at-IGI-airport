#include <iostream>
#include <istream>
#include <ostream>
#include <unordered_map>
#include "sqlSession.h"
#include "../decoding/parameters.h"
double getValDouble(const std::vector<std::string> &refHash, const int fieldPos, const int defaultValue = -99)
{
    if(refHash[fieldPos].empty() )
        return defaultValue;

    try {
        return std::stod(refHash[fieldPos]);
    } catch(std::exception &e) {
        std::cerr << "Sql error: error converting string to double. ["<<fieldPos<<"] = "<<refHash[fieldPos]<<'\n';
        return defaultValue;
    }
}

void pushDB(soci::session *const sql, const std::vector<std::string> &hash, const int readingNo)
{
    if(sql == NULL) {
        std::cerr << "sql error: in function pushDB, sql session is null \n";
        return;
    }

    try {
        //only some of the fields have been used, for now
        int stncode = getValDouble(hash, STATION_CODE);
        int day = getValDouble(hash, DAY);
        int month = getValDouble(hash, MONTH);
        int year = getValDouble(hash, YEAR);
        std::string date = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
        int nearest_hour = getValDouble(hash, NEAREST_HOUR);
        double visibility = getValDouble(hash, VISIBILITY_KM);
        int wind_direction = getValDouble(hash, WIND_DIRECTION);
        int surface_wind_speed = getValDouble(hash, SURFACE_WIND_SPEED);
        int wind_speed_indicator = getValDouble(hash, WIND_INDICATOR);
        int temperature = getValDouble(hash, TEMPERATURE_C);
        int dewpoint = getValDouble(hash, DEWPOINT_C);
        int cloudCover = getValDouble(hash, CLOUD_COVER);
        int station_pressure = getValDouble(hash, STATION_PRESSURE_HPA);
        //add a -99.9 function if value does not exists
        int sea_level_pressure = getValDouble(hash, SEA_LEVEL_PRESSURE_MB);
        int liquid_precipitation_amount_Sec_1 = getValDouble(hash, LIQUID_PRECIPITATION_AMOUNT_SEC_1_MM);
        int liquid_precipitation_amount_Sec_2 = getValDouble(hash, LIQUID_PRECIPITATION_AMOUNT_SEC_2MM);
        int precipitation_duration_Sec_2 = getValDouble(hash, PRECIPITATION_DURATION_SEC_2_HR);

        // *INDENT-OFF*
        (*sql)<<"INSERT INTO `data` ("
                    "`station_code`, `date`, `reading_no`, "
                    "`nearest_hour`, `visibility_(km)`, "
                    "`wind_direction`, `surface_wind_speed`, "
                    "`wind_speed_indicator`, `temperature_(C)`, "
                    "`dewpoint_(C)`, `cloud_cover`, `station_pressure_(mb)`, "
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
                        ":cloudCover, "
                        ":station_pressure, "
                        ":sea_level_pressure, "
                        ":liquid_precipitation_amount_Sec_1, "
                        ":liquid_precipitation_amount_Sec_2, "
                        ":precipitation_duration_Sec_2"
                    ")",
              soci::use(stncode), soci::use(date), soci::use(readingNo), soci::use(nearest_hour), soci::use(visibility), soci::use(wind_direction), soci::use(surface_wind_speed), soci::use(wind_speed_indicator), soci::use(temperature), soci::use(dewpoint), soci::use(cloudCover), soci::use(station_pressure), soci::use(sea_level_pressure), soci::use(liquid_precipitation_amount_Sec_1), soci::use(liquid_precipitation_amount_Sec_2), soci::use(precipitation_duration_Sec_2);
        // *INDENT-OFF*

    } catch (std::exception const &e) {
        std::cerr << "Sql error: " << e.what() << '\n';
    }
}
