#include <iostream>
#include <istream>
#include <ostream>
#include <unordered_map>
#include "sqlSession.h"
#include "../decoding/parameters.h"
int getValInt(const std::vector<std::string> &refHash, const int fieldPos, const int defaultValue = -99)
{
    if(refHash[fieldPos].empty() )
        return defaultValue;

    try {
        return std::stoi(refHash[fieldPos]);
    } catch(std::exception &e) {
        std::cerr << "Sql error: error converting string to int. ["<<fieldPos<<"] = "<<refHash[fieldPos]<<'\n';
        return defaultValue;
    }
}

void pushDB(soci::session * const sql, const std::vector<std::string> &hash, const int readingNo)
{
    if(sql == NULL) {
        std::cerr << "sql error: in function pushDB, sql session is null \n";
        return;
    }

    try {
        //only some of the fields have been used, for now
        int stncode = getValInt(hash, STATION_CODE);
        int day = getValInt(hash, DAY);
        int month = getValInt(hash, MONTH);
        int year = getValInt(hash, YEAR);
        std::string date = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
        int nearest_hour = getValInt(hash, NEAREST_HOUR);
        int visibility = getValInt(hash, VISIBILITY_KM);
        int wind_direction = getValInt(hash, WIND_DIRECTION);
        int surface_wind_speed = getValInt(hash, SURFACE_WIND_SPEED);
        int wind_speed_indicator = getValInt(hash, WIND_INDICATOR);
        int temperature = getValInt(hash, TEMPERATURE_C);
        int dewpoint = getValInt(hash, DEWPOINT_C);
        int station_pressure = getValInt(hash, STATION_PRESSURE_HPA);
        //add a -99.9 function if value does not exists
        int sea_level_pressure = getValInt(hash, SEA_LEVEL_PRESSURE_MB);
        int liquid_precipitation_amount_Sec_1 = getValInt(hash, LIQUID_PRECIPITATION_AMOUNT_SEC_1_MM);
        int liquid_precipitation_amount_Sec_2 = getValInt(hash, LIQUID_PRECIPITATION_AMOUNT_SEC_2MM);
        int precipitation_duration_Sec_2 = getValInt(hash, PRECIPITATION_DURATION_SEC_2_HR);

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
