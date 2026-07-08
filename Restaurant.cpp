#include "Restaurant.h"

Restaurant::Restaurant(
    int id,
    std::string name,
    std::string address,
    int status,
    int prepTime,
    std::string contact,
    std::string description,
    int managerId
) {
    this->id = id;
    this->name = name;
    this->address = address;
    this->status = status;
    this->prepTime = prepTime;
    this->contact = contact;
    this->description = description;
    this->managerId = managerId;
}

int Restaurant::getId() const { return id; }
std::string Restaurant::getName() const { return name; }
std::string Restaurant::getAddress() const { return address; }
int Restaurant::getStatus() const { return status; }
int Restaurant::getPrepTime() const { return prepTime; }
std::string Restaurant::getContact() const { return contact; }
std::string Restaurant::getDescription() const { return description; }
int Restaurant::getManagerId() const { return managerId; }

void Restaurant::setId(int id) { this->id = id; }
void Restaurant::setName(const std::string& name) { this->name = name; }
void Restaurant::setAddress(const std::string& address) { this->address = address; }
void Restaurant::setStatus(int status) { this->status = status; }
void Restaurant::setPrepTime(int prepTime) { this->prepTime = prepTime; }
void Restaurant::setContact(const std::string& contact) { this->contact = contact; }
void Restaurant::setDescription(const std::string& description) { this->description = description; }
void Restaurant::setManagerId(int managerId) { this->managerId = managerId; }
