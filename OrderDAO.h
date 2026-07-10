#ifndef ORDERDAO_H
#define ORDERDAO_H

#include "Order.h"
#include <vector>
#include <string>

class OrderDAO {
public:
    bool addOrder(Order order);
    int addOrderAndReturnId(Order order);
    bool updateOrderStatus(int orderId, std::string status);
    bool deleteOrder(int id);

    Order getOrderById(int id);
    std::vector<Order> getAllOrders();
    std::vector<Order> getOrdersByCustomerId(int customerId);
    std::vector<Order> getOrdersByRestaurantId(int restaurantId);

    void applyCustomerLoyaltyAfterOrder(Order& order);

private:
    bool insertMembershipHistory(int userId, const std::string& oldLevel,
                                 const std::string& newLevel, int pointsAtChange);
};

#endif
