#include "sqlhandler.h"
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <ctime>

SqlHandler::SqlHandler(const std::string& dbname) {
    int rc;
    /*Connect to database*/
    rc = sqlite3_open(dbname.c_str(), &db);
    createDataTable();

}

bool const SqlHandler::dbExists(const std::string& dbname) {
    return ( access(dbname.c_str(), F_OK) != -1);
}

void SqlHandler::createDataTable() {
    const char* sql;
    char* zErrMsg = 0;
    const char* data = "Callback function called";
    int rc;

    sql = "CREATE TABLE IF NOT EXISTS fightdata (" \
                "id INTEGER PRIMARY KEY, " \
                "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "\
                "playerBoard TEXT NOT NULL, "\
                "enemyBoard TEXT NOT NULL, "\
                "damage TEXT NOT NULL, "\
                "playerLevel INTEGER DEFAULT -1, "\
                "enemyLevel INTEGER DEFAULT -1, "\
                "sumRankPB INTEGER DEFAULT -1, "\
                "sumRankEB INTEGER DEFAULT -1 "\
        ");";
    rc = sqlite3_exec(db, sql, callback, this, &zErrMsg);
    std::cout << "Table Created" << std::endl;

}

int SqlHandler::insertDataTable() {
    const char* sql;
    const char* zErrMsg = 0;
    const char* data = "Callback function called";
    int rc;
    auto currtime = std::chrono::system_clock::now();
    std::time_t curr_time = std::chrono::system_clock::to_time_t(currtime);

    // sql = "INSERT INTO fightdata (playerBoard, enemyBoard, damage) VALUES ("
    //             + quotesql("testpb") + ","
    //             + quotesql("testeb") + ","
    //             + quotesql("testdmg") + ");";

    sql = "INSERT INTO fightdata (playerBoard, enemyBoard, damage) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, &zErrMsg);
    if(rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, "pb", 2, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, "eb", 2, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, "tdmg", 4, SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }


    // char * buffer = new char[sql.length()];
    // std::strcpy(buffer,sql.c_str());

    // rc = sqlite3_exec(db, sql, callback, this, &zErrMsg);
    std::cout << "Test Data Inserted" << std::endl;

    return 0;
}

std::string SqlHandler::quotesql( const std::string& s ) {
    return std::string("'") + s + std::string("'");
}

void SqlHandler::closedb() {sqlite3_close(db);}
