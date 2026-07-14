#include "MenuItem.h"

MenuItem::MenuItem() {
    id = 0;
    restaurantId = 0;
    name = "";
    category = "";
    price = 0;
    available = 1;
    description = "";
    isSpecial = 0;
}

MenuItem::MenuItem(int id,
                   int restaurantId,
                   std::string name,
                   std::string category,
                   double price,
                   int available,
                   std::string description,
                   int isSpecial,
                   int prepTime,
                   double weight,
                   double volume) {
    this->id = id;
    this->restaurantId = restaurantId;
    this->name = name;
    this->category = category;
    this->price = price;
    this->available = available;
    this->description = description;
    this->isSpecial = isSpecial;
    this->prepTime = prepTime;
    this->weight = weight;
    this->volume = volume;
}

void MenuItem::setId(int id) {
    this->id = id;
}

void MenuItem::setRestaurantId(int restaurantId) {
    this->restaurantId = restaurantId;
}

void MenuItem::setName(std::string name) {
    this->name = name;
}

void MenuItem::setCategory(std::string category) {
    this->category = category;
}

void MenuItem::setPrice(double price) {
    this->price = price;
}

void MenuItem::setAvailable(int available) {
    this->available = available;
}

void MenuItem::setDescription(std::string description) {
    this->description = description;
}

void MenuItem::setIsSpecial(int isSpecial) {
    this->isSpecial = isSpecial;
}

void MenuItem::setPrepTime(int prepTime) {
    this->prepTime = prepTime;
}

void MenuItem::setWeight(double weight) {
    this->weight = weight;
}

void MenuItem::setVolume(double volume) {
    this->volume = volume;
}

int MenuItem::getId() {
    return id;
}

int MenuItem::getRestaurantId() {
    return restaurantId;
}

std::string MenuItem::getName() {
    return name;
}

std::string MenuItem::getCategory() {
    return category;
}

double MenuItem::getPrice() {
    return price;
}

int MenuItem::getAvailable() {
    return available;
}

std::string MenuItem::getDescription() {
    return description;
}

int MenuItem::getIsSpecial() const {
    return isSpecial;
}

int MenuItem::getPrepTime() {
    return prepTime;
}

double MenuItem::getWeight() {
    return weight;
}

double MenuItem::getVolume() {
    return volume;
}