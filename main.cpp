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