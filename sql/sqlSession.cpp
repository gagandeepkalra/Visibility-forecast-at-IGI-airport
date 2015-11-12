#include "sqlSession.h"
#include <string>
#include <iostream>

soci::session *SQLSession::getSession(std::string *sqldbname, std::string *sqlusername, std::string *sqlpassword)
{
    if (s == NULL) {
        try {
            if( ! sqlusername) {
                std::cerr << "error: in function getSession username is null\n";
                abort();
            }

            if( ! sqlpassword) {
                std::cerr <<"error: in function getSession password is null\n";
                abort();
            }

            if( ! sqldbname) {
                std::cerr <<"error: in function getSession dbname is null\n";
                abort();
            }

            std::string loginStr = "service=" + *sqldbname + " user=" + *sqlusername + " password=" + *sqlpassword;
            s = new soci::session(soci::mysql, loginStr.c_str());
            //s = new soci::session(soci::mysql, "service=imd user=root password=root");
        } catch (std::exception const &e) {
            std::cerr << "Sql error: " << e.what() << '\n';
            std::cerr <<"Check if sql server is running or username/password/dbname is correct\n";
            abort();
        }
    }

    return s;
}

SQLSession::~SQLSession()
{
    delete(s);
}

soci::session *SQLSession::s = NULL;