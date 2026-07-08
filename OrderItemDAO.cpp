#include "OrderItemDAO.h"
#include "DatabaseManager.h"
#include "sqlite3.h"
#include <iostream>
#include <sstream>

bool OrderItemDAO::addOrderItem(OrderItem orderItem) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "INSERT INTO OrderItems (order_id, menu_item_id, quantity, price) VALUES ("
       << orderItem.getOrderId() << ", "
       << orderItem.getMenuItemId() << ", "
       << orderItem.getQuantity() << ", "
       << orderItem.getPrice() << ");";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << " Order Item addition failed " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool OrderItemDAO::updateOrderItem(OrderItem orderItem) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "UPDATE OrderItems SET "
       << "order_id = " << orderItem.getOrderId() << ", "
       << "menu_item_id = " << orderItem.getMenuItemId() << ", "
       << "quantity = " << orderItem.getQuantity() << ", "
       << "price = " << orderItem.getPrice()
       << " WHERE id = " << orderItem.getId() << ";";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << " Order Item update failed " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool OrderItemDAO::deleteOrderItem(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "DELETE FROM OrderItems WHERE id = " << id << ";";

    char* errorMessage = 0;
    int result = sqlite3_exec(db, ss.str().c_str(), 0, 0, &errorMessage);

    if (result != SQLITE_OK) {
        std::cout << "Order Item deletion failed " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

OrderItem OrderItemDAO::getOrderItemById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "SELECT id, order_id, menu_item_id, quantity, price "
       << "FROM OrderItems WHERE id = " << id << ";";

    sqlite3_stmt* stmt;
    OrderItem orderItem;

    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            orderItem.setId(sqlite3_column_int(stmt, 0));
            orderItem.setOrderId(sqlite3_column_int(stmt, 1));
            orderItem.setMenuItemId(sqlite3_column_int(stmt, 2));
            orderItem.setQuantity(sqlite3_column_int(stmt, 3));
            orderItem.setPrice(sqlite3_column_double(stmt, 4));
        }
    } else {
        std::cout << " Getting Order Item by id failed" << std::endl;
    }

    sqlite3_finalize(stmt);
    return orderItem;
}

std::vector<OrderItem> OrderItemDAO::getAllOrderItems() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::string sql = "SELECT id, order_id, menu_item_id, quantity, price FROM OrderItems;";
    sqlite3_stmt* stmt;
    std::vector<OrderItem> orderItems;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            OrderItem orderItem;
            orderItem.setId(sqlite3_column_int(stmt, 0));
            orderItem.setOrderId(sqlite3_column_int(stmt, 1));
            orderItem.setMenuItemId(sqlite3_column_int(stmt, 2));
            orderItem.setQuantity(sqlite3_column_int(stmt, 3));
            orderItem.setPrice(sqlite3_column_double(stmt, 4));

            orderItems.push_back(orderItem);
        }
    } else {
        std::cout << " Getting all Order Items failed " << std::endl;
    }

    sqlite3_finalize(stmt);
    return orderItems;
}

std::vector<OrderItem> OrderItemDAO::getItemsByOrderId(int orderId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    std::stringstream ss;
    ss << "SELECT id, order_id, menu_item_id, quantity, price "
       << "FROM OrderItems WHERE order_id = " << orderId << ";";

    sqlite3_stmt* stmt;
    std::vector<OrderItem> orderItems;

    if (sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            OrderItem orderItem;
            orderItem.setId(sqlite3_column_int(stmt, 0));
            orderItem.setOrderId(sqlite3_column_int(stmt, 1));
            orderItem.setMenuItemId(sqlite3_column_int(stmt, 2));
            orderItem.setQuantity(sqlite3_column_int(stmt, 3));
            orderItem.setPrice(sqlite3_column_double(stmt, 4));

            orderItems.push_back(orderItem);
        }
    } else {
        std::cout << " Getting Order Items by order id failed " << std::endl;
    }

    sqlite3_finalize(stmt);
    return orderItems;
}
