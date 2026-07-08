#include "UserDAO.h"
#include "Customer.h"
#include "RestaurantManager.h"
#include "SystemAdmin.h"
#include "RestaurantDAO.h"
#include "DatabaseManager.h"
#include <iostream>

bool UserDAO::addUser(User user) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        std::string("INSERT INTO Users (name, username, password, role) VALUES ('") +
        user.getName() + "', '" +
        user.getUsername() + "', '" +
        user.getPassword() + "', '" +
        user.getRole() + "');";

    char* errMsg = 0;

    int result = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

    if (result != SQLITE_OK) {
        std::cout << "Error adding user: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

User* UserDAO::login(std::string username, std::string password) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        std::string("SELECT id, name, username, password, role FROM Users WHERE username = '") +
        username + "' AND password = '" + password + "';";

    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Login query failed: " << sqlite3_errmsg(db) << std::endl;
        return NULL;
    }

    User* loggedInUser = NULL;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string uname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string pass = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        if (role == "customer") {
            loggedInUser = new Customer(id, name, uname, pass);
        }
        else if (role == "manager") {
            RestaurantDAO restaurantDAO;
            Restaurant r = restaurantDAO.getRestaurantByManagerId(id);
            loggedInUser = new RestaurantManager(id, name, uname, pass, r.getId());
        }
        else if (role == "admin") {
            loggedInUser = new SystemAdmin(id, name, uname, pass);
        }
        else {
            loggedInUser = new User(id, name, uname, pass, role);
        }
    }

    sqlite3_finalize(stmt);

    return loggedInUser;
}

std::vector<User> UserDAO::getAllUsers() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::vector<User> users;

    std::string sql = "SELECT id, name, username, password, role FROM Users;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Error fetching users: " << sqlite3_errmsg(db) << std::endl;
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);

        std::string name = "";
        std::string username = "";
        std::string password = "";
        std::string role = "";

        if (sqlite3_column_text(stmt, 1) != NULL) {
            name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        }

        if (sqlite3_column_text(stmt, 2) != NULL) {
            username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        }

        if (sqlite3_column_text(stmt, 3) != NULL) {
            password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        }

        if (sqlite3_column_text(stmt, 4) != NULL) {
            role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        }

        User user(id, name, username, password, role);
        users.push_back(user);
    }

    sqlite3_finalize(stmt);
    return users;
}

bool UserDAO::deleteUser(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql = "DELETE FROM Users WHERE id = ?;";

    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        std::cout << "Error preparing delete user statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        std::cout << "Error deleting user: "
                  << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
