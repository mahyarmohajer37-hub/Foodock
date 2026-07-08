#include "DatabaseManager.h"
#include <iostream>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {
    int rc = sqlite3_open("Foodock.db", &db);

    if (rc != SQLITE_OK) {
        std::cout << "Error opening DB: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }

    char* errMsg = 0;

    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error enabling foreign keys: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    createTables();
}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

void DatabaseManager::releaseInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

sqlite3* DatabaseManager::getDB() {
    return db;
}

void DatabaseManager::createTables() {
    char* errMsg = 0;

    std::string createUsersTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "role TEXT NOT NULL"
        ");";

    std::string createRestaurantsTable =
        "CREATE TABLE IF NOT EXISTS Restaurants ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "address TEXT, "
        "status INTEGER NOT NULL DEFAULT 1, "
        "prep_time INTEGER DEFAULT 0, "
        "contact TEXT, "
        "description TEXT, "
        "manager_id INTEGER UNIQUE, "
        "FOREIGN KEY(manager_id) REFERENCES Users(id)"
        ");";

    std::string createMenuItemsTable =
        "CREATE TABLE IF NOT EXISTS MenuItems ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_id INTEGER NOT NULL, "
        "name TEXT NOT NULL, "
        "category TEXT, "
        "price REAL NOT NULL, "
        "available INTEGER NOT NULL DEFAULT 1, "
        "description TEXT, "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(id)"
        ");";

    std::string createOrdersTable =
        "CREATE TABLE IF NOT EXISTS Orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "customer_id INTEGER NOT NULL, "
        "restaurant_id INTEGER NOT NULL, "
        "total_price REAL NOT NULL, "
        "status TEXT NOT NULL DEFAULT 'Pending', "
        "order_date TEXT, "
        "FOREIGN KEY(customer_id) REFERENCES Users(id), "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(id)"
        ");";

    std::string createOrderItemsTable =
        "CREATE TABLE IF NOT EXISTS OrderItems ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "order_id INTEGER NOT NULL, "
        "menu_item_id INTEGER NOT NULL, "
        "quantity INTEGER NOT NULL, "
        "price REAL NOT NULL, "
        "FOREIGN KEY(order_id) REFERENCES Orders(id), "
        "FOREIGN KEY(menu_item_id) REFERENCES MenuItems(id)"
        ");";

    if (sqlite3_exec(db, createUsersTable.c_str(), NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating Users table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, createRestaurantsTable.c_str(), NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating Restaurants table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, createMenuItemsTable.c_str(), NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating MenuItems table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, createOrdersTable.c_str(), NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating Orders table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, createOrderItemsTable.c_str(), NULL, NULL, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating OrderItems table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

DatabaseManager::~DatabaseManager() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}
