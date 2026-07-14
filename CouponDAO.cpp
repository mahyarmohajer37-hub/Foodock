#include "CouponDAO.h"
#include "DatabaseManager.h"
#include "sqlite3.h"
#include <iostream>

using namespace std;

bool CouponDAO::addCoupon(Coupon coupon) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "INSERT INTO Coupons (user_id, code, discount_percent, issued_at, expires_at, is_used) "
                 "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, coupon.getUserId());
    sqlite3_bind_text(stmt, 2, coupon.getCode().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, coupon.getDiscountPercent());
    sqlite3_bind_text(stmt, 4, coupon.getIssuedAt().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, coupon.getExpiresAt().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, coupon.getIsUsed() ? 1 : 0);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

bool CouponDAO::updateCoupon(Coupon coupon) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "UPDATE Coupons SET user_id = ?, code = ?, discount_percent = ?, "
                 "issued_at = ?, expires_at = ?, is_used = ? WHERE id = ?;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, coupon.getUserId());
    sqlite3_bind_text(stmt, 2, coupon.getCode().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, coupon.getDiscountPercent());
    sqlite3_bind_text(stmt, 4, coupon.getIssuedAt().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, coupon.getExpiresAt().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, coupon.getIsUsed() ? 1 : 0);
    sqlite3_bind_int(stmt, 7, coupon.getId());

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

bool CouponDAO::deleteCoupon(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "DELETE FROM Coupons WHERE id = ?;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

Coupon CouponDAO::getCouponById(int id) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    Coupon coupon;

    string sql = "SELECT id, user_id, code, discount_percent, issued_at, expires_at, is_used "
                 "FROM Coupons WHERE id = ?;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return coupon;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        coupon.setId(sqlite3_column_int(stmt, 0));
        coupon.setUserId(sqlite3_column_int(stmt, 1));
        coupon.setCode((const char*)sqlite3_column_text(stmt, 2));
        coupon.setDiscountPercent(sqlite3_column_double(stmt, 3));
        coupon.setIssuedAt((const char*)sqlite3_column_text(stmt, 4));

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            coupon.setExpiresAt((const char*)sqlite3_column_text(stmt, 5));
        } else {
            coupon.setExpiresAt("");
        }

        coupon.setIsUsed(sqlite3_column_int(stmt, 6) != 0);
    }

    sqlite3_finalize(stmt);
    return coupon;
}

Coupon CouponDAO::getUnusedCouponByUserId(int userId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    Coupon coupon;

    string sql = "SELECT id, user_id, code, discount_percent, issued_at, expires_at, is_used "
                 "FROM Coupons "
                 "WHERE user_id = ? AND is_used = 0 "
                 "AND (expires_at IS NULL OR expires_at >= CURRENT_TIMESTAMP) "
                 "ORDER BY id LIMIT 1;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return coupon;
    }

    sqlite3_bind_int(stmt, 1, userId);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        coupon.setId(sqlite3_column_int(stmt, 0));
        coupon.setUserId(sqlite3_column_int(stmt, 1));
        coupon.setCode((const char*)sqlite3_column_text(stmt, 2));
        coupon.setDiscountPercent(sqlite3_column_double(stmt, 3));
        coupon.setIssuedAt((const char*)sqlite3_column_text(stmt, 4));

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            coupon.setExpiresAt((const char*)sqlite3_column_text(stmt, 5));
        } else {
            coupon.setExpiresAt("");
        }

        coupon.setIsUsed(sqlite3_column_int(stmt, 6) != 0);
    }

    sqlite3_finalize(stmt);
    return coupon;
}

Coupon CouponDAO::getValidCouponByCode(int userId, const string& code) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    Coupon coupon;

    string sql = "SELECT id, user_id, code, discount_percent, issued_at, expires_at, is_used "
                 "FROM Coupons "
                 "WHERE user_id = ? AND code = ? AND is_used = 0 "
                 "AND (expires_at IS NULL OR expires_at >= CURRENT_TIMESTAMP);";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return coupon;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, code.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        coupon.setId(sqlite3_column_int(stmt, 0));
        coupon.setUserId(sqlite3_column_int(stmt, 1));
        coupon.setCode((const char*)sqlite3_column_text(stmt, 2));
        coupon.setDiscountPercent(sqlite3_column_double(stmt, 3));
        coupon.setIssuedAt((const char*)sqlite3_column_text(stmt, 4));

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            coupon.setExpiresAt((const char*)sqlite3_column_text(stmt, 5));
        } else {
            coupon.setExpiresAt("");
        }

        coupon.setIsUsed(sqlite3_column_int(stmt, 6) != 0);
    }

    sqlite3_finalize(stmt);
    return coupon;
}

bool CouponDAO::markCouponUsed(int couponId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();

    string sql = "UPDATE Coupons SET is_used = 1 WHERE id = ? AND is_used = 0;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, couponId);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE && sqlite3_changes(db) > 0;
}

vector<Coupon> CouponDAO::getCouponsByUserId(int userId) {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    vector<Coupon> coupons;

    string sql = "SELECT id, user_id, code, discount_percent, issued_at, expires_at, is_used "
                 "FROM Coupons WHERE user_id = ? ORDER BY id DESC;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return coupons;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Coupon coupon;
        coupon.setId(sqlite3_column_int(stmt, 0));
        coupon.setUserId(sqlite3_column_int(stmt, 1));
        coupon.setCode((const char*)sqlite3_column_text(stmt, 2));
        coupon.setDiscountPercent(sqlite3_column_double(stmt, 3));
        coupon.setIssuedAt((const char*)sqlite3_column_text(stmt, 4));

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            coupon.setExpiresAt((const char*)sqlite3_column_text(stmt, 5));
        } else {
            coupon.setExpiresAt("");
        }

        coupon.setIsUsed(sqlite3_column_int(stmt, 6) != 0);
        coupons.push_back(coupon);
    }

    sqlite3_finalize(stmt);
    return coupons;
}

vector<Coupon> CouponDAO::getAllCoupons() {
    sqlite3* db = DatabaseManager::getInstance()->getDB();
    vector<Coupon> coupons;

    string sql = "SELECT id, user_id, code, discount_percent, issued_at, expires_at, is_used "
                 "FROM Coupons ORDER BY id DESC;";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (result != SQLITE_OK) {
        return coupons;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Coupon coupon;
        coupon.setId(sqlite3_column_int(stmt, 0));
        coupon.setUserId(sqlite3_column_int(stmt, 1));
        coupon.setCode((const char*)sqlite3_column_text(stmt, 2));
        coupon.setDiscountPercent(sqlite3_column_double(stmt, 3));
        coupon.setIssuedAt((const char*)sqlite3_column_text(stmt, 4));

        if (sqlite3_column_type(stmt, 5) != SQLITE_NULL) {
            coupon.setExpiresAt((const char*)sqlite3_column_text(stmt, 5));
        } else {
            coupon.setExpiresAt("");
        }

        coupon.setIsUsed(sqlite3_column_int(stmt, 6) != 0);
        coupons.push_back(coupon);
    }

    sqlite3_finalize(stmt);
    return coupons;
}
