#ifndef MENU_ITEM_DAO_H
#define MENU_ITEM_DAO_H

#include <vector>
#include "MenuItem.h"
#include "DatabaseManager.h"

class MenuItemDAO {
public:
    bool addMenuItem(MenuItem item);
    bool updateMenuItem(MenuItem item);
    bool deleteMenuItem(int id);

    std::vector<MenuItem> getAllMenuItems();
    std::vector<MenuItem> getMenuItemsByRestaurantId(int restaurantId);

    MenuItem getMenuItemById(int id);
};

#endif
