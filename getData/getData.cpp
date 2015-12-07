#include <soci.h>
#include <mysql/soci-mysql.h>
#include <getopt.h>
#include "../sql/sqlSession.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string sqlusername, sqlpassword, sqldbname;
    int enableHeader = false;

    while (1) {
        static struct option long_options[] = {
            {"sqlusername", required_argument, 0, 'u'},
            {"sqlpassword", required_argument, 0, 'p'},
            {"sqldbname", required_argument, 0, 'b'},
            {"header", no_argument, &enableHeader, 1},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        char c;

        c = getopt_long(argc, argv, "u:p:b:h", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch(c) {
        case 'u':
            sqlusername.assign(optarg);
            break;

        case 'b':
            sqldbname.assign(optarg);
            break;

        case 'p':
            sqlpassword.assign(optarg);
            break;
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

    try {
        soci::session *sql= SQLSession::getSession(&sqldbname, &sqlusername, &sqlpassword);

        soci::rowset<soci::row> rs = ((*sql).prepare <<
                                      "SELECT "
                                      "c.station_code, "
                                      "c.date, "
                                      "c.reading_no, "
                                      "c.`temperature_(C)`, "
                                      "c.cloud_cover, "
                                      "c.wind_direction, "
                                      "c.surface_wind_speed, "
                                      "c.`sea_level_pressure_(mb)` - p.`sea_level_pressure_(mb)` AS dmslp, "
                                      "c.`dewpoint_(C)`, "
                                      "c.`visibility_(km)` "
                                      "FROM "
                                      "data as c "
                                      "INNER JOIN ("
                                      "SELECT "
                                      "`station_code`, "
                                      "date, "
                                      "reading_no, "
                                      "`sea_level_pressure_(mb)` "
                                      "FROM "
                                      "`data`"
                                      ") as p "
                                      "WHERE "
                                      "c.`station_code` = p.`station_code` "
                                      "AND c.date = DATE_ADD(p.date, INTERVAL 1 DAY) "
                                      "AND c.`reading_no` = p.`reading_no`AND "
                                      "c.`temperature_(C)` <> -99 AND "
                                      "c.cloud_cover <> -99 AND "
                                      "c.wind_direction <> -99 AND "
                                      "c.surface_wind_speed <> -99 AND "
                                      "c.`sea_level_pressure_(mb)` <> -99 AND "
                                      "c.`dewpoint_(C)` <> -99 AND "
                                      "c.`visibility_(km)` <> -99 ");

        bool firstTime = true;

        for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
            soci::row const &row = *it;

            const int rowSize = row.size();

            for(std::size_t i = 0; i != rowSize; ++i) {
                const soci::column_properties &props = row.get_properties(i);

                if(firstTime) {
                    if(enableHeader) {
                        for(std::size_t j = 0; j != rowSize; ++j) {
                            std::cout<<'"';
                            std::cout<<row.get_properties(j).get_name();
                            std::cout<<'"';

                            if(j+1 != rowSize)
                                std::cout<<',';
                        }

                        std::cout<<'\n';
                    }

                    firstTime = false;
                }

                switch(props.get_data_type()) {
                case soci::dt_string:
                    std::cout << row.get<std::string>(i);
                    break;

                case soci::dt_double:
                    std::cout << row.get<double>(i);
                    break;

                case soci::dt_integer:
                    std::cout << row.get<int>(i);
                    break;

                case soci::dt_long_long:
                    std::cout << row.get<long long>(i);
                    break;

                case soci::dt_unsigned_long_long:
                    std::cout << row.get<unsigned long long>(i);
                    break;

                case soci::dt_date:
                    std::tm when = row.get<std::tm>(i);
                    std::cout << when.tm_mday<<'/'<<when.tm_mon+1<<'/'<<when.tm_year + 1900;
                    break;
                }

                if(i+1 != rowSize)
                    std::cout<<',';
            }

            std::cout <<'\n';
        }
    } catch (std::exception &e) {
        std::cerr << "sql error: "<<e.what()<<'\n';
    }

    return 0;
}