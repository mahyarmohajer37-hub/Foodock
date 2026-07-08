#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

class User {
public:
    User(int id = 0, std::string name = "", std::string username = "", std::string password = "", std::string role = "");
    
    virtual ~User() {}

    int getId() const;
    std::string getName() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole() const;

    void setId(int id);
    void setName(const std::string& name);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setRole(const std::string& role);

    virtual void displayMenu() const;
    
protected: 
    int id;
    std::string name;
    std::string username;
    std::string password;
    std::string role;
};

#endif
