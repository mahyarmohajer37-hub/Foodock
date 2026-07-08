#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include "User.h"
#include <iostream>

class SystemAdmin : public User {
public:
    SystemAdmin(int id = 0, std::string name = "", std::string username = "", std::string password = "")
        : User(id, name, username, password, "admin") {}

    void displayMenu() const override {
        std::cout << "\n========== System Admin Menu ==========\n";
        std::cout << "1. View All Users\n";
        std::cout << "2. View All Restaurants\n";
        std::cout << "3. Add Restaurant\n";
        std::cout << "4. Update Restaurant\n";
        std::cout << "5. Activate/Deactivate Restaurant\n";
        std::cout << "6. View All Orders\n";
        std::cout << "7. Reports\n";
        std::cout << "0. Logout\n";
        std::cout << "=======================================\n";
    }
};

#endif
