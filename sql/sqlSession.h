#ifndef SQL_SESSION_H
#define SQL_SESSION_H

#include <soci.h>
#include <mysql/soci-mysql.h>

/*
 * singleton class. creates a sql session
 *
 * right now, it has a limitation: once a session is created with credentials, they cannot be changed
 */

class SQLSession {
    static soci::session *s;

    SQLSession(SQLSession &s);

    void operator=(const SQLSession &s);
public:

    static soci::session *getSession(std::string *sqldbname = NULL, std::string *sqlusername = NULL, std::string *sqlpassword = NULL);

    ~SQLSession();
};

#endif
