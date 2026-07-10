#ifndef USERDAO_H
#define USERDAO_H

#include "User.h"
#include "Customer.h"
#include <vector>
#include <string>
#include <map>

class UserDAO {
public:
    bool addUser(User user);
    bool updateUser(User user);
    bool deleteUser(int id);

    User getUserById(int id);
    std::vector<User> getAllUsers();

    User* login(std::string username, std::string password);

    Customer* getCustomerById(int id);
    
    bool updateCustomerLoyalty(int customerId, int points, const std::string& level);

    struct MembershipHistoryRecord {
        int id;
        int userId;
        std::string userName;
        std::string oldLevel;
        std::string newLevel;
        int pointsAtChange;
        std::string changedAt;
    };

    std::map<std::string, int> getCustomerCountByLevel();
    std::vector<MembershipHistoryRecord> getMembershipHistory();
    std::vector<Customer*> getAllCustomersWithDetails();
    bool downgradeUsersByInactivity(int daysOfInactivity, int pointsPenalty);
};

#endif
