#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

enum class UserRole {
    ADMIN,
    DOCTOR,
    PHARMACIST,
    BILLING
};

struct User {
    std::string username;
    std::string password;
    UserRole role;

    std::string getRoleString() const {
        switch (role) {
            case UserRole::ADMIN: return "Admin";
            case UserRole::DOCTOR: return "Doctor";
            case UserRole::PHARMACIST: return "Pharmacist";
            case UserRole::BILLING: return "Billing";
            default: return "Unknown";
        }
    }
};

#endif
