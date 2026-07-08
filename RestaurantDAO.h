#ifndef RESTAURANTDAO_H
#define RESTAURANTDAO_H

#include <vector>
#include "Restaurant.h"

class RestaurantDAO {
public:
    bool addRestaurant(Restaurant restaurant);
    bool updateRestaurant(Restaurant restaurant);
    bool deleteRestaurant(int id);

    std::vector<Restaurant> getAllRestaurants();
    Restaurant getRestaurantById(int id);
    Restaurant getRestaurantByManagerId(int managerId);
};

#endif
