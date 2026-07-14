#ifndef COUPONDAO_H
#define COUPONDAO_H

#include "Coupon.h"
#include <vector>
#include <string>

using namespace std;

class CouponDAO {
public:
    bool addCoupon(Coupon coupon);
    bool updateCoupon(Coupon coupon);
    bool deleteCoupon(int id);

    Coupon getCouponById(int id);
    Coupon getUnusedCouponByUserId(int userId);
    int getUnusedCouponsCountByUserId(int userId);
    Coupon getValidCouponByCode(int userId, const string& code);

    bool markCouponUsed(int couponId);

    vector<Coupon> getCouponsByUserId(int userId);
    vector<Coupon> getAllCoupons();
};

#endif
