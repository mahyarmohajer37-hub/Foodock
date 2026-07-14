#include "Coupon.h"

Coupon::Coupon() {
    this->id = 0;
    this->userId = 0;
    this->code = "";
    this->discountPercent = 0.0;
    this->issuedAt = "";
    this->expiresAt = "";
    this->isUsed = false;
}

Coupon::Coupon(int id, int userId, string code, double discountPercent,
               string issuedAt, string expiresAt, bool isUsed) {
    this->id = id;
    this->userId = userId;
    this->code = code;
    this->discountPercent = discountPercent;
    this->issuedAt = issuedAt;
    this->expiresAt = expiresAt;
    this->isUsed = isUsed;
}

int Coupon::getId() {
    return id;
}

int Coupon::getUserId() {
    return userId;
}

string Coupon::getCode() {
    return code;
}

double Coupon::getDiscountPercent() {
    return discountPercent;
}

string Coupon::getIssuedAt() {
    return issuedAt;
}

string Coupon::getExpiresAt() {
    return expiresAt;
}

bool Coupon::getIsUsed() {
    return isUsed;
}

void Coupon::setId(int id) {
    this->id = id;
}

void Coupon::setUserId(int userId) {
    this->userId = userId;
}

void Coupon::setCode(string code) {
    this->code = code;
}

void Coupon::setDiscountPercent(double discountPercent) {
    this->discountPercent = discountPercent;
}

void Coupon::setIssuedAt(string issuedAt) {
    this->issuedAt = issuedAt;
}

void Coupon::setExpiresAt(string expiresAt) {
    this->expiresAt = expiresAt;
}

void Coupon::setIsUsed(bool isUsed) {
    this->isUsed = isUsed;
}
