#ifndef USERDAO_H
#define USERDAO_H

#include "User.h"
#include <vector>
#include <string>

class UserDAO {
public:
    bool addUser(User user);
    bool updateUser(User user);
    bool deleteUser(int id);

    User getUserById(int id);
    std::vector<User> getAllUsers();

    User* login(std::string username, std::string password);
};

#endif
