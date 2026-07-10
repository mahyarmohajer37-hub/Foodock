#include "OrderDAO.h"
#include "DatabaseManager.h"
#include "UserDAO.h"
#include "Customer.h"
#include "sqlite3.h"
#include <iostream>
#include <sstream>

void OrderDAO::applyCustomerLoyaltyAfterOrder(Order& order) {
    UserDAO userDAO;
    Customer* customer = userDAO.getCustomerById(order.getCustomerId());

    if (customer == NULL) {
        return;
    }

    std::string oldLevel = customer->getLevelName();

    int earnedPoints =
        static_cast<int>(order.getTotalPrice() * customer->getPointMultiplier());

    customer->addPoints(earnedPoints);

    std::string newLevel = customer->getLevelName();

    if (oldLevel != newLevel) {
        insertMembershipHistory(customer->getId(), oldLevel, newLevel, customer->getPoints());
    }

    userDAO.updateCustomerLoyalty(
        customer->getId(),
        customer->getPoints(),
        customer->getLevelName()
    );

    delete customer;
}

bool OrderDAO::insertMembershipHistory(int userId, const std::string& oldLevel,
                                       const std::string& newLevel, int pointsAtChange) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        "INSERT INTO MembershipHistory "
        "(user_id, old_level, new_level, points_at_change) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        std::cout << "Error preparing membership history statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, oldLevel.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newLevel.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, pointsAtChange);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool OrderDAO::addOrder(Order order) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "INSERT INTO Orders (customer_id, restaurant_id, total_price, status, order_date) VALUES ("
       << order.getCustomerId() << ", "
       << order.getRestaurantId() << ", "
       << order.getTotalPrice() << ", '"
       << order.getStatus() << "', '"
       << order.getOrderDate() << "');";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << "Error adding order: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

int OrderDAO::addOrderAndReturnId(Order order) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "INSERT INTO Orders (customer_id, restaurant_id, total_price, status, order_date) VALUES ("
       << order.getCustomerId() << ", "
       << order.getRestaurantId() << ", "
       << order.getTotalPrice() << ", '"
       << order.getStatus() << "', '"
       << order.getOrderDate() << "');";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << "Error adding order: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return -1;
    }

    int orderId = static_cast<int>(sqlite3_last_insert_rowid(db));
    return orderId;
}

bool OrderDAO::updateOrderStatus(int orderId, std::string status) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "UPDATE Orders SET status = '" << status
       << "' WHERE id = " << orderId << ";";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << "Error updating order status: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool OrderDAO::deleteOrder(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "DELETE FROM Orders WHERE id = " << id << ";";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << "Error deleting order: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

Order OrderDAO::getOrderById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "SELECT id, customer_id, restaurant_id, total_price, status, order_date "
       << "FROM Orders WHERE id = " << id << ";";

    sqlite3_stmt* stmt;
    Order order;

    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            order.setId(sqlite3_column_int(stmt, 0));
            order.setCustomerId(sqlite3_column_int(stmt, 1));
            order.setRestaurantId(sqlite3_column_int(stmt, 2));
            order.setTotalPrice(sqlite3_column_double(stmt, 3));

            const unsigned char* statusText = sqlite3_column_text(stmt, 4);
            const unsigned char* dateText = sqlite3_column_text(stmt, 5);

            order.setStatus(statusText ? reinterpret_cast<const char*>(statusText) : "");
            order.setOrderDate(dateText ? reinterpret_cast<const char*>(dateText) : "");
        }
    } else {
        std::cout << "Error getting order by id." << std::endl;
    }

    sqlite3_finalize(stmt);
    return order;
}

std::vector<Order> OrderDAO::getAllOrders() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql =
        "SELECT id, customer_id, restaurant_id, total_price, status, order_date FROM Orders;";

    sqlite3_stmt* stmt;
    std::vector<Order> orders;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Order order;

            order.setId(sqlite3_column_int(stmt, 0));
            order.setCustomerId(sqlite3_column_int(stmt, 1));
            order.setRestaurantId(sqlite3_column_int(stmt, 2));
            order.setTotalPrice(sqlite3_column_double(stmt, 3));

            const unsigned char* statusText = sqlite3_column_text(stmt, 4);
            const unsigned char* dateText = sqlite3_column_text(stmt, 5);

            order.setStatus(statusText ? reinterpret_cast<const char*>(statusText) : "");
            order.setOrderDate(dateText ? reinterpret_cast<const char*>(dateText) : "");

            orders.push_back(order);
        }
    } else {
        std::cout << "Error getting all orders." << std::endl;
    }

    sqlite3_finalize(stmt);
    return orders;
}

std::vector<Order> OrderDAO::getOrdersByCustomerId(int customerId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "SELECT id, customer_id, restaurant_id, total_price, status, order_date "
       << "FROM Orders WHERE customer_id = " << customerId << ";";

    sqlite3_stmt* stmt;
    std::vector<Order> orders;

    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Order order;

            order.setId(sqlite3_column_int(stmt, 0));
            order.setCustomerId(sqlite3_column_int(stmt, 1));
            order.setRestaurantId(sqlite3_column_int(stmt, 2));
            order.setTotalPrice(sqlite3_column_double(stmt, 3));

            const unsigned char* statusText = sqlite3_column_text(stmt, 4);
            const unsigned char* dateText = sqlite3_column_text(stmt, 5);

            order.setStatus(statusText ? reinterpret_cast<const char*>(statusText) : "");
            order.setOrderDate(dateText ? reinterpret_cast<const char*>(dateText) : "");

            orders.push_back(order);
        }
    } else {
        std::cout << "Error getting customer orders." << std::endl;
    }

    sqlite3_finalize(stmt);
    return orders;
}

std::vector<Order> OrderDAO::getOrdersByRestaurantId(int restaurantId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "SELECT id, customer_id, restaurant_id, total_price, status, order_date "
       << "FROM Orders WHERE restaurant_id = " << restaurantId << ";";

    sqlite3_stmt* stmt;
    std::vector<Order> orders;

    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Order order;

            order.setId(sqlite3_column_int(stmt, 0));
            order.setCustomerId(sqlite3_column_int(stmt, 1));
            order.setRestaurantId(sqlite3_column_int(stmt, 2));
            order.setTotalPrice(sqlite3_column_double(stmt, 3));

            const unsigned char* statusText = sqlite3_column_text(stmt, 4);
            const unsigned char* dateText = sqlite3_column_text(stmt, 5);

            order.setStatus(statusText ? reinterpret_cast<const char*>(statusText) : "");
            order.setOrderDate(dateText ? reinterpret_cast<const char*>(dateText) : "");

            orders.push_back(order);
        }
    } else {
        std::cout << "Error getting restaurant orders." << std::endl;
    }

    sqlite3_finalize(stmt);
    return orders;
}
