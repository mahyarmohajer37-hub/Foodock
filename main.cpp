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