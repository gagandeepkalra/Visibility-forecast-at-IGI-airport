#include <soci.h>
#include <mysql/soci-mysql.h>
#include <string>
#include <unordered_map>

/*
 * pushes decoded fields to database
 */

double getValDouble(const std::vector<std::string> &hash, const std::string key, const int defaultValue = -99);

void pushDB(soci::session * const sql, const std::vector<std::string> &hash, const int readingNo);