#ifndef MEMBERSHIP_LEVEL_H
#define MEMBERSHIP_LEVEL_H

#include <string>

class MembershipLevel {
public:
    virtual ~MembershipLevel() {}

    virtual std::string getLevelName() const = 0;
    virtual double getDiscountPercent() const = 0;
    virtual double calculateShippingCost(double baseCost, double total) const = 0;
    virtual double getPointMultiplier() const = 0;
    virtual int getRequiredPointsForNextLevel() const = 0;

    virtual MembershipLevel* clone() const = 0;
};

class NormalLevel : public MembershipLevel {
public:
    std::string getLevelName() const override { return "Normal"; }
    double getDiscountPercent() const override { return 0.0; } 
    
    double calculateShippingCost(double baseCost, double total) const override {
        return baseCost; 
    }
    
    double getPointMultiplier() const override { return 1.0; } 
    int getRequiredPointsForNextLevel() const override { return 100; } 

    MembershipLevel* clone() const override {
        return new NormalLevel(*this);
    }
};

class SilverLevel : public MembershipLevel {
public:
    std::string getLevelName() const override { return "Silver"; }
    double getDiscountPercent() const override { return 0.05; }
    
    double calculateShippingCost(double baseCost, double total) const override {
        return baseCost; 
    }
    
    double getPointMultiplier() const override { return 1.2; }
    int getRequiredPointsForNextLevel() const override { return 250; } 

    MembershipLevel* clone() const override {
        return new SilverLevel(*this);
    }
};

class GoldLevel : public MembershipLevel {
public:
    std::string getLevelName() const override { return "Gold"; }
    double getDiscountPercent() const override { return 0.10; } 
    
    double calculateShippingCost(double baseCost, double total) const override {
        return baseCost * 0.5; 
    }
    
    double getPointMultiplier() const override { return 1.5; } 
    int getRequiredPointsForNextLevel() const override { return 500; } 

    MembershipLevel* clone() const override {
        return new GoldLevel(*this);
    }
};

class VipLevel : public MembershipLevel {
public:
    std::string getLevelName() const override { return "VIP"; }
    double getDiscountPercent() const override { return 0.15; } 
    
    double calculateShippingCost(double baseCost, double total) const override {
        return 0.0; 
    }
    
    double getPointMultiplier() const override { return 2.0; }
    int getRequiredPointsForNextLevel() const override { return 0; } 

    MembershipLevel* clone() const override {
        return new VipLevel(*this);
    }
};

#endif 
