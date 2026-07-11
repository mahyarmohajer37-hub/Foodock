#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>

#include "DatabaseManager.h"
#include "UserDAO.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "OrderItemDAO.h"

#include "User.h"
#include "Restaurant.h"
#include "MenuItem.h"
#include "Order.h"
#include "OrderItem.h"

using namespace std;

void clearInputBuffer();

int readInt();

double readDouble();

string readString(const string& prompt);

void pauseProgram();

string trim(const string& str);

string readNonEmptyString(const string& prompt);

string readOptionalString(const string& prompt);

int readZeroOne(const string& prompt);

int readPositiveInt(const string& prompt);

double readPositiveDouble(const string& prompt);

void showWelcome();

void showAllRestaurants(RestaurantDAO& restaurantDAO);

void showAllUsers(UserDAO& userDAO);

void showMenuItemsByRestaurant(int restaurantId);

void seedTestData(UserDAO& userDAO, RestaurantDAO& restaurantDAO);

void registerManager(UserDAO& userDAO);

void registerCustomer(UserDAO& userDAO);

void login(UserDAO& userDAO);

void adminPanel(User* currentUser);
void adminAddUserMenu(UserDAO& userDAO);
void adminDeleteUserMenu(UserDAO& userDAO);
void adminAddRestaurantMenu(RestaurantDAO& restaurantDAO);
void adminUpdateRestaurantMenu(RestaurantDAO& restaurantDAO);
void adminDeleteRestaurantMenu(RestaurantDAO& restaurantDAO);
void showReports(UserDAO& userDAO, OrderDAO& orderDAO);
void showTotalSales(OrderDAO& orderDAO);
void showTotalOrders(OrderDAO& orderDAO);
void showActiveUsersByOrders(UserDAO& userDAO, OrderDAO& orderDAO);

void managerPanel(User* currentUser);
Restaurant getManagerRestaurant(int managerId);
void managerAddMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
void managerUpdateMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
void managerDeleteMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
void managerUpdateRestaurantInfoMenu(RestaurantDAO& restaurantDAO, Restaurant& myRestaurant, User* currentUser);
void managerUpdateOrderStatusMenu(OrderDAO& orderDAO, Restaurant& myRestaurant);

void customerPanel(User* currentUser);
double calculateCartTotal(vector<OrderItem>& cart);
int calculateCartPreviewPoints(vector<OrderItem>& cart, Customer* customer);

