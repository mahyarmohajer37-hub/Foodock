#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <iostream>

class Customer : public User {
public:
    Customer(int id = 0, std::string name = "", std::string username = "", std::string password = "")
        : User(id, name, username, password, "customer") {}

    void displayMenu() const override {
        std::cout << "\n========== Customer Menu ==========\n";
        std::cout << "1. View Active Restaurants\n";
        std::cout << "2. View Menu of Restaurant\n";
        std::cout << "3. Add Item to Cart\n";
        std::cout << "4. View Cart\n";
        std::cout << "5. Finalize Order\n";
        std::cout << "6. View My Orders\n";
        std::cout << "0. Logout\n";
        std::cout << "===================================\n";
    }
};

#endif
