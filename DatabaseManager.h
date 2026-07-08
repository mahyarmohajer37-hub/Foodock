#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <string>
#include "sqlite3.h"

class DatabaseManager {
public:
    static DatabaseManager* getInstance();
    static void releaseInstance();
    sqlite3* getDB();

private:
    static DatabaseManager* instance;
    sqlite3* db;

    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void createTables();
};

#endif
