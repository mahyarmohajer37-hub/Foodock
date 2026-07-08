#include "RestaurantDAO.h"
#include "DatabaseManager.h"
#include <iostream>

bool RestaurantDAO::addRestaurant(Restaurant restaurant) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        std::string("INSERT INTO Restaurants (name, address, status, prep_time, contact, description, manager_id) VALUES ('") +
        restaurant.getName() + "', '" +
        restaurant.getAddress() + "', " +
        std::to_string(restaurant.getStatus()) + ", " +
        std::to_string(restaurant.getPrepTime()) + ", '" +
        restaurant.getContact() + "', '" +
        restaurant.getDescription() + "', " +
        std::to_string(restaurant.getManagerId()) + ");";

    char* errMsg = nullptr;

    int result = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);

    if (result != SQLITE_OK) {
        std::cout << "Error adding restaurant: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool RestaurantDAO::updateRestaurant(Restaurant restaurant) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        std::string("UPDATE Restaurants SET ") +
        "name = '" + restaurant.getName() + "', " +
        "address = '" + restaurant.getAddress() + "', " +
        "status = " + std::to_string(restaurant.getStatus()) + ", " +
        "prep_time = " + std::to_string(restaurant.getPrepTime()) + ", " +
        "contact = '" + restaurant.getContact() + "', " +
        "description = '" + restaurant.getDescription() + "', " +
        "manager_id = " + std::to_string(restaurant.getManagerId()) +
        " WHERE id = " + std::to_string(restaurant.getId()) + ";";

    char* errMsg = nullptr;

    int result = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);

    if (result != SQLITE_OK) {
        std::cout << "Error updating restaurant: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool RestaurantDAO::deleteRestaurant(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql = "DELETE FROM Restaurants WHERE id = " + std::to_string(id) + ";";

    char* errMsg = nullptr;

    int result = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);

    if (result != SQLITE_OK) {
        std::cout << "Error deleting restaurant: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

std::vector<Restaurant> RestaurantDAO::getAllRestaurants() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::vector<Restaurant> restaurants;

    std::string sql = "SELECT id, name, address, status, prep_time, contact, description, manager_id FROM Restaurants;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Error fetching restaurants: " << sqlite3_errmsg(db) << std::endl;
        return restaurants;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Restaurant restaurant(
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_int(stmt, 4),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)),
            sqlite3_column_int(stmt, 7)
        );

        restaurants.push_back(restaurant);
    }

    sqlite3_finalize(stmt);
    return restaurants;
}

Restaurant RestaurantDAO::getRestaurantById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        "SELECT id, name, address, status, prep_time, contact, description, manager_id "
        "FROM Restaurants WHERE id = " + std::to_string(id) + ";";

    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Error fetching restaurant by id: " << sqlite3_errmsg(db) << std::endl;
        return Restaurant();
    }

    Restaurant restaurant;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        restaurant = Restaurant(
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_int(stmt, 4),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)),
            sqlite3_column_int(stmt, 7)
        );
    }

    sqlite3_finalize(stmt);
    return restaurant;
}

Restaurant RestaurantDAO::getRestaurantByManagerId(int managerId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        "SELECT id, name, address, status, prep_time, contact, description, manager_id "
        "FROM Restaurants WHERE manager_id = " + std::to_string(managerId) + ";";

    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Error fetching restaurant by manager id: " << sqlite3_errmsg(db) << std::endl;
        return Restaurant();
    }

    Restaurant restaurant;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        restaurant = Restaurant(
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            sqlite3_column_int(stmt, 3),
            sqlite3_column_int(stmt, 4),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)),
            sqlite3_column_int(stmt, 7)
        );
    }

    sqlite3_finalize(stmt);
    return restaurant;
}
