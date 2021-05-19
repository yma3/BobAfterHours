#ifndef SQLHANDLER_H
#define SQLHANDLER_H

#include <string>
#include <sqlite3.h>

class SqlHandler {
    static int callback(void *data, int argc, char** argv, char** azColName) {return 0;};

    public:
        SqlHandler(const std::string& dbname);
        bool const dbExists(const std::string& dbname);
        void createDataTable();
        int insertDataTable();
        void closedb();

    private:
        sqlite3 *db;
        std::string quotesql( const std::string& s );


};



#endif