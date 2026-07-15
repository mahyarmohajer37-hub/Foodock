#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "DatabaseManager.h"
#include "UserDAO.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "OrderItemDAO.h"

#include "User.h"
#include "Customer.h"
#include "Restaurant.h"
#include "MenuItem.h"
#include "Order.h"
#include "OrderItem.h"
#include "MembershipLevel.h"
#include "MembershipFactory.h"
#include "CouponDAO.h"

using namespace std;

void clearInputBuffer();

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
void adminModifyUserLoyalty(UserDAO& userDAO);
void adminApplyInactivityDowngrade(UserDAO& userDAO);
void adminViewMembershipHistory(UserDAO& userDAO);
void showMembershipLevelStats(UserDAO& userDAO);

void managerPanel(User* currentUser);
Restaurant getManagerRestaurant(int managerId);
void managerAddMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
void managerUpdateMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
int readCategoryChoice(const string& prompt);
void managerDeleteMenuItemMenu(MenuItemDAO& menuItemDAO, Restaurant& myRestaurant);
void managerUpdateRestaurantInfoMenu(RestaurantDAO& restaurantDAO, Restaurant& myRestaurant, User* currentUser);
void managerUpdateOrderStatusMenu(OrderDAO& orderDAO, Restaurant& myRestaurant);

void customerPanel(User* currentUser);
double calculateCartTotal(vector<OrderItem>& cart);
int calculateCartPreviewPoints(vector<OrderItem>& cart, Customer* customer);
void initializeUserCoupons();
bool isNightOrder(Order& order);
vector<string> getCustomerTitles(vector<Order>& orders);
string getCurrentDateTime();

int main() {
    DatabaseManager* db = DatabaseManager::getInstance();
    if (db == nullptr) {
        cout << " Database initialization failed \n";
        return 1;
    }

    UserDAO userDAO;
    RestaurantDAO restaurantDAO;

    seedTestData(userDAO, restaurantDAO);
    initializeUserCoupons();

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

int readPositiveInt(const string& prompt) {
    string input;
    int value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << " Input cannot be empty. Please enter a non-negative number.\n";
            continue;
        }

        stringstream ss(input);
        if (ss >> value && ss.eof() && value >= 0) {
            return value;
        }

        cout << " Invalid input. Please enter a non-negative number.\n";
    }
}

