#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>

class MenuItem {
public:
    MenuItem();

    MenuItem(int id,
             int restaurantId,
             std::string name,
             std::string category,
             double price,
             int available,
             std::string description,
             int isSpecial,
			 int prepTime,
             double weight,
             double volume);

    void setId(int id);
    void setRestaurantId(int restaurantId);
    void setName(std::string name);
    void setCategory(std::string category);
    void setPrice(double price);
    void setAvailable(int available);
    void setDescription(std::string description);
    void setIsSpecial(int isSpecial);
    void setPrepTime(int prepTime);
    void setWeight(double weight);
    void setVolume(double volume);

    int getId();
    int getRestaurantId();
    std::string getName();
    std::string getCategory();
    double getPrice();
    int getAvailable();
    std::string getDescription();
    int getIsSpecial() const;
    int getPrepTime();
    double getWeight();
    double getVolume();

private:
    int id;
    int restaurantId;
    std::string name;
    std::string category;
    double price;
    int available;
    std::string description;
    int isSpecial;
    int prepTime;
    double weight;
    double volume;
};

#endif
