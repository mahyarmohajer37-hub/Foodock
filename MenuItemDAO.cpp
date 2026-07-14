#include "MenuItemDAO.h"
#include <iostream>

using namespace std;

bool MenuItemDAO::addMenuItem(MenuItem item) {

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "INSERT INTO MenuItems "
                 "(restaurant_id, name, category, price, available, description, is_special) VALUES (" +
                 to_string(item.getRestaurantId()) + ", '" +
                 item.getName() + "', '" +
                 item.getCategory() + "', " +
                 to_string(item.getPrice()) + ", " +
                 to_string(item.getAvailable()) + ", '" +
                 item.getDescription() + "', " +
                 to_string(item.getIsSpecial()) + ");";

    char* errMsg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cout << "Insert menu item failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool MenuItemDAO::updateMenuItem(MenuItem item) {

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "UPDATE MenuItems SET "
                 "restaurant_id=" + to_string(item.getRestaurantId()) + ", "
                 "name='" + item.getName() + "', "
                 "category='" + item.getCategory() + "', "
                 "price=" + to_string(item.getPrice()) + ", "
                 "available=" + to_string(item.getAvailable()) + ", "
                 "description='" + item.getDescription() + "', "
                 "is_special=" + to_string(item.getIsSpecial()) + " "
                 "WHERE id=" + to_string(item.getId()) + ";";

    char* errMsg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cout << "Update menu item failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool MenuItemDAO::deleteMenuItem(int id) {

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "DELETE FROM MenuItems WHERE id=" + to_string(id) + ";";

    char* errMsg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cout << "Delete menu item failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

vector<MenuItem> MenuItemDAO::getAllMenuItems() {

    vector<MenuItem> items;

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "SELECT * FROM MenuItems;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

        while (sqlite3_step(stmt) == SQLITE_ROW) {

            MenuItem item;

            item.setId(sqlite3_column_int(stmt, 0));
            item.setRestaurantId(sqlite3_column_int(stmt, 1));
            item.setName((char*)sqlite3_column_text(stmt, 2));
            item.setCategory((char*)sqlite3_column_text(stmt, 3));
            item.setPrice(sqlite3_column_double(stmt, 4));
            item.setAvailable(sqlite3_column_int(stmt, 5));
            item.setDescription((char*)sqlite3_column_text(stmt, 6));
            item.setIsSpecial(sqlite3_column_int(stmt, 7));

            items.push_back(item);
        }
    }

    sqlite3_finalize(stmt);

    return items;
}

vector<MenuItem> MenuItemDAO::getMenuItemsByRestaurantId(int restaurantId) {

    vector<MenuItem> items;

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "SELECT * FROM MenuItems WHERE restaurant_id=" +
                 to_string(restaurantId) + ";";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

        while (sqlite3_step(stmt) == SQLITE_ROW) {

            MenuItem item;

            item.setId(sqlite3_column_int(stmt, 0));
            item.setRestaurantId(sqlite3_column_int(stmt, 1));
            item.setName((char*)sqlite3_column_text(stmt, 2));
            item.setCategory((char*)sqlite3_column_text(stmt, 3));
            item.setPrice(sqlite3_column_double(stmt, 4));
            item.setAvailable(sqlite3_column_int(stmt, 5));
            item.setDescription((char*)sqlite3_column_text(stmt, 6));
            item.setIsSpecial(sqlite3_column_int(stmt, 7));

            items.push_back(item);
        }
    }

    sqlite3_finalize(stmt);

    return items;
}

MenuItem MenuItemDAO::getMenuItemById(int id) {

    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "SELECT * FROM MenuItems WHERE id=" + to_string(id) + ";";

    sqlite3_stmt* stmt;
    MenuItem item;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {

        if (sqlite3_step(stmt) == SQLITE_ROW) {

            item.setId(sqlite3_column_int(stmt, 0));
            item.setRestaurantId(sqlite3_column_int(stmt, 1));
            item.setName((char*)sqlite3_column_text(stmt, 2));
            item.setCategory((char*)sqlite3_column_text(stmt, 3));
            item.setPrice(sqlite3_column_double(stmt, 4));
            item.setAvailable(sqlite3_column_int(stmt, 5));
            item.setDescription((char*)sqlite3_column_text(stmt, 6));
            item.setIsSpecial(sqlite3_column_int(stmt, 7));
        }
    }

    sqlite3_finalize(stmt);

    return item;
}
