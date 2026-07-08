#ifndef ORDERITEM_H
#define ORDERITEM_H

class OrderItem {
private:
    int id;
    int orderId;
    int menuItemId;
    int quantity;
    double price;

public:
    OrderItem();
    OrderItem(int id, int orderId, int menuItemId, int quantity, double price);

    void setId(int id);
    void setOrderId(int orderId);
    void setMenuItemId(int menuItemId);
    void setQuantity(int quantity);
    void setPrice(double price);

    int getId();
    int getOrderId();
    int getMenuItemId();
    int getQuantity();
    double getPrice();
};

#endif
