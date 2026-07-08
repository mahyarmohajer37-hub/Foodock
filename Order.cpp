#include "Order.h"

Order::Order() {
    this->id = 0;
    this->customerId = 0;
    this->restaurantId = 0;
    this->totalPrice = 0.0;
    this->status = "";
    this->orderDate = "";
}

Order::Order(int id, int customerId, int restaurantId, double totalPrice,
             std::string status, std::string orderDate) {
    this->id = id;
    this->customerId = customerId;
    this->restaurantId = restaurantId;
    this->totalPrice = totalPrice;
    this->status = status;
    this->orderDate = orderDate;
}

void Order::setId(int id) {
    this->id = id;
}

void Order::setCustomerId(int customerId) {
    this->customerId = customerId;
}

void Order::setRestaurantId(int restaurantId) {
    this->restaurantId = restaurantId;
}

void Order::setTotalPrice(double totalPrice) {
    this->totalPrice = totalPrice;
}

void Order::setStatus(std::string status) {
    this->status = status;
}

void Order::setOrderDate(std::string orderDate) {
    this->orderDate = orderDate;
}

int Order::getId() {
    return id;
}

int Order::getCustomerId() {
    return customerId;
}

int Order::getRestaurantId() {
    return restaurantId;
}

double Order::getTotalPrice() {
    return totalPrice;
}

std::string Order::getStatus() {
    return status;
}

std::string Order::getOrderDate() {
    return orderDate;
}
