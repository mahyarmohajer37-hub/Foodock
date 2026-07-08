#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>

class Restaurant {
private:
    int id;
    std::string name;
    std::string address;
    int status;
    int prepTime;
    std::string contact;
    std::string description;
    int managerId;

public:
    Restaurant(
        int id = 0,
        std::string name = "",
        std::string address = "",
        int status = 1,
        int prepTime = 0,
        std::string contact = "",
        std::string description = "",
        int managerId = 0
    );

    int getId() const;
    std::string getName() const;
    std::string getAddress() const;
    int getStatus() const;
    int getPrepTime() const;
    std::string getContact() const;
    std::string getDescription() const;
    int getManagerId() const;

    void setId(int id);
    void setName(const std::string& name);
    void setAddress(const std::string& address);
    void setStatus(int status);
    void setPrepTime(int prepTime);
    void setContact(const std::string& contact);
    void setDescription(const std::string& description);
    void setManagerId(int managerId);
};

#endif
