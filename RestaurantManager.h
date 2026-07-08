#ifndef RESTAURANTMANAGER_H
#define RESTAURANTMANAGER_H

#include "User.h"
#include <iostream>

class RestaurantManager : public User {
private:
    int restaurantId;

public:
    RestaurantManager(
        int id = 0,
        std::string name = "",
        std::string username = "",
        std::string password = "",
        int restaurantId = 0
    ) : User(id, name, username, password, "manager") {
        this->restaurantId = restaurantId;
    }

    int getRestaurantId() const {
        return restaurantId;
    }

    void setRestaurantId(int restaurantId) {
        this->restaurantId = restaurantId;
    }

    void displayMenu() const override {
        std::cout << "\n======= Restaurant Manager Menu =======\n";
        std::cout << "Managing Restaurant ID: " << restaurantId << "\n";
        std::cout << "1. View My Restaurant Info\n";
        std::cout << "2. Update Restaurant Info\n";
        std::cout << "3. View My Menu Items\n";
        std::cout << "4. Add Menu Item\n";
        std::cout << "5. Update Menu Item\n";
        std::cout << "6. Delete Menu Item\n";
        std::cout << "7. View Orders\n";
        std::cout << "8. Update Order Status\n";
        std::cout << "0. Logout\n";
        std::cout << "=======================================\n";
    }
};

#endif
