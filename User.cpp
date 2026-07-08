#include "User.h"

User::User(int id, std::string name, std::string username, std::string password, std::string role) {
    this->id = id;
    this->name = name;
    this->username = username;
    this->password = password;
    this->role = role;
}

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

std::string User::getRole() const {
    return role;
}

void User::setId(int id) {
    this->id = id;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setUsername(const std::string& username) {
    this->username = username;
}

void User::setPassword(const std::string& password) {
    this->password = password;
}

void User::setRole(const std::string& role) {
    this->role = role;
}

void User::displayMenu() const {
    std::cout << "\n--- User Menu ---" << std::endl;
    std::cout << "Base user has no specific menu." << std::endl;
}
