#ifndef ORDERITEMDAO_H
#define ORDERITEMDAO_H

#include "OrderItem.h"
#include <vector>

class OrderItemDAO {
public:
    bool addOrderItem(OrderItem orderItem);
    bool updateOrderItem(OrderItem orderItem);
    bool deleteOrderItem(int id);

    OrderItem getOrderItemById(int id);
    std::vector<OrderItem> getAllOrderItems();
    std::vector<OrderItem> getItemsByOrderId(int orderId);
};

#endif
