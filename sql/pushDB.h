#include <soci.h>
#include <mysql/soci-mysql.h>
#include <string>
#include <unordered_map>

/*
 * pushes decoded fields to database
 */

int getValInt(const std::unordered_map<std::string, std::string> &hash, const std::string key, const int defaultValue = -99);

void pushDB(soci::session *sql, std::unordered_map<std::string, std::string> &hash, int readingNo);