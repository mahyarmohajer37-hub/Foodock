#ifndef MEMBERSHIP_FACTORY_H
#define MEMBERSHIP_FACTORY_H

#include "MembershipLevel.h"
#include <string>

class MembershipFactory {
public:
    static inline MembershipLevel* createLevelByName(const std::string& levelName) {
        if (levelName == "Silver") {
            return new SilverLevel();
        } else if (levelName == "Gold") {
            return new GoldLevel();
        } else if (levelName == "VIP" || levelName == "Vip") {
            return new VipLevel();
        }
        return new NormalLevel();
    }

    static inline MembershipLevel* determineLevelByPoints(int points) {
        if (points >= 700) {
            return new VipLevel();
        } else if (points >= 300) {
            return new GoldLevel();
        } else if (points >= 100) {
            return new SilverLevel();
        }
        return new NormalLevel();
    }
};

#endif 
