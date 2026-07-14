#ifndef COUPON_H
#define COUPON_H

#include <string>

using namespace std;

class Coupon {
public:
    Coupon();
    Coupon(int id, int userId, string code, double discountPercent,
           string issuedAt, string expiresAt, bool isUsed);

    int getId();
    int getUserId();
    string getCode();
    double getDiscountPercent();
    string getIssuedAt();
    string getExpiresAt();
    bool getIsUsed();

    void setId(int id);
    void setUserId(int userId);
    void setCode(string code);
    void setDiscountPercent(double discountPercent);
    void setIssuedAt(string issuedAt);
    void setExpiresAt(string expiresAt);
    void setIsUsed(bool isUsed);
	
private:
    int id;
    int userId;
    string code;
    double discountPercent;
    string issuedAt;
    string expiresAt;
    bool isUsed;
};

#endif