int main() {
    DatabaseManager* db = DatabaseManager::getInstance();
    if (db == nullptr) {
        cout << " Database initialization failed \n";
        return 1;
    }

    UserDAO userDAO;
    RestaurantDAO restaurantDAO;

    seedTestData(userDAO, restaurantDAO);

    int choice;
    do {
        showWelcome();
        choice = readPositiveInt("Enter your choice: ");

        switch (choice) {
        case 1:
            login(userDAO);
            break;
        case 2:
        	registerManager(userDAO);
        	break;
        case 3:
            registerCustomer(userDAO);
            pauseProgram();
            break;
        case 0:
            cout << " Farewell\n";
            break;
        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);

    DatabaseManager::releaseInstance();
    return 0;
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt() {
    int value;
    while (!(cin >> value)) {
        cout << " Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

double readDouble() {
    double value;
    while (!(cin >> value)) {
        cout << " Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

string readString(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

void pauseProgram() {
    cout << "\n Press Enter to continue...";
    cin.get();
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

string readNonEmptyString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        value = trim(value);

        if (!value.empty())
            return value;

        cout << " Input cannot be empty. Please try again.\n";
    }
}

string readOptionalString(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return trim(value);
}

int readZeroOne(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        value = readInt();

        if (value == 0 || value == 1)
            return value;

        cout << " Invalid input. Please enter 1 or 0.\n";
    }
}

int readPositiveInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        value = readInt();

        if (value >= 0)
            return value;

        cout << " Invalid input. Please enter a non-negative number.\n";
    }
}

double readPositiveDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        value = readDouble();

        if (value >= 0)
            return value;

        cout << " Invalid input. Please enter a non-negative number.\n";
    }
}

void showWelcome() {
    cout << "\n=================================\n";
    cout << "          Welcome to Foodock     \n";
    cout << "=================================\n";
    cout << "1. Login\n";
    cout << "2. Register as Manager\n";
    cout << "3. Register as Customer\n";
    cout << "0. Exit\n";
    cout << "---------------------------------\n";
}

void showAllRestaurants(RestaurantDAO& restaurantDAO) {
    vector<Restaurant> restaurants = restaurantDAO.getAllRestaurants();

    if (restaurants.empty()) {
        cout << " No restaurants found\n";
        return;
    }

    cout << "\n===== Restaurants =====\n";
    for (size_t i = 0; i < restaurants.size(); i++) {
        cout << " ID: " << restaurants[i].getId()
             << " | Name: " << restaurants[i].getName()
             << " | Address: " << restaurants[i].getAddress()
             << " | Status: " << (restaurants[i].getStatus() ? "active" : "inactive")
             << " | PrepTime: " << restaurants[i].getPrepTime()
             << " | Contact: " << restaurants[i].getContact()
             << " | Manager ID: " << restaurants[i].getManagerId()
             << endl;
    }
}

void showAllUsers(UserDAO& userDAO) {
    vector<User> users = userDAO.getAllUsers();

    if (users.empty()) {
        cout << " No users found\n";
        return;
    }

    cout << "\n===== Users =====\n";
    for (size_t i = 0; i < users.size(); i++) {
        cout << " ID: " << users[i].getId()
             << " | Name: " << users[i].getName()
             << " | Username: " << users[i].getUsername()
             << " | Role: " << users[i].getRole()
             << endl;
    }
}

void showMenuItemsByRestaurant(int restaurantId) {
    MenuItemDAO menuItemDAO;
    vector<MenuItem> items = menuItemDAO.getMenuItemsByRestaurantId(restaurantId);

    if (items.empty()) {
        cout << " No menu items found\n";
        return;
    }

    cout << "\n===== Menu Items =====\n";
    for (size_t i = 0; i < items.size(); i++) {
        cout << " ID: " << items[i].getId()
             << " | Name: " << items[i].getName()
             << " | Category: " << items[i].getCategory()
             << " | Price: " << items[i].getPrice()
             << " | Available: " << (items[i].getAvailable() ? "yes" : "no")
             << " | Description: " << items[i].getDescription()
             << endl;
    }
}

void seedTestData(UserDAO& userDAO, RestaurantDAO& restaurantDAO) {
    if (!userDAO.getAllUsers().empty()) return;

    userDAO.addUser(User(0, "System Admin", "admin", "1234", "admin"));
    userDAO.addUser(User(0, "Manager One", "manager", "1234", "manager"));
    userDAO.addUser(User(0, "Customer One", "customer", "1234", "customer"));

    User* manager = userDAO.login("manager", "1234");
    if (manager != nullptr) {
        Restaurant r(0, "Foodock Restaurant", "Main Street", 1, 20, "09120000000", "Best food in town", manager->getId());
        restaurantDAO.addRestaurant(r);
        delete manager;
    }
}

void registerManager(UserDAO& userDAO) {
    RestaurantDAO restaurantDAO;

    cout << "\n===== Register as Manager =====\n";

    string name = readNonEmptyString(" Enter manager name: ");
    string username = readNonEmptyString(" Enter username: ");
    string password = readNonEmptyString(" Enter password: ");

    bool userAdded = userDAO.addUser(User(0, name, username, password, "manager"));

    if (!userAdded) {
        cout << " Manager registration failed\n";
        return;
    }

    User* manager = userDAO.login(username, password);

    if (manager == nullptr) {
        cout << " Manager user was created, but could not login to get manager ID\n";
        return;
    }

    cout << "\n===== Restaurant Information =====\n";

    string restaurantName = readNonEmptyString(" Enter restaurant name: ");
    string address = readNonEmptyString("Enter restaurant address: ");

    int status = readZeroOne(" Enter restaurant status (1=active, 0=inactive): ");

    int prepTime = readPositiveInt(" Enter preparation time: ");

    string contact = readNonEmptyString(" Enter contact: ");
    string description = readNonEmptyString(" Enter description: ");

    Restaurant restaurant(
        0,
        restaurantName,
        address,
        status,
        prepTime,
        contact,
        description,
        manager->getId()
    );

    if (restaurantDAO.addRestaurant(restaurant)) {
        cout << " Manager and restaurant registered successfully.\n";
    } else {
        cout << " Manager registered, but restaurant registration failed.\n";
    }

    delete manager;
}

void registerCustomer(UserDAO& userDAO) {
    string name = readNonEmptyString("Enter name: ");
    string username = readNonEmptyString("Enter username: ");
    string password = readNonEmptyString("Enter password: ");

    if (userDAO.addUser(User(0, name, username, password, "customer")))
        cout << "Customer registered successfully.\n";
    else
        cout << " Registration failed\n";
}

void login(UserDAO& userDAO) {
    string username = readNonEmptyString("Enter username: ");
    string password = readNonEmptyString("Enter password: ");

    User* currentUser = userDAO.login(username, password);
    if (currentUser == nullptr) {
        cout << " Login failed\n";
        cout << " Invalid username or password\n";
        return;
    }

    if (currentUser->getRole() == "admin") adminPanel(currentUser);
    else if (currentUser->getRole() == "manager") managerPanel(currentUser);
    else if (currentUser->getRole() == "customer") customerPanel(currentUser);
    else cout << " Unknown role\n";

    delete currentUser;
}

void adminPanel(User* currentUser) {
    UserDAO userDAO;
    RestaurantDAO restaurantDAO;
    OrderDAO orderDAO;

    int choice;
    do {
        cout << "\n===== Admin Panel =====\n";
        cout << " Welcome, " << currentUser->getName() << "\n";
        cout << "1. Show All Users\n";
        cout << "2. Show All Restaurants\n";
        cout << "3. Add User\n";
        cout << "4. Delete User\n";
        cout << "5. Add Restaurant\n";
        cout << "6. Update Restaurant\n";
        cout << "7. Delete Restaurant\n";
        cout << "8. Reports\n";
        cout << "0. Logout\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1:
            showAllUsers(userDAO);
            pauseProgram();
            break;

        case 2:
            showAllRestaurants(restaurantDAO);
            pauseProgram();
            break;

        case 3: {
            adminAddUserMenu(userDAO);
    		break;
        }

        case 4: {
            adminDeleteUserMenu(userDAO);
    		break;
        }

        case 5: {
            adminAddRestaurantMenu(restaurantDAO);
    		break;
        }

        case 6: {
            adminUpdateRestaurantMenu(restaurantDAO);
    		break;
        }

        case 7: {
            adminDeleteRestaurantMenu(restaurantDAO);
    		break;
        }

        case 8:
            showReports(userDAO, orderDAO);
            break;

        case 0:
            cout << " Logging out...\n";
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
            break;
        }

    } while (choice != 0);
}

