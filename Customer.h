#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "MembershipLevel.h"
#include "MembershipFactory.h"
#include <iostream>
#include <string>

class Customer : public User {
private:
    int points;
    MembershipLevel* membershipLevel;

public:
    inline Customer(int id = 0,
                    std::string name = "",
                    std::string username = "",
                    std::string password = "")
        : User(id, name, username, password, "customer"),
          points(0),
          membershipLevel(new NormalLevel()) 
    {
    }

    inline Customer(const Customer& other)
        : User(other), points(other.points), membershipLevel(NULL) 
    {
        if (other.membershipLevel != NULL) {
            membershipLevel = other.membershipLevel->clone();
        } else {
            membershipLevel = new NormalLevel();
        }
    }

    inline Customer& operator=(const Customer& other) {
        if (this != &other) {
            User::operator=(other); 
            points = other.points;

            delete membershipLevel; 
            if (other.membershipLevel != NULL) {
                membershipLevel = other.membershipLevel->clone();
            } else {
                membershipLevel = new NormalLevel();
            }
        }
        return *this;
    }

    inline virtual ~Customer() {
        delete membershipLevel;
        membershipLevel = NULL;
    }

    inline void displayMenu() const override {
        std::cout << "\n========== Customer Menu ==========\n";
        std::cout << "Logged in as: " << getName() << "\n";
        std::cout << "Membership Level: " << getLevelName() << "\n";
        std::cout << "Loyalty Points: " << points << "\n";
        
        int nextPoints = membershipLevel->getRequiredPointsForNextLevel();
        if (nextPoints > 0) {
            std::cout << "Points needed for next level: " << (nextPoints - points) << "\n";
        } else {
            std::cout << "You are at the maximum level!\n";
        }
        std::cout << "-----------------------------------\n";
        std::cout << "1. View Active Restaurants\n";
        std::cout << "2. View Menu of Restaurant\n";
        std::cout << "3. Add Item to Cart\n";
        std::cout << "4. View Cart\n";
        std::cout << "5. Finalize Order\n";
        std::cout << "6. View My Orders\n";
        std::cout << "0. Logout\n";
        std::cout << "===================================\n";
    }

    inline int getPoints() const { return points; }
    inline void setPoints(int p) { 
        points = (p < 0) ? 0 : p; 
        updateMembershipByPoints(); 
    }
    inline void addPoints(int p) { 
        if (p > 0) {
            points += p;
            updateMembershipByPoints();
        }
    }

    inline std::string getLevelName() const {
        return membershipLevel ? membershipLevel->getLevelName() : "Normal";
    }

    inline double getDiscountPercent() const {
        return membershipLevel ? membershipLevel->getDiscountPercent() : 0.0;
    }

    inline double calculateShippingCost(double baseCost, double total) const {
        return membershipLevel ? membershipLevel->calculateShippingCost(baseCost, total) : baseCost;
    }

    inline double getPointMultiplier() const {
        return membershipLevel ? membershipLevel->getPointMultiplier() : 1.0;
    }

    inline void setMembershipLevel(MembershipLevel* newLevel) {
        if (newLevel != NULL) {
            delete membershipLevel;
            membershipLevel = newLevel;
        }
    }

private:
    inline void updateMembershipByPoints() {
        MembershipLevel* newLevel = MembershipFactory::determineLevelByPoints(points);
        if (newLevel->getLevelName() != membershipLevel->getLevelName()) {
            delete membershipLevel;
            membershipLevel = newLevel;
        } else {
            delete newLevel; 
        }
    }
};

#endif 
