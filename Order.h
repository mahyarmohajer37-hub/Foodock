#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {
private:
    int id;
    int customerId;
    int restaurantId;
    double totalPrice;
    std::string status;
    std::string orderDate;

public:
    Order();

    Order(int id, int customerId, int restaurantId, double totalPrice,
          std::string status, std::string orderDate);

    void setId(int id);
    void setCustomerId(int customerId);
    void setRestaurantId(int restaurantId);
    void setTotalPrice(double totalPrice);
    void setStatus(std::string status);
    void setOrderDate(std::string orderDate);

    int getId();
    int getCustomerId();
    int getRestaurantId();
    double getTotalPrice();
    std::string getStatus();
    std::string getOrderDate();
};

#endif
