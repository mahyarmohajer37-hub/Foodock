#include "OrderDAO.h"
#include "DatabaseManager.h"
#include "sqlite3.h"
#include <iostream>
#include <sstream>

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

    return static_cast<int>(sqlite3_last_insert_rowid(db));
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
