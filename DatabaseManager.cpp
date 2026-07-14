#include "DatabaseManager.h"
#include <iostream>
#include <string>

using namespace std;

DatabaseManager* DatabaseManager::instance = NULL;

DatabaseManager::DatabaseManager() {
    int rc = sqlite3_open("Foodock.db", &db);

    if (rc != SQLITE_OK) {
        cout << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        db = NULL;
    } else {
        cout << "Database opened successfully." << endl;
        createTables();
    }
}

DatabaseManager::~DatabaseManager() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == NULL) {
        instance = new DatabaseManager();
    }
    return instance;
}

void DatabaseManager::releaseInstance() {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

sqlite3* DatabaseManager::getDB() {
    return db;
}

void DatabaseManager::createTables() {
    char* errMsg = 0;
    int rc;

    string createUsersTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "role TEXT NOT NULL, "
        "points INTEGER NOT NULL DEFAULT 0, "
        "level_current TEXT NOT NULL DEFAULT 'Normal'"
        ");";

    string createRestaurantsTable =
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

    string createMenuItemsTable =
        "CREATE TABLE IF NOT EXISTS MenuItems ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_id INTEGER NOT NULL, "
        "name TEXT NOT NULL, "
        "category TEXT, "
        "price REAL NOT NULL, "
        "available INTEGER NOT NULL DEFAULT 1, "
        "description TEXT, "
        "is_special INTEGER NOT NULL DEFAULT 0, "
        "prep_time INTEGER NOT NULL DEFAULT 0, "
        "weight REAL NOT NULL DEFAULT 0, "
        "volume REAL NOT NULL DEFAULT 0, "
        "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(id)"
        ");";

    string createOrdersTable =
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

    string createOrderItemsTable =
        "CREATE TABLE IF NOT EXISTS OrderItems ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "order_id INTEGER NOT NULL, "
        "menu_item_id INTEGER NOT NULL, "
        "quantity INTEGER NOT NULL, "
        "price REAL NOT NULL, "
        "FOREIGN KEY(order_id) REFERENCES Orders(id), "
        "FOREIGN KEY(menu_item_id) REFERENCES MenuItems(id)"
        ");";

    string createMembershipHistoryTable =
        "CREATE TABLE IF NOT EXISTS MembershipHistory ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "old_level TEXT NOT NULL, "
        "new_level TEXT NOT NULL, "
        "points_at_change INTEGER NOT NULL DEFAULT 0, "
        "changed_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(user_id) REFERENCES Users(id)"
        ");";

    string createCouponsTable =
        "CREATE TABLE IF NOT EXISTS Coupons ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "code TEXT UNIQUE NOT NULL, "
        "discount_percent REAL NOT NULL, "
        "is_used INTEGER NOT NULL DEFAULT 0, "
        "issued_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, "
        "expires_at TEXT, "
        "FOREIGN KEY(user_id) REFERENCES Users(id)"
        ");";

    rc = sqlite3_exec(db, createUsersTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating Users table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createRestaurantsTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating Restaurants table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createMenuItemsTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating MenuItems table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createOrdersTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating Orders table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createOrderItemsTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating OrderItems table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createMembershipHistoryTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating MembershipHistory table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    rc = sqlite3_exec(db, createCouponsTable.c_str(), NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error creating Coupons table: " << errMsg << endl;
        sqlite3_free(errMsg);
        errMsg = 0;
    }

    sqlite3_exec(
        db,
        "ALTER TABLE Users ADD COLUMN points INTEGER NOT NULL DEFAULT 0;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE Users ADD COLUMN level_current TEXT NOT NULL DEFAULT 'Normal';",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE MenuItems ADD COLUMN is_special INTEGER NOT NULL DEFAULT 0;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE MenuItems ADD COLUMN prep_time INTEGER NOT NULL DEFAULT 0;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE MenuItems ADD COLUMN weight REAL NOT NULL DEFAULT 0;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE MenuItems ADD COLUMN volume REAL NOT NULL DEFAULT 0;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE Coupons ADD COLUMN code TEXT;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "ALTER TABLE Coupons ADD COLUMN expires_at TEXT;",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "UPDATE Coupons "
        "SET code = 'COUPON20-' || id "
        "WHERE code IS NULL OR code = '';",
        NULL, NULL, NULL
    );

    sqlite3_exec(
        db,
        "CREATE UNIQUE INDEX IF NOT EXISTS idx_coupons_code "
        "ON Coupons(code);",
        NULL, NULL, NULL
    );
}