void adminAddUserMenu(UserDAO& userDAO) {
    int choice;

    do {
        cout << "\n===== Add User =====\n";
        cout << "1. Add User\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1: {
            string name = readNonEmptyString(" Enter name: ");
            string username = readNonEmptyString(" Enter username: ");
            string password = readNonEmptyString(" Enter password: ");
            string role = readNonEmptyString(" Enter role (customer/manager/admin): ");

            if (role != "customer" && role != "manager" && role != "admin") {
                cout << " Invalid role\n";
            } else if (userDAO.addUser(User(0, name, username, password, role))) {
                cout << " User added successfully\n";
            } else {
                cout << " Failed to add user\n";
            }

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void adminDeleteUserMenu(UserDAO& userDAO) {
    int choice;

    do {
        cout << "\n===== Delete User =====\n";
        cout << "1. Show All Users\n";
        cout << "2. Delete User\n";
        cout << "0. Back\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1:
            showAllUsers(userDAO);
            pauseProgram();
            break;

        case 2: {
            int id = readPositiveInt(" Enter user ID to delete: ");

            if (userDAO.deleteUser(id))
                cout << " User deleted successfully\n";
            else
                cout << " Failed to delete user (If the user is a manager, please delete the manager's restaurant first)\n";
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void adminAddRestaurantMenu(RestaurantDAO& restaurantDAO) {
    int choice;

    do {
        cout << "\n===== Add Restaurant =====\n";
        cout << "1. Add Restaurant\n";
        cout << "0. Back\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1: {
            string name = readNonEmptyString(" Enter restaurant name: ");
            string address = readNonEmptyString(" Enter restaurant address: ");

            int status = readZeroOne(" Enter status (1=active, 0=inactive): ");

            int prepTime = readPositiveInt(" Enter prep time: ");

            string contact = readNonEmptyString(" Enter contact: ");
            string description = readNonEmptyString(" Enter description: ");

            int managerId = readPositiveInt(" Enter manager ID: ");

            Restaurant r(0, name, address, status, prepTime, contact, description, managerId);

            if (restaurantDAO.addRestaurant(r))
                cout << " Restaurant added successfully\n";
            else
                cout << " Failed to add restaurant\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void adminUpdateRestaurantMenu(RestaurantDAO& restaurantDAO) {
    int choice;

    do {
        cout << "\n===== Update Restaurant =====\n";
        cout << "1. Show All Restaurants\n";
        cout << "2. Update Restaurant\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showAllRestaurants(restaurantDAO);
            pauseProgram();
            break;

        case 2: {
            showAllRestaurants(restaurantDAO);

            int id = readPositiveInt(" Enter restaurant ID to update: ");

            Restaurant r = restaurantDAO.getRestaurantById(id);

            if (r.getId() == 0) {
                cout << " Restaurant not found\n";
                pauseProgram();
                break;
            }

            r.setName(readNonEmptyString(" Enter new name: "));
            r.setAddress(readNonEmptyString(" Enter new address: "));

            r.setStatus(readZeroOne(" Enter new status: "));

            r.setPrepTime(readPositiveInt(" Enter new prep time: "));

            r.setContact(readNonEmptyString(" Enter new contact: "));
            r.setDescription(readNonEmptyString(" Enter new description: "));

            r.setManagerId(readPositiveInt(" Enter new manager ID: "));

            if (restaurantDAO.updateRestaurant(r))
                cout << " Restaurant updated successfully\n";
            else
                cout << " Failed to update restaurant\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void adminDeleteRestaurantMenu(RestaurantDAO& restaurantDAO) {
    int choice;

    do {
        cout << "\n===== Delete Restaurant =====\n";
        cout << "1. Show All Restaurants\n";
        cout << "2. Delete Restaurant\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showAllRestaurants(restaurantDAO);
            pauseProgram();
            break;

        case 2: {
            showAllRestaurants(restaurantDAO);

            int id = readPositiveInt(" Enter restaurant ID to delete: ");

            if (restaurantDAO.deleteRestaurant(id))
                cout << " Restaurant deleted successfully\n";
            else
                cout << " Failed to delete restaurant\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void showReports(UserDAO& userDAO, OrderDAO& orderDAO) {
    int choice;

    do {
        cout << "\n===== Admin Reports =====\n";
        cout << "1. Total Sales\n";
        cout << "2. Total Orders\n";
        cout << "3. Active Users\n";
        cout << "0. Back\n";

        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showTotalSales(orderDAO);
            pauseProgram();
            break;

        case 2:
            showTotalOrders(orderDAO);
            pauseProgram();
            break;

        case 3:
            showActiveUsersByOrders(userDAO, orderDAO);
            pauseProgram();
            break;

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
            break;
        }

    } while (choice != 0);
}

void showTotalSales(OrderDAO& orderDAO) {
    vector<Order> orders = orderDAO.getAllOrders();

    double totalSales = 0.0;

    for (size_t i = 0; i < orders.size(); i++) {
        totalSales += orders[i].getTotalPrice();
    }

    cout << "\n===== Total Sales Report =====\n";
    cout << "Total Sales: " << totalSales << endl;
}

void showTotalOrders(OrderDAO& orderDAO) {
    vector<Order> orders = orderDAO.getAllOrders();

    cout << "\n===== Total Orders Report =====\n";
    cout << "Total Orders: " << orders.size() << endl;
}

void showActiveUsersByOrders(UserDAO& userDAO, OrderDAO& orderDAO) {
    vector<User> users = userDAO.getAllUsers();
    vector<Order> orders = orderDAO.getAllOrders();

    map<int, int> userOrderCount;

    for (size_t i = 0; i < orders.size(); i++) {
        int customerId = orders[i].getCustomerId();
        userOrderCount[customerId]++;
    }

    int activeUsers = 0;

    cout << "\n===== Active Users Report =====\n";
    cout << "Active users are customers who have placed at least one order.\n\n";

    for (size_t i = 0; i < users.size(); i++) {
        int userId = users[i].getId();

        if (userOrderCount[userId] > 0) {
            activeUsers++;

            cout << "User ID: " << users[i].getId()
                 << " | Name: " << users[i].getName()
                 << " | Username: " << users[i].getUsername()
                 << " | Role: " << users[i].getRole()
                 << " | Orders: " << userOrderCount[userId]
                 << endl;
        }
    }

    cout << "\nTotal Active Users: " << activeUsers << endl;
}

void managerPanel(User* currentUser) {
    RestaurantDAO restaurantDAO;
    MenuItemDAO menuItemDAO;
    OrderDAO orderDAO;
    OrderItemDAO orderItemDAO;

    Restaurant myRestaurant = getManagerRestaurant(currentUser->getId());

    if (myRestaurant.getId() == 0) {
        cout << " You do not have an assigned restaurant\n";
        pauseProgram();
        return;
    }

    int choice;
    do {
        cout << "\n===== Manager Panel =====\n";
        cout << " Restaurant: " << myRestaurant.getName() << "\n";
        cout << "1. View My Menu Items\n";
        cout << "2. Add Menu Item\n";
        cout << "3. Update Menu Item\n";
        cout << "4. Delete Menu Item\n";
        cout << "5. View Orders\n";
        cout << "6. Update Restaurant Info\n";
        cout << "7. Update Order Status\n";
        cout << "0. Logout\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1:
            showMenuItemsByRestaurant(myRestaurant.getId());
            pauseProgram();
            break;

        case 2: {
            managerAddMenuItemMenu(menuItemDAO, myRestaurant);
    		break;
        }

        case 3: {
            managerUpdateMenuItemMenu(menuItemDAO, myRestaurant);
    		break;
        }

        case 4: {
            managerDeleteMenuItemMenu(menuItemDAO, myRestaurant);
    		break;
        }

        case 5: {
            vector<Order> orders = orderDAO.getOrdersByRestaurantId(myRestaurant.getId());

            if (orders.empty()) {
                cout << " No orders found\n";
            } else {
                for (size_t i = 0; i < orders.size(); i++) {
                    cout << "Order ID: " << orders[i].getId()
                         << " | Customer ID: " << orders[i].getCustomerId()
                         << " | Total: " << orders[i].getTotalPrice()
                         << " | Status: " << orders[i].getStatus()
                         << " | Date: " << orders[i].getOrderDate()
                         << endl;

                    vector<OrderItem> items = orderItemDAO.getItemsByOrderId(orders[i].getId());
                    for (size_t j = 0; j < items.size(); j++) {
                        cout << "   Item ID: " << items[j].getMenuItemId()
                             << " | Quantity: " << items[j].getQuantity()
                             << " | Price: " << items[j].getPrice()
                             << endl;
                    }
                }
            }
            pauseProgram();
            break;
        }
        
        case 6: {
            managerUpdateRestaurantInfoMenu(restaurantDAO, myRestaurant, currentUser);
    		break;
        }

        case 7: {
            managerUpdateOrderStatusMenu(orderDAO, myRestaurant);
    		break;
        }

        case 0:
            cout << " Logging out...\n";
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

Restaurant getManagerRestaurant(int managerId) {
    RestaurantDAO restaurantDAO;
    return restaurantDAO.getRestaurantByManagerId(managerId);
}

void managerAddMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant) {
    int choice;

    do {
        cout << "\n===== Add Menu Item =====\n";
        cout << "1. Add Menu Item\n";
        cout << "0. Back\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1: {
            MenuItem item;
            item.setRestaurantId(myRestaurant.getId());
            item.setName(readNonEmptyString(" Enter item name: "));
            item.setCategory(readNonEmptyString(" Enter category: "));

            item.setPrice(readPositiveDouble(" Enter price: "));

            item.setAvailable(readZeroOne(" Enter available (1/0): "));

            item.setDescription(readNonEmptyString(" Enter description: "));

            if (menuItemDAO.addMenuItem(item))
                cout << " Menu item added successfully\n";
            else
                cout << " Failed to add menu item\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void managerUpdateMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant) {
    int choice;

    do {
        cout << "\n===== Update Menu Item =====\n";
        cout << "1. View My Menu Items\n";
        cout << "2. Update Menu Item\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showMenuItemsByRestaurant(myRestaurant.getId());
            pauseProgram();
            break;

        case 2: {
            showMenuItemsByRestaurant(myRestaurant.getId());

            int id = readPositiveInt(" Enter menu item ID to update: ");

            MenuItem item = menuItemDAO.getMenuItemById(id);

            if (item.getId() == 0 || item.getRestaurantId() != myRestaurant.getId()) {
                cout << " Invalid item\n";
                pauseProgram();
                break;
            }

            item.setName(readNonEmptyString(" Enter new name: "));
            item.setCategory(readNonEmptyString(" Enter new category: "));

            item.setPrice(readPositiveDouble(" Enter new price: "));

            item.setAvailable(readPositiveInt(" Enter available (1/0): "));

            item.setDescription(readNonEmptyString(" Enter new description: "));

            if (menuItemDAO.updateMenuItem(item))
                cout << " Menu item updated successfully\n";
            else
                cout << " Failed to update menu item\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void managerDeleteMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant) {
    int choice;

    do {
        cout << "\n===== Delete Menu Item =====\n";
        cout << "1. View My Menu Items\n";
        cout << "2. Delete Menu Item\n";
        cout << "0. Back\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1:
            showMenuItemsByRestaurant(myRestaurant.getId());
            pauseProgram();
            break;

        case 2: {
            showMenuItemsByRestaurant(myRestaurant.getId());

            int id = readPositiveInt(" Enter menu item ID to delete: ");

            MenuItem item = menuItemDAO.getMenuItemById(id);

            if (item.getId() == 0 || item.getRestaurantId() != myRestaurant.getId()) {
                cout << " Invalid item\n";
                pauseProgram();
                break;
            }

            if (menuItemDAO.deleteMenuItem(id))
                cout << " Menu item deleted successfully\n";
            else
                cout << " Failed to delete menu item\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void managerUpdateRestaurantInfoMenu(RestaurantDAO& restaurantDAO, Restaurant& myRestaurant, User* currentUser) {
    int choice;

    do {
        cout << "\n===== Update Restaurant Info =====\n";
        cout << "1. Show Current Restaurant Info\n";
        cout << "2. Update Restaurant Info\n";
        cout << "0. Back\n";
        cout << " Enter your choice: ";
        choice = readInt();

        switch (choice) {
        case 1:
            cout << "\nCurrent Name: " << myRestaurant.getName() << endl;
            cout << "Current Address: " << myRestaurant.getAddress() << endl;
            cout << "Current Status: " << (myRestaurant.getStatus() ? "active" : "inactive") << endl;
            cout << "Current Prep Time: " << myRestaurant.getPrepTime() << endl;
            cout << "Current Contact: " << myRestaurant.getContact() << endl;
            cout << "Current Description: " << myRestaurant.getDescription() << endl;
            pauseProgram();
            break;

        case 2:
            cout << "\nCurrent Name: " << myRestaurant.getName() << endl;
            cout << "Current Address: " << myRestaurant.getAddress() << endl;
            cout << "Current Status: " << (myRestaurant.getStatus() ? "active" : "inactive") << endl;
            cout << "Current Prep Time: " << myRestaurant.getPrepTime() << endl;
            cout << "Current Contact: " << myRestaurant.getContact() << endl;
            cout << "Current Description: " << myRestaurant.getDescription() << endl;

            myRestaurant.setName(readNonEmptyString(" Enter new name: "));
            myRestaurant.setAddress(readNonEmptyString(" Enter new address: "));

            myRestaurant.setStatus(readPositiveInt (" Enter new status (1=active, 0=inactive): "));

            myRestaurant.setPrepTime(readPositiveInt(" Enter new preparation time: "));

            myRestaurant.setContact(readNonEmptyString(" Enter new contact: "));
            myRestaurant.setDescription(readNonEmptyString(" Enter new description: "));
            myRestaurant.setManagerId(currentUser->getId());

            if (restaurantDAO.updateRestaurant(myRestaurant)) {
                cout << " Restaurant info updated successfully\n";
                myRestaurant = restaurantDAO.getRestaurantByManagerId(currentUser->getId());
            } else {
                cout << " Failed to update restaurant info\n";
            }

            pauseProgram();
            break;

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void managerUpdateOrderStatusMenu(OrderDAO& orderDAO, Restaurant& myRestaurant) {
    int choice;

    do {
        cout << "\n===== Update Order Status =====\n";
        cout << "1. View Orders\n";
        cout << "2. Update Order Status\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1: {
            vector<Order> orders = orderDAO.getOrdersByRestaurantId(myRestaurant.getId());

            if (orders.empty()) {
                cout << " No orders found\n";
            } else {
                cout << "\n===== Orders =====\n";

                for (size_t i = 0; i < orders.size(); i++) {
                    cout << "Order ID: " << orders[i].getId()
                         << " | Customer ID: " << orders[i].getCustomerId()
                         << " | Total: " << orders[i].getTotalPrice()
                         << " | Status: " << orders[i].getStatus()
                         << " | Date: " << orders[i].getOrderDate()
                         << endl;
                }
            }

            pauseProgram();
            break;
        }

        case 2: {
            vector<Order> orders = orderDAO.getOrdersByRestaurantId(myRestaurant.getId());

            if (orders.empty()) {
                cout << " No orders found\n";
                pauseProgram();
                break;
            }

            cout << "\n===== Orders =====\n";

            for (size_t i = 0; i < orders.size(); i++) {
                cout << "Order ID: " << orders[i].getId()
                     << " | Customer ID: " << orders[i].getCustomerId()
                     << " | Total: " << orders[i].getTotalPrice()
                     << " | Status: " << orders[i].getStatus()
                     << " | Date: " << orders[i].getOrderDate()
                     << endl;
            }

            int orderId = readPositiveInt("\n Enter order ID to update: ");

            Order selectedOrder = orderDAO.getOrderById(orderId);

            if (selectedOrder.getId() == 0) {
                cout << " Order not found\n";
                pauseProgram();
                break;
            }

            if (selectedOrder.getRestaurantId() != myRestaurant.getId()) {
                cout << " You cannot update orders of another restaurant\n";
                pauseProgram();
                break;
            }

            cout << "\nSelect new status:\n";
            cout << "1. Preparing\n";
            cout << "2. Ready to Send\n";
            cout << "3. Delivered\n";
            cout << "0. Back\n";

            int statusChoice = readPositiveInt(" Enter your choice: ");
            string newStatus;

            if (statusChoice == 1) {
                newStatus = "Preparing";
            } else if (statusChoice == 2) {
                newStatus = "Ready to Send";
            } else if (statusChoice == 3) {
                newStatus = "Delivered";
            } else if (statusChoice == 0) {
                break;
            } else {
                cout << " Invalid status choice\n";
                pauseProgram();
                break;
            }

            if (orderDAO.updateOrderStatus(orderId, newStatus))
                cout << " Order status updated successfully\n";
            else
                cout << " Failed to update order status\n";

            pauseProgram();
            break;
        }

        case 0:
            break;

        default:
            cout << " Invalid choice\n";
            pauseProgram();
        }
    } while (choice != 0);
}

void customerPanel(User* currentUser) {
    UserDAO userDAO;
    MenuItemDAO menuItemDAO;
    OrderDAO orderDAO;
    OrderItemDAO orderItemDAO;
    RestaurantDAO restaurantDAO;

    vector<OrderItem> cart;
    int cartRestaurantId = 0;

    Customer* customerInfo = userDAO.getCustomerById(currentUser->getId());
    if (customerInfo != nullptr) {
        cout << "\n===== Membership Status =====\n";
        cout << " Level: " << customerInfo->getLevelName() << endl;
        cout << " Points: " << customerInfo->getPoints() << endl;

        int nextPoints = 0;
        if (customerInfo->getLevelName() == "Normal") {
            nextPoints = 100 - customerInfo->getPoints();
        } else if (customerInfo->getLevelName() == "Silver") {
            nextPoints = 250 - customerInfo->getPoints();
        } else if (customerInfo->getLevelName() == "Gold") {
            nextPoints = 500 - customerInfo->getPoints();
        } else {
            nextPoints = 0;
        }

        if (nextPoints > 0) {
            cout << " Points needed for next level: " << nextPoints << endl;
        } else {
            cout << " You are at the highest level." << endl;
        }

        cout << "=============================\n";
        delete customerInfo;
    }

    int choice;
    do {
        cout << "\n===== Customer Panel =====\n";
        cout << " Welcome, " << currentUser->getName() << "\n";
        cout << "1. View Restaurants\n";
        cout << "2. View Restaurant Menu\n";
        cout << "3. Shopping Cart\n";
        cout << "4. Finalize Order\n";
        cout << "5. View My Orders\n";
        cout << "0. Logout\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showAllRestaurants(restaurantDAO);
            pauseProgram();
            break;

        case 2: {
            showAllRestaurants(restaurantDAO);

            int restaurantId = readPositiveInt(" Enter restaurant ID: ");

            Restaurant restaurant = restaurantDAO.getRestaurantById(restaurantId);
            if (restaurant.getId() == 0) {
                cout << " Restaurant not found\n";
                pauseProgram();
                break;
            }

            showMenuItemsByRestaurant(restaurantId);
            pauseProgram();
            break;
        }

        case 3: {
            int cartChoice;

            do {
                cout << "\n===== Shopping Cart =====\n";

                Customer* customer = userDAO.getCustomerById(currentUser->getId());

                if (cart.empty()) {
                    cout << " Cart is empty.\n";
                    cout << " Preview Earned Points: 0\n";
                } else {
                    double total = calculateCartTotal(cart);
                    int previewPoints = calculateCartPreviewPoints(cart, customer);

                    cout << " Restaurant ID: " << cartRestaurantId << endl;
                    cout << " Items:\n";

                    for (size_t i = 0; i < cart.size(); i++) {
                        MenuItem item = menuItemDAO.getMenuItemById(cart[i].getMenuItemId());

                        double subtotal = cart[i].getPrice() * cart[i].getQuantity();

                        cout << " " << i + 1 << ". "
                             << "Item ID: " << cart[i].getMenuItemId()
                             << " | Name: " << item.getName()
                             << " | Quantity: " << cart[i].getQuantity()
                             << " | Unit Price: " << cart[i].getPrice()
                             << " | Subtotal: " << subtotal
                             << endl;
                    }

                    cout << "--------------------------------\n";
                    cout << " Cart Total Price: " << total << endl;
                    cout << " Preview Earned Points: " << previewPoints << endl;
                }

                delete customer;

                cout << "\nShopping Cart Menu:\n";
                cout << "1. Add item to cart\n";
                cout << "2. Decrease/remove item from cart\n";
                cout << "3. Clear cart\n";
                cout << "0. Back to customer panel\n";
                cartChoice = readPositiveInt(" Enter your choice: ");

                switch (cartChoice) {
                case 1: {
                    showAllRestaurants(restaurantDAO);

                    int restaurantId = readPositiveInt(" Enter restaurant ID: ");

                    Restaurant restaurant = restaurantDAO.getRestaurantById(restaurantId);
                    if (restaurant.getId() == 0) {
                        cout << " Restaurant not found\n";
                        pauseProgram();
                        break;
                    }

                    if (restaurant.getStatus() == 0) {
                        cout << " This restaurant is inactive.\n";
                        pauseProgram();
                        break;
                    }

                    if (!cart.empty() && cartRestaurantId != restaurantId) {
                        cout << " Your cart already contains items from another restaurant.\n";
                        cout << " Please finalize or clear your cart first.\n";
                        pauseProgram();
                        break;
                    }

                    vector<MenuItem> items = menuItemDAO.getMenuItemsByRestaurantId(restaurantId);
                    if (items.empty()) {
                        cout << " No items found\n";
                        pauseProgram();
                        break;
                    }

                    cout << "\n===== Available Items =====\n";

                    bool hasAvailableItem = false;

                    for (size_t i = 0; i < items.size(); i++) {
                        if (items[i].getAvailable() == 1) {
                            hasAvailableItem = true;

                            cout << "ID: " << items[i].getId()
                                 << " | Name: " << items[i].getName()
                                 << " | Price: " << items[i].getPrice()
                                 << endl;
                        }
                    }

                    if (!hasAvailableItem) {
                        cout << " No available items in this restaurant.\n";
                        pauseProgram();
                        break;
                    }

                    int itemId = readPositiveInt(" Enter item ID to add: ");

                    MenuItem item = menuItemDAO.getMenuItemById(itemId);

                    if (item.getId() == 0 ||
                        item.getRestaurantId() != restaurantId ||
                        item.getAvailable() != 1) {
                        cout << " Invalid item\n";
                        pauseProgram();
                        break;
                    }

                    int qty = readPositiveInt(" Enter quantity: ");

                    if (qty <= 0) {
                        cout << " Invalid quantity\n";
                        pauseProgram();
                        break;
                    }

                    if (cart.empty()) {
                        cartRestaurantId = restaurantId;
                    }

                    bool found = false;

                    for (size_t i = 0; i < cart.size(); i++) {
                        if (cart[i].getMenuItemId() == itemId) {
                            cart[i].setQuantity(cart[i].getQuantity() + qty);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        OrderItem oi;
                        oi.setOrderId(0);
                        oi.setMenuItemId(item.getId());
                        oi.setQuantity(qty);
                        oi.setPrice(item.getPrice());

                        cart.push_back(oi);
                    }

                    Customer* customer = userDAO.getCustomerById(currentUser->getId());
                    double total = calculateCartTotal(cart);
                    int previewPoints = calculateCartPreviewPoints(cart, customer);
                    int addedItemPoints = 0;

                    if (customer != NULL) {
                        addedItemPoints = static_cast<int>(
                            item.getPrice() * qty * customer->getPointMultiplier()
                        );
                    }

                    cout << " Item added to cart successfully.\n";
                    cout << " Added Item Preview Points: " << addedItemPoints << endl;
                    cout << " Current Cart Total Price: " << total << endl;
                    cout << " Preview Earned Points: " << previewPoints << endl;

                    delete customer;

                    pauseProgram();
                    break;
                }

                case 2: {
                    if (cart.empty()) {
                        cout << " Cart is empty.\n";
                        pauseProgram();
                        break;
                    }

                    cout << " Enter item ID to decrease/remove: ";
                    int itemId = readInt();

                    bool found = false;

                    for (size_t i = 0; i < cart.size(); i++) {
                        if (cart[i].getMenuItemId() == itemId) {
                            found = true;

                            cout << " Current quantity: " << cart[i].getQuantity() << endl;
                            cout << " Enter quantity to remove: ";
                            int removeQty = readInt();

                            if (removeQty <= 0) {
                                cout << " Invalid quantity.\n";
                                break;
                            }

                            if (removeQty >= cart[i].getQuantity()) {
                                cart.erase(cart.begin() + i);
                                cout << " Item removed from cart.\n";
                            } else {
                                cart[i].setQuantity(cart[i].getQuantity() - removeQty);
                                cout << " Item quantity decreased.\n";
                            }

                            if (cart.empty()) {
                                cartRestaurantId = 0;
                            }

                            break;
                        }
                    }

                    if (!found) {
                        cout << " Item not found in cart.\n";
                    }

                    Customer* customer = userDAO.getCustomerById(currentUser->getId());
                    double total = calculateCartTotal(cart);
                    int previewPoints = calculateCartPreviewPoints(cart, customer);

                    cout << " Current Cart Total Price: " << total << endl;
                    cout << " Preview Earned Points: " << previewPoints << endl;

                    delete customer;

                    pauseProgram();
                    break;
                }

                case 3: {
                    cart.clear();
                    cartRestaurantId = 0;

                    cout << " Cart cleared successfully.\n";
                    cout << " Current Cart Total Price: 0\n";
                    cout << " Preview Earned Points: 0\n";

                    pauseProgram();
                    break;
                }

                case 0:
                    break;

                default:
                    cout << " Invalid choice.\n";
                    pauseProgram();
                }

            } while (cartChoice != 0);

            break;
        }

        case 4: {
            if (cart.empty()) {
                cout << " Cart is empty. Please add items first.\n";
                pauseProgram();
                break;
            }

            if (cartRestaurantId == 0) {
                cout << " Invalid cart restaurant.\n";
                pauseProgram();
                break;
            }

            double basePrice = calculateCartTotal(cart);

            Customer* customer = userDAO.getCustomerById(currentUser->getId());

            double discountAmount = 0.0;
            double shippingCost = 0.0;
            double finalPrice = basePrice;
            int earnedPoints = 0;

            if (customer != nullptr) {
                discountAmount = basePrice * customer->getDiscountPercent();
                double discountedPrice = basePrice - discountAmount;
                shippingCost = customer->calculateShippingCost(10000, discountedPrice);
                finalPrice = discountedPrice + shippingCost;

                earnedPoints = static_cast<int>(
                    finalPrice * customer->getPointMultiplier()
                );
            }

            cout << "\n===== Final Order Preview =====\n";

            for (size_t i = 0; i < cart.size(); i++) {
                MenuItem item = menuItemDAO.getMenuItemById(cart[i].getMenuItemId());

                cout << "Item: " << item.getName()
                     << " | Quantity: " << cart[i].getQuantity()
                     << " | Unit Price: " << cart[i].getPrice()
                     << " | Subtotal: " << cart[i].getPrice() * cart[i].getQuantity()
                     << endl;
            }

            cout << "--------------------------------\n";
            cout << "Base Price: " << basePrice << endl;
            cout << "Discount Amount: " << discountAmount << endl;
            cout << "Shipping Cost: " << shippingCost << endl;
            cout << "Total Price: " << finalPrice << endl;

            if (customer != nullptr) {
                cout << "Estimated Earned Points: " << earnedPoints << endl;
                cout << "Current Loyalty Points: " << customer->getPoints() << endl;
                cout << "Membership Level: " << customer->getLevelName() << endl;
            }

            int confirm = readZeroOne("Finalize this order? (1 = Yes, 0 = No): ");

            if (confirm != 1) {
                cout << " Order finalization cancelled.\n";
                delete customer;
                pauseProgram();
                break;
            }

            string orderDate = readNonEmptyString("Enter order date (e.g. 2026-06-11 18:30): ");

            Order order;
            order.setCustomerId(currentUser->getId());
            order.setRestaurantId(cartRestaurantId);
            order.setTotalPrice(finalPrice);
            order.setStatus("Preparing");
            order.setOrderDate(orderDate);

            int orderId = orderDAO.addOrderAndReturnId(order);

            if (orderId == -1) {
                cout << " Failed to place order.\n";
                delete customer;
                pauseProgram();
                break;
            }

            bool ok = true;

            for (size_t i = 0; i < cart.size(); i++) {
                cart[i].setOrderId(orderId);

                if (!orderItemDAO.addOrderItem(cart[i])) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                orderDAO.applyCustomerLoyaltyAfterOrder(order);

                cout << " Order finalized successfully.\n";
                cout << " Order ID: " << orderId << endl;
                cout << " Final Paid Amount: " << finalPrice << endl;
                cout << " Points Earned: " << earnedPoints << endl;

                cart.clear();
                cartRestaurantId = 0;

                cout << " Cart is now empty.\n";
            } else {
                cout << " Order was created, but some items failed to save.\n";
                cout << " Loyalty points were not applied.\n";
                cout << " Cart was not cleared so you can try again or check the database.\n";
            }

            delete customer;
            pauseProgram();
            break;
        }

        case 5: {
            vector<Order> orders = orderDAO.getOrdersByCustomerId(currentUser->getId());

            if (orders.empty()) {
                cout << "You have no orders.\n";
            } else {
                cout << "\n===== My Orders =====\n";

                for (size_t i = 0; i < orders.size(); i++) {
                    cout << "Order ID: " << orders[i].getId()
                         << " | Restaurant ID: " << orders[i].getRestaurantId()
                         << " | Total: " << orders[i].getTotalPrice()
                         << " | Status: " << orders[i].getStatus()
                         << " | Date: " << orders[i].getOrderDate()
                         << endl;

                    vector<OrderItem> items = orderItemDAO.getItemsByOrderId(orders[i].getId());

                    for (size_t j = 0; j < items.size(); j++) {
                        MenuItem item = menuItemDAO.getMenuItemById(items[j].getMenuItemId());

                        cout << "   Item: " << item.getName()
                             << " | Item ID: " << items[j].getMenuItemId()
                             << " | Quantity: " << items[j].getQuantity()
                             << " | Unit Price: " << items[j].getPrice()
                             << " | Subtotal: " << items[j].getPrice() * items[j].getQuantity()
                             << endl;
                    }

                    cout << "--------------------------------\n";
                }
            }

            pauseProgram();
            break;
        }

        case 0:
            cout << "Logging out...\n";
            break;

        default:
            cout << "Invalid choice.\n";
            pauseProgram();
        }

    } while (choice != 0);
}

double calculateCartTotal(vector<OrderItem>& cart) {
    double total = 0.0;

    for (size_t i = 0; i < cart.size(); i++) {
        total += cart[i].getPrice() * cart[i].getQuantity();
    }

    return total;
}

int calculateCartPreviewPoints(vector<OrderItem>& cart, Customer* customer) {
    if (customer == NULL || cart.empty()) {
        return 0;
    }

    int previewPoints = 0;

    for (size_t i = 0; i < cart.size(); i++) {
        double subtotal = cart[i].getPrice() * cart[i].getQuantity();

        previewPoints += static_cast<int>(
            subtotal * customer->getPointMultiplier()
        );
    }

    return previewPoints;
}