double readPositiveDouble(const string& prompt) {
    string input;
    double value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << " Input cannot be empty. Please enter a non-negative number.\n";
            continue;
        }

        stringstream ss(input);
        if (ss >> value && ss.eof() && value >= 0) {
            return value;
        }

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
             << " | Special: " << (items[i].getIsSpecial() ? "yes" : "no")
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

    int status = readPositiveInt(" Enter restaurant status (1=active, 0=inactive): ");

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
        cout << "8. Modify User Points/Level\n";    
        cout << "9. Apply Inactivity Downgrade\n";   
        cout << "10. View Level Change Logs\n";      
        cout << "11. Reports\n";                   
        cout << "0. Logout\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showAllUsers(userDAO);
            pauseProgram();
            break;
        case 2:
            showAllRestaurants(restaurantDAO);
            pauseProgram();
            break;
        case 3:
            adminAddUserMenu(userDAO);
            break;
        case 4:
            adminDeleteUserMenu(userDAO);
            break;
        case 5:
            adminAddRestaurantMenu(restaurantDAO);
            break;
        case 6:
            adminUpdateRestaurantMenu(restaurantDAO);
            break;
        case 7:
            adminDeleteRestaurantMenu(restaurantDAO);
            break;
        case 8:
            adminModifyUserLoyalty(userDAO); 
            break;
        case 9:
            adminApplyInactivityDowngrade(userDAO); 
            break;
        case 10:
            adminViewMembershipHistory(userDAO); 
            break;
        case 11:
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
        choice = readPositiveInt(" Enter your choice: ");

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
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1: {
            string name = readNonEmptyString(" Enter restaurant name: ");
            string address = readNonEmptyString(" Enter restaurant address: ");

            int status = readPositiveInt(" Enter status (1=active, 0=inactive): ");

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

            r.setStatus(readPositiveInt(" Enter new status: "));

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
        cout << "\n===== Users Reports =====\n";
        cout << "1. Total Sales\n";
        cout << "2. Total Orders\n";
        cout << "3. Active Users\n";
        cout << "4. User Membership Level Statistics\n";
        cout << "0. Back\n";
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1:
            showTotalSales(orderDAO);
            break;
        case 2:
            showTotalOrders(orderDAO);
            break;
        case 3:
            showActiveUsersByOrders(userDAO, orderDAO);
            break;
        case 4:
            showMembershipLevelStats(userDAO);
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

void adminModifyUserLoyalty(UserDAO& userDAO) {
    cout << "\n===== Modify Customer Loyalty (Admin) =====\n";
    int customerId = readPositiveInt("Enter Customer User ID: ");

    Customer* customer = userDAO.getCustomerById(customerId);
    if (customer == NULL) {
        cout << " [Error] Customer not found with ID " << customerId << ".\n";
        pauseProgram();
        return;
    }

    cout << "\nCurrent details for " << customer->getName() << ":\n";
    cout << " - Current Points: " << customer->getPoints() << "\n";
    cout << " - Current Level: " << customer->getLevelName() << "\n";

    int newPoints = readPositiveInt("\nEnter new loyalty points: ");
    if (newPoints < 0) {
        cout << " [Error] Points cannot be negative!\n";
        delete customer;
        pauseProgram();
        return;
    }

    std::string oldLevel = customer->getLevelName();
    
    customer->setPoints(newPoints);
    std::string newLevel = customer->getLevelName();

    if (userDAO.updateCustomerLoyalty(customer->getId(), customer->getPoints(), newLevel)) {
        cout << " [Success] Loyalty points updated in database.\n";
        
        if (oldLevel != newLevel) {
            sqlite3* db = DatabaseManager::getInstance()->getDB();
            std::string histSql = "INSERT INTO MembershipHistory (user_id, old_level, new_level, points_at_change) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* histStmt;

            if (sqlite3_prepare_v2(db, histSql.c_str(), -1, &histStmt, NULL) == SQLITE_OK) {
                sqlite3_bind_int(histStmt, 1, customer->getId());
                sqlite3_bind_text(histStmt, 2, oldLevel.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(histStmt, 3, newLevel.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(histStmt, 4, customer->getPoints());

                sqlite3_step(histStmt);
                sqlite3_finalize(histStmt);
            }
            cout << " [Info] Level changed from " << oldLevel << " to " << newLevel << ".\n";
        }
    } else {
        cout << " [Error] Failed to update customer loyalty in database.\n";
    }

    delete customer;
    pauseProgram();
}

void adminApplyInactivityDowngrade(UserDAO& userDAO) {
    cout << "\n===== Apply Inactivity Downgrade =====\n";
    int days = readPositiveInt("Enter days of inactivity threshold (e.g., 30): ");
    int penalty = readPositiveInt("Enter loyalty points penalty for inactive users: ");

    if (days <= 0 || penalty < 0) {
        cout << " [Error] Invalid inputs!\n";
        pauseProgram();
        return;
    }

    cout << " Processing inactivity check and downgrades... Please wait.\n";
    if (userDAO.downgradeUsersByInactivity(days, penalty)) {
        cout << " [Success] Inactivity check completed successfully. Inactive users downgraded.\n";
    } else {
        cout << " [Error] Something went wrong while applying downgrades.\n";
    }
    pauseProgram();
}

void adminViewMembershipHistory(UserDAO& userDAO) {
    cout << "\n===== Membership Level Change Logs =====\n";
    std::vector<UserDAO::MembershipHistoryRecord> history = userDAO.getMembershipHistory();

    if (history.empty()) {
        cout << " No level change history found.\n";
    } else {
        printf("%-5s | %-8s | %-15s | %-12s | %-12s | %-8s | %-19s\n",
               "LogID", "UserID", "Customer Name", "Old Level", "New Level", "Points", "Changed At");
        cout << std::string(90, '-') << "\n";

        for (size_t i = 0; i < history.size(); i++) {
            printf("%-5d | %-8d | %-15s | %-12s | %-12s | %-8d | %-19s\n",
                   history[i].id,
                   history[i].userId,
                   history[i].userName.c_str(),
                   history[i].oldLevel.c_str(),
                   history[i].newLevel.c_str(),
                   history[i].pointsAtChange,
                   history[i].changedAt.c_str());
        }
    }
    pauseProgram();
}

void showMembershipLevelStats(UserDAO& userDAO) {
    cout << "\n===== User Count by Membership Level =====\n";
    std::map<std::string, int> stats = userDAO.getCustomerCountByLevel();

    cout << " - VIP:      " << stats["VIP"] << " users\n";
    cout << " - Gold:     " << stats["Gold"] << " users\n";
    cout << " - Silver:   " << stats["Silver"] << " users\n";
    cout << " - Normal:   " << stats["Normal"] << " users\n";
    cout << "==========================================\n";
    pauseProgram();
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
        choice = readPositiveInt(" Enter your choice: ");

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
            UserDAO userDAO;

            if (orders.empty()) {
                cout << " No orders found\n";
            } else {
                for (size_t i = 0; i < orders.size(); i++) {
                    Customer* customer = userDAO.getCustomerById(orders[i].getCustomerId());
                    string customerName = "Unknown";
                    string customerLevel = "Normal";

                    if (customer != NULL) {
                        customerName = customer->getName();
                        customerLevel = customer->getLevelName();
                    }

                    cout << "Order ID: " << orders[i].getId()
                         << " | Customer: " << customerName;

                    if (customerLevel == "VIP") {
                        cout << " | Top Priority";
                    }

                    vector<Order> custOrders = orderDAO.getOrdersByCustomerId(orders[i].getCustomerId());
                    vector<string> titles = getCustomerTitles(custOrders);

                    if (!titles.empty()) {
                        cout << " | Titles: ";
                        for (size_t t = 0; t < titles.size(); ++t) {
                            cout << titles[t] << (t == titles.size() - 1 ? "" : ", ");
                        }
                    }

                    cout << " | Total: " << orders[i].getTotalPrice()
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

                    delete customer;
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
        choice = readPositiveInt(" Enter your choice: ");

        switch (choice) {
        case 1: {
            MenuItem item;
            item.setRestaurantId(myRestaurant.getId());
            item.setName(readNonEmptyString(" Enter item name: "));

            cout << " Select category:\n";
            cout << " 1. Appetizer\n";
            cout << " 2. Main Course\n";
            cout << " 3. Dessert\n";
            cout << " 4. Drink\n";

            int categoryChoice = readPositiveInt(" Enter your choice (1-4): ");

            if (categoryChoice == 1) {
                item.setCategory("appetizer");
                item.setPrepTime(readPositiveInt(" Enter preparation time: "));
                item.setWeight(readPositiveDouble(" Enter weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 2) {
                item.setCategory("main course");
                item.setPrepTime(readPositiveInt(" Enter preparation time: "));
                item.setWeight(readPositiveDouble(" Enter weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 3) {
                item.setCategory("dessert");
                item.setPrepTime(readPositiveInt(" Enter preparation time: "));
                item.setWeight(readPositiveDouble(" Enter weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 4) {
                item.setCategory("drink");
                item.setPrepTime(readPositiveInt(" Enter preparation time: "));
                item.setVolume(readPositiveDouble(" Enter volume: "));
                item.setWeight(0);
            }

            item.setPrice(readPositiveDouble(" Enter price: "));
            item.setAvailable(readPositiveInt(" Enter available (1/0): "));
            item.setDescription(readNonEmptyString(" Enter description: "));
            item.setIsSpecial(readPositiveInt(" Enter is special (1/0): "));

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

            cout << " Select new category:\n";
            cout << " 1. Appetizer\n";
            cout << " 2. Main Course\n";
            cout << " 3. Dessert\n";
            cout << " 4. Drink\n";

            int categoryChoice = readPositiveInt(" Enter your choice (1-4): ");

            if (categoryChoice == 1) {
                item.setCategory("appetizer");
                item.setPrepTime(readPositiveInt(" Enter new preparation time: "));
                item.setWeight(readPositiveDouble(" Enter new weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 2) {
                item.setCategory("main course");
                item.setPrepTime(readPositiveInt(" Enter new preparation time: "));
                item.setWeight(readPositiveDouble(" Enter new weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 3) {
                item.setCategory("dessert");
                item.setPrepTime(readPositiveInt(" Enter new preparation time: "));
                item.setWeight(readPositiveDouble(" Enter new weight: "));
                item.setVolume(0);
            }
            else if (categoryChoice == 4) {
                item.setCategory("drink");
                item.setPrepTime(readPositiveInt(" Enter new preparation time: "));
                item.setVolume(readPositiveDouble(" Enter new volume: "));
                item.setWeight(0);
            }

            item.setPrice(readPositiveDouble(" Enter new price: "));
            item.setAvailable(readPositiveInt(" Enter available (1/0): "));
            item.setDescription(readNonEmptyString(" Enter new description: "));
            item.setIsSpecial(readPositiveInt(" Enter new is special (1/0): "));

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
        choice = readPositiveInt(" Enter your choice: ");

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
        choice = readPositiveInt(" Enter your choice: ");

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
    UserDAO userDAO;
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
                    Customer* customer =
                        userDAO.getCustomerById(orders[i].getCustomerId());

                    string customerName = "Unknown";
                    string customerLevel = "Normal";

                    if (customer != NULL) {
                        customerName = customer->getName();
                        customerLevel = customer->getLevelName();
                    }

                    cout << "Order ID: " << orders[i].getId()
                         << " | Customer: " << customerName;

                    if (customerLevel == "VIP") {
                        cout << " | Top Priority";
                    }

                    cout << " | Total: " << orders[i].getTotalPrice()
                         << " | Status: " << orders[i].getStatus()
                         << " | Date: " << orders[i].getOrderDate()
                         << endl;

                    delete customer;
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
                Customer* customer =
                    userDAO.getCustomerById(orders[i].getCustomerId());

                string customerName = "Unknown";
                string customerLevel = "Normal";

                if (customer != NULL) {
                    customerName = customer->getName();
                    customerLevel = customer->getLevelName();
                }

                cout << "Order ID: " << orders[i].getId()
                     << " | Customer: " << customerName;

                if (customerLevel == "VIP") {
                    cout << " | Top Priority";
                }

                cout << " | Total: " << orders[i].getTotalPrice()
                     << " | Status: " << orders[i].getStatus()
                     << " | Date: " << orders[i].getOrderDate()
                     << endl;

                delete customer;
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
    CouponDAO couponDAO;

    vector<OrderItem> cart;
    int cartRestaurantId = 0;

    Customer* customerInfo = userDAO.getCustomerById(currentUser->getId());
    if (customerInfo != nullptr) {
        cout << "\n===== Membership Status =====\n";
        cout << " Level: " << customerInfo->getLevelName() << endl;
        cout << " Points: " << customerInfo->getPoints() << endl;
        int activeCoupons = couponDAO.getUnusedCouponsCountByUserId(currentUser->getId());
        cout << " Active Coupons: " << activeCoupons << endl;

        int nextPoints = 0;
        if (customerInfo->getLevelName() == "Normal") {
            nextPoints = 100 - customerInfo->getPoints();
        } else if (customerInfo->getLevelName() == "Silver") {
            nextPoints = 300 - customerInfo->getPoints();
        } else if (customerInfo->getLevelName() == "Gold") {
            nextPoints = 700 - customerInfo->getPoints();
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

            Customer* customer = userDAO.getCustomerById(currentUser->getId());
            vector<MenuItem> items =
                menuItemDAO.getMenuItemsByRestaurantId(restaurantId);

            if (items.empty()) {
                cout << " No menu items found\n";
            } else {
                bool canSeeSpecial = false;

                if (customer != NULL) {
                    string level = customer->getLevelName();
                    if (level == "Gold" || level == "VIP") {
                        canSeeSpecial = true;
                    }
                }

                cout << "\n===== Menu Items =====\n";
                for (size_t i = 0; i < items.size(); i++) {
                    if (items[i].getIsSpecial() == 1 && !canSeeSpecial) {
                        continue;
                    }

                    cout << " ID: " << items[i].getId()
                         << " | Name: " << items[i].getName()
                         << " | Category: " << items[i].getCategory()
                         << " | Price: " << items[i].getPrice()
                         << " | Prep Time: " << items[i].getPrepTime()
                         << " | Weight: " << items[i].getWeight()
                         << " | Volume: " << items[i].getVolume()
                         << " | Available: "
                         << (items[i].getAvailable() ? "yes" : "no")
                         << " | Description: "
                         << items[i].getDescription();

                    if (items[i].getIsSpecial() == 1) {
                        cout << " | Special: yes";
                    }

                    cout << endl;
                }
            }

            delete customer;
            pauseProgram();
            break;
        }

        case 3: {
            int cartChoice;

            do {
                cout << "\n===== Shopping Cart =====\n";

                Customer* customer =
                    userDAO.getCustomerById(currentUser->getId());

                if (cart.empty()) {
                    cout << " Cart is empty.\n";
                    cout << " Preview Earned Points: 0\n";
                } else {
                    double total = calculateCartTotal(cart);
                    int previewPoints =
                        calculateCartPreviewPoints(cart, customer);

                    cout << " Restaurant ID: " << cartRestaurantId << endl;
                    cout << " Items:\n";

                    for (size_t i = 0; i < cart.size(); i++) {
                        MenuItem item =
                            menuItemDAO.getMenuItemById(
                                cart[i].getMenuItemId());

                        double subtotal =
                            cart[i].getPrice() * cart[i].getQuantity();

                        cout << " " << i + 1 << ". "
                             << "Item ID: "
                             << cart[i].getMenuItemId()
                             << " | Name: " << item.getName()
                             << " | Quantity: "
                             << cart[i].getQuantity()
                             << " | Unit Price: "
                             << cart[i].getPrice()
                             << " | Subtotal: " << subtotal
                             << endl;
                    }

                    cout << "--------------------------------\n";
                    cout << " Cart Total Price: " << total << endl;
                    cout << " Preview Earned Points: "
                         << previewPoints << endl;
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

                    int restaurantId =
                        readPositiveInt(" Enter restaurant ID: ");

                    Restaurant restaurant =
                        restaurantDAO.getRestaurantById(restaurantId);

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

                    if (!cart.empty() &&
                        cartRestaurantId != restaurantId) {
                        cout << " Your cart already contains items "
                                "from another restaurant.\n";
                        cout << " Please finalize or clear your cart first.\n";
                        pauseProgram();
                        break;
                    }

                    Customer* customer =
                        userDAO.getCustomerById(currentUser->getId());

                    bool canSeeSpecial = false;

                    if (customer != NULL) {
                        string level = customer->getLevelName();
                        if (level == "Gold" || level == "VIP") {
                            canSeeSpecial = true;
                        }
                    }

                    vector<MenuItem> items =
                        menuItemDAO.getMenuItemsByRestaurantId(restaurantId);

                    if (items.empty()) {
                        cout << " No items found\n";
                        delete customer;
                        pauseProgram();
                        break;
                    }

                    cout << "\n===== Available Items =====\n";

                    bool hasAvailableItem = false;

                    for (size_t i = 0; i < items.size(); i++) {
                        if (items[i].getAvailable() == 1) {
                            if (items[i].getIsSpecial() == 1 &&
                                !canSeeSpecial) {
                                continue;
                            }

                            hasAvailableItem = true;

                            cout << "ID: " << items[i].getId()
                                 << " | Name: " << items[i].getName()
                                 << " | Price: " << items[i].getPrice();

                            if (items[i].getIsSpecial() == 1) {
                                cout << " | Special: yes";
                            }

                            cout << endl;
                        }
                    }

                    if (!hasAvailableItem) {
                        cout << " No available items in this restaurant.\n";
                        delete customer;
                        pauseProgram();
                        break;
                    }

                    int itemId =
                        readPositiveInt(" Enter item ID to add: ");

                    MenuItem item =
                        menuItemDAO.getMenuItemById(itemId);

                    if (item.getId() == 0 ||
                        item.getRestaurantId() != restaurantId ||
                        item.getAvailable() != 1) {
                        cout << " Invalid item\n";
                        delete customer;
                        pauseProgram();
                        break;
                    }

                    if (item.getIsSpecial() == 1 && !canSeeSpecial) {
                        cout << " This item is special and only available "
                                "for Gold/VIP customers.\n";
                        delete customer;
                        pauseProgram();
                        break;
                    }

                    int qty = readPositiveInt(" Enter quantity: ");

                    if (qty <= 0) {
                        cout << " Invalid quantity\n";
                        delete customer;
                        pauseProgram();
                        break;
                    }

                    if (cart.empty()) {
                        cartRestaurantId = restaurantId;
                    }

                    bool found = false;

                    for (size_t i = 0; i < cart.size(); i++) {
                        if (cart[i].getMenuItemId() == itemId) {
                            cart[i].setQuantity(
                                cart[i].getQuantity() + qty);
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

                    double total = calculateCartTotal(cart);
                    int previewPoints =
                        calculateCartPreviewPoints(cart, customer);
                    int addedItemPoints = 0;

                    if (customer != NULL) {
                        addedItemPoints = static_cast<int>(
                            item.getPrice() * qty *
                            customer->getPointMultiplier()
                        );
                    }

                    cout << " Item added to cart successfully.\n";
                    cout << " Added Item Preview Points: "
                         << addedItemPoints << endl;
                    cout << " Current Cart Total Price: "
                         << total << endl;
                    cout << " Preview Earned Points: "
                         << previewPoints << endl;

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

                    int itemId = readPositiveInt(" Enter item ID to decrease/remove: ");

                    bool found = false;

                    for (size_t i = 0; i < cart.size(); i++) {
                        if (cart[i].getMenuItemId() == itemId) {
                            found = true;

                            cout << " Current quantity: "
                                 << cart[i].getQuantity() << endl;
                            int removeQty = readPositiveInt(" Enter quantity to remove: ");

                            if (removeQty <= 0) {
                                cout << " Invalid quantity.\n";
                                break;
                            }

                            if (removeQty >= cart[i].getQuantity()) {
                                cart.erase(cart.begin() + i);
                                cout << " Item removed from cart.\n";
                            } else {
                                cart[i].setQuantity(
                                    cart[i].getQuantity() - removeQty);
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

                    Customer* customer =
                        userDAO.getCustomerById(currentUser->getId());

                    double total = calculateCartTotal(cart);
                    int previewPoints =
                        calculateCartPreviewPoints(cart, customer);

                    cout << " Current Cart Total Price: "
                         << total << endl;
                    cout << " Preview Earned Points: "
                         << previewPoints << endl;

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

            Customer* customer =
                userDAO.getCustomerById(currentUser->getId());

            double membershipDiscountAmount = 0.0;
            double couponDiscountAmount = 0.0;
            double shippingCost = 0.0;
            double finalPrice = basePrice;
            int earnedPoints = 0;

            Coupon coupon;
            bool hasCoupon = false;
            string couponCode = "";
            double couponPercent = 0.0;

            if (customer != nullptr) {
                membershipDiscountAmount =
                    basePrice * customer->getDiscountPercent();

                double afterMembershipDiscount =
                    basePrice - membershipDiscountAmount;

                coupon = couponDAO.getUnusedCouponByUserId(currentUser->getId());
                if (coupon.getId() != 0) {
                    cout << "\n You have an active coupon (" << coupon.getDiscountPercent() << "% OFF).";
                    int useChoice = readPositiveInt(" Use it for this order? (1 = Yes, 0 = No): ");
                    
                    if (useChoice == 1) {
                        hasCoupon = true;
                        couponCode = coupon.getCode();
                        couponPercent = coupon.getDiscountPercent();
                        couponDiscountAmount = afterMembershipDiscount * (couponPercent / 100.0);
                    }
                }

                double afterCouponDiscount =
                    afterMembershipDiscount -
                    couponDiscountAmount;

                shippingCost =
                    customer->calculateShippingCost(
                        50, afterCouponDiscount);

                finalPrice =
                    afterCouponDiscount + shippingCost;

                earnedPoints = static_cast<int>(
                    finalPrice * customer->getPointMultiplier()
                );
            }

            cout << "\n===== Final Order Preview =====\n";

            for (size_t i = 0; i < cart.size(); i++) {
                MenuItem item =
                    menuItemDAO.getMenuItemById(
                        cart[i].getMenuItemId());

                cout << "Item: " << item.getName()
                     << " | Quantity: " << cart[i].getQuantity()
                     << " | Unit Price: " << item.getPrice()
                     << " | Subtotal: "
                     << cart[i].getPrice() *
                        cart[i].getQuantity()
                     << endl;
            }

            cout << "--------------------------------\n";
            cout << "Base Price: " << basePrice << endl;
            cout << "Membership Discount Amount: "
                 << membershipDiscountAmount << endl;
            cout << "Coupon Discount Amount: "
                 << couponDiscountAmount << endl;
            cout << "Shipping Cost: " << shippingCost << endl;
            cout << "Total Price: " << finalPrice << endl;

            if (hasCoupon) {
                cout << "Coupon Code: " << couponCode << endl;
                cout << "Coupon Percent: "
                     << couponPercent << "%" << endl;
            }

            if (customer != nullptr) {
                cout << "Estimated Earned Points: "
                     << earnedPoints << endl;
                cout << "Current Loyalty Points: "
                     << customer->getPoints() << endl;
                cout << "Membership Level: "
                     << customer->getLevelName() << endl;
            }

            int confirm =
                readPositiveInt(
                    "Finalize this order? (1 = Yes, 0 = No): ");

            if (confirm != 1) {
                cout << " Order finalization cancelled.\n";
                delete customer;
                pauseProgram();
                break;
            }

            string orderDate = getCurrentDateTime();

            Order order;
            order.setCustomerId(currentUser->getId());
            order.setRestaurantId(cartRestaurantId);
            order.setTotalPrice(finalPrice);
            order.setStatus("Preparing");
            order.setOrderDate(orderDate);

            int orderId =
                orderDAO.addOrderAndReturnId(order);

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
                order.setId(orderId);
                orderDAO.applyCustomerLoyaltyAfterOrder(order);

                if (hasCoupon) {
                    couponDAO.markCouponUsed(coupon.getId());
                }

                cout << " Order finalized successfully.\n";
                cout << " Order ID: " << orderId << endl;
                cout << " Final Paid Amount: "
                     << finalPrice << endl;
                cout << " Points Earned: "
                     << earnedPoints << endl;

                cart.clear();
                cartRestaurantId = 0;

                cout << " Cart is now empty.\n";
            } else {
                cout << " Order was created, but some items "
                        "failed to save.\n";
                cout << " Loyalty points were not applied.\n";
                cout << " Cart was not cleared so you can try again "
                        "or check the database.\n";
            }

            delete customer;
            pauseProgram();
            break;
        }

                case 5: {
            vector<Order> orders =
                orderDAO.getOrdersByCustomerId(
                    currentUser->getId());

            if (orders.empty()) {
                cout << "You have no orders.\n";
            } else {
                cout << "\n===== My Orders =====\n";
                vector<string> titles = getCustomerTitles(orders);

                for (size_t i = 0; i < orders.size(); i++) {
                    cout << "Order ID: "
                         << orders[i].getId()
                         << " | Restaurant ID: "
                         << orders[i].getRestaurantId()
                         << " | Total: "
                         << orders[i].getTotalPrice()
                         << " | Status: "
                         << orders[i].getStatus()
                         << " | Date: "
                         << orders[i].getOrderDate()
                         << endl;

                    if (!titles.empty()) {
                        cout << " Titles: ";
                        for (size_t t = 0; t < titles.size(); ++t) {
                            cout << titles[t] << (t == titles.size() - 1 ? "" : ", ");
                        }
                        cout << endl;
                    }

                    vector<OrderItem> items =
                        orderItemDAO.getItemsByOrderId(
                            orders[i].getId());

                    for (size_t j = 0; j < items.size(); j++) {
                        MenuItem item =
                            menuItemDAO.getMenuItemById(
                                items[j].getMenuItemId());

                        cout << "   Item: "
                             << item.getName()
                             << " | Item ID: "
                             << items[j].getMenuItemId()
                             << " | Quantity: "
                             << items[j].getQuantity()
                             << " | Unit Price: "
                             << items[j].getPrice()
                             << " | Subtotal: "
                             << items[j].getPrice() *
                                items[j].getQuantity()
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

void initializeUserCoupons() {
    UserDAO userDAO;
    CouponDAO couponDAO;
    
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    const char* sql =
        "SELECT id, level_current FROM Users WHERE role = 'customer';";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);

            const unsigned char* levelText = sqlite3_column_text(stmt, 1);
            string level = levelText
                ? reinterpret_cast<const char*>(levelText)
                : "";

            int requiredCoupons = 0;

            if (level == "Silver") {
                requiredCoupons = 1;
            } else if (level == "Gold") {
                requiredCoupons = 1;
            } else if (level == "VIP" || level == "Vip") {
                requiredCoupons = 3;
            }

            int activeCoupons =
                couponDAO.getUnusedCouponsCountByUserId(userId);

            for (int i = activeCoupons; i < requiredCoupons; i++) {
                Coupon newCoupon;
                newCoupon.setUserId(userId);
                newCoupon.setCode(
                    "DISCOUNT_" + to_string(userId) + "_" +
                    to_string(i + 1) + "_" +
                    to_string(rand() % 900 + 100)
                );
                newCoupon.setDiscountPercent(20.0);
                newCoupon.setIssuedAt("2026-07-14 00:00:00");
                newCoupon.setExpiresAt("2027-12-31 23:59:59");
                newCoupon.setIsUsed(false);
                
                couponDAO.addCoupon(newCoupon);
            }
        }

        sqlite3_finalize(stmt);
    }
}

bool isNightOrder(Order& order) {
    const std::string& orderDate = order.getOrderDate();
    size_t spacePos = orderDate.find(' ');
    if (spacePos == std::string::npos || orderDate.length() < spacePos + 3) {
        return false;
    }

    try {
        int hour = std::stoi(orderDate.substr(spacePos + 1, 2));
        return (hour >= 23 || hour < 3);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

vector<string> getCustomerTitles(vector<Order>& orders) {
    vector<string> titles;

    if (orders.size() >= 3) {
        titles.push_back("Frequent Buyer");
    }

    bool foundNightOrder = false;
    for (size_t i = 0; i < orders.size(); i++) {
        if (isNightOrder(orders[i])) {
            foundNightOrder = true;
            break;
        }
    }

    if (foundNightOrder) {
        titles.push_back("Night Customer");
    }

    return titles;
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d %H:%M");
    return ss.str();
}