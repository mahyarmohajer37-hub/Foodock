#include "UserDAO.h"
#include "Customer.h"
#include "RestaurantManager.h"
#include "SystemAdmin.h"
#include "RestaurantDAO.h"
#include "DatabaseManager.h"
#include "MembershipFactory.h"
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include "sqlite3.h"

bool UserDAO::addUser(User user) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql;
    sqlite3_stmt* stmt;

    if (user.getRole() == "customer") {
        sql = "INSERT INTO Users (name, username, password, role, points, level_current) VALUES (?, ?, ?, ?, ?, ?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, 0);
            sqlite3_bind_text(stmt, 6, "Normal", -1, SQLITE_TRANSIENT);

            bool success = (sqlite3_step(stmt) == SQLITE_DONE);
            sqlite3_finalize(stmt);
            return success;
        }
    } else {
        sql = "INSERT INTO Users (name, username, password, role) VALUES (?, ?, ?, ?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, user.getRole().c_str(), -1, SQLITE_TRANSIENT);

            bool success = (sqlite3_step(stmt) == SQLITE_DONE);
            sqlite3_finalize(stmt);
            return success;
        }
    }

    std::cout << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
    return false;
}

bool UserDAO::updateUser(User user) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::string sql = "UPDATE Users SET name = ?, username = ?, password = ?, role = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, user.getId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

User* UserDAO::login(std::string username, std::string password) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql = "SELECT id, name, username, password, role, points, level_current FROM Users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        std::cout << "Login query failed: " << sqlite3_errmsg(db) << std::endl;
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    User* loggedInUser = NULL;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string uname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string pass = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        if (role == "customer") {
            int points = 0;
            std::string level_current = "Normal";

            if (sqlite3_column_text(stmt, 5) != NULL) {
                points = sqlite3_column_int(stmt, 5);
            }
            if (sqlite3_column_text(stmt, 6) != NULL) {
                level_current = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            }

            Customer* customer = new Customer(id, name, uname, pass);
            customer->setPoints(points);

            MembershipLevel* mLevel = MembershipFactory::createLevelByName(level_current);
            if (mLevel != NULL) {
                customer->setMembershipLevel(mLevel);
            }
            loggedInUser = customer;
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

User UserDAO::getUserById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::string sql = "SELECT id, name, username, password, role FROM Users WHERE id = ?;";
    sqlite3_stmt* stmt;

    User user(0, "", "", "", "");

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            user = User(id, name, username, password, role);
        }
        sqlite3_finalize(stmt);
    }
    return user;
}

Customer* UserDAO::getCustomerById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::string sql = "SELECT id, name, username, password, points, level_current FROM Users WHERE id = ? AND role = 'customer';";
    sqlite3_stmt* stmt;
    Customer* customer = NULL;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int points = sqlite3_column_int(stmt, 4);
            std::string level_current = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

            customer = new Customer(id, name, username, password);
            customer->setPoints(points);
            MembershipLevel* mLevel = MembershipFactory::createLevelByName(level_current);
            if (mLevel != NULL) {
                customer->setMembershipLevel(mLevel);
            }
        }
        sqlite3_finalize(stmt);
    }
    return customer;
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
        std::string name = (sqlite3_column_text(stmt, 1) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "";
        std::string username = (sqlite3_column_text(stmt, 2) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        std::string password = (sqlite3_column_text(stmt, 3) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        std::string role = (sqlite3_column_text(stmt, 4) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : "";

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
        std::cout << "Error preparing delete user statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

bool UserDAO::updateCustomerLoyalty(int customerId, int points, const std::string& level) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::string sql = "UPDATE Users SET points = ?, level_current = ? WHERE id = ? AND role = 'customer';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing loyalty update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, points);
    sqlite3_bind_text(stmt, 2, level.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, customerId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::map<std::string, int> UserDAO::getCustomerCountByLevel() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::map<std::string, int> stats;

    stats["Normal"] = 0;
    stats["Silver"] = 0;
    stats["Gold"] = 0;
    stats["VIP"] = 0;

    std::string sql = "SELECT level_current, COUNT(*) FROM Users WHERE role = 'customer' GROUP BY level_current;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing customer count by level statement: " << sqlite3_errmsg(db) << std::endl;
        return stats;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string level = "Normal";
        if (sqlite3_column_text(stmt, 0) != NULL) {
            level = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        int count = sqlite3_column_int(stmt, 1);
        stats[level] = count;
    }

    sqlite3_finalize(stmt);
    return stats;
}

std::vector<UserDAO::MembershipHistoryRecord> UserDAO::getMembershipHistory() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::vector<MembershipHistoryRecord> history;

    std::string sql =
        "SELECT mh.id, mh.user_id, u.name, mh.old_level, mh.new_level, mh.points_at_change, mh.changed_at "
        "FROM MembershipHistory mh "
        "JOIN Users u ON mh.user_id = u.id "
        "ORDER BY mh.changed_at DESC;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing membership history statement: " << sqlite3_errmsg(db) << std::endl;
        return history;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        MembershipHistoryRecord record;

        record.id = sqlite3_column_int(stmt, 0);
        record.userId = sqlite3_column_int(stmt, 1);
        record.userName = (sqlite3_column_text(stmt, 2) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        record.oldLevel = (sqlite3_column_text(stmt, 3) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        record.newLevel = (sqlite3_column_text(stmt, 4) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : "";
        record.pointsAtChange = sqlite3_column_int(stmt, 5);
        record.changedAt = (sqlite3_column_text(stmt, 6) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)) : "";

        history.push_back(record);
    }

    sqlite3_finalize(stmt);
    return history;
}

std::vector<Customer*> UserDAO::getAllCustomersWithDetails() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::vector<Customer*> customers;

    std::string sql = "SELECT id, name, username, password, points, level_current FROM Users WHERE role = 'customer';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing get all customers statement: " << sqlite3_errmsg(db) << std::endl;
        return customers;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = (sqlite3_column_text(stmt, 1) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "";
        std::string username = (sqlite3_column_text(stmt, 2) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        std::string password = (sqlite3_column_text(stmt, 3) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "";
        int points = sqlite3_column_int(stmt, 4);
        std::string level_current = (sqlite3_column_text(stmt, 5) != NULL) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)) : "Normal";

        Customer* customer = new Customer(id, name, username, password);
        customer->setPoints(points);

        MembershipLevel* mLevel = MembershipFactory::createLevelByName(level_current);
        if (mLevel != NULL) {
            customer->setMembershipLevel(mLevel);
        }

        customers.push_back(customer);
    }

    sqlite3_finalize(stmt);
    return customers;
}

bool UserDAO::downgradeUsersByInactivity(int daysOfInactivity, int pointsPenalty) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    std::vector<int> inactiveUserIds;

    std::stringstream ss;
    ss << "SELECT u.id "
       << "FROM Users u "
       << "WHERE u.role = 'customer' "
       << "AND u.id NOT IN ("
       << "SELECT DISTINCT customer_id FROM Orders "
       << "WHERE datetime(order_date) >= datetime('now', '-" << daysOfInactivity << " days')"
       << ");";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing inactivity query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        inactiveUserIds.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);

    for (size_t i = 0; i < inactiveUserIds.size(); i++) {
        Customer* customer = getCustomerById(inactiveUserIds[i]);
        if (customer == NULL) {
            continue;
        }

        std::string oldLevel = customer->getLevelName();

        int newPoints = customer->getPoints() - pointsPenalty;
        if (newPoints < 0) {
            newPoints = 0;
        }

        customer->setPoints(newPoints);
        std::string newLevel = customer->getLevelName();

        if (!updateCustomerLoyalty(customer->getId(), customer->getPoints(), customer->getLevelName())) {
            delete customer;
            return false;
        }

        if (oldLevel != newLevel) {
            std::string histSql = "INSERT INTO MembershipHistory (user_id, old_level, new_level, points_at_change) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* histStmt;

            if (sqlite3_prepare_v2(db, histSql.c_str(), -1, &histStmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(histStmt, 1, customer->getId());
                sqlite3_bind_text(histStmt, 2, oldLevel.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(histStmt, 3, newLevel.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(histStmt, 4, customer->getPoints());

                sqlite3_step(histStmt);
                sqlite3_finalize(histStmt);
            }
        }

        delete customer;
    }

    return true;
}
