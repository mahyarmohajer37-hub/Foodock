#include "OrderItem.h"

OrderItem::OrderItem() {
    this->id = 0;
    this->orderId = 0;
    this->menuItemId = 0;
    this->quantity = 0;
    this->price = 0.0;
}

OrderItem::OrderItem(int id, int orderId, int menuItemId, int quantity, double price) {
    this->id = id;
    this->orderId = orderId;
    this->menuItemId = menuItemId;
    this->quantity = quantity;
    this->price = price;
}

void OrderItem::setId(int id) {
    this->id = id;
}

void OrderItem::setOrderId(int orderId) {
    this->orderId = orderId;
}

void OrderItem::setMenuItemId(int menuItemId) {
    this->menuItemId = menuItemId;
}

void OrderItem::setQuantity(int quantity) {
    this->quantity = quantity;
}

void OrderItem::setPrice(double price) {
    this->price = price;
}

int OrderItem::getId() {
    return id;
}

int OrderItem::getOrderId() {
    return orderId;
}

int OrderItem::getMenuItemId() {
    return menuItemId;
}

int OrderItem::getQuantity() {
    return quantity;
}

double OrderItem::getPrice() {
    return price;
}
