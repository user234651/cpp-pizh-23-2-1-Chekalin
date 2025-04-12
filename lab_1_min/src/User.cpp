#include "User.h"
#include <iostream>

User::User() : login(""), password(""), role("") {}

User::User(std::string l, std::string p, std::string r) {
    login = l;
    password = p;
    role = r;
}

void User::setUser(std::string l, std::string p, std::string r) {
    login = l;
    password = p;
    role = r;
}

void User::displayUser() const {
    std::cout << "Login: " << login
              << ", Password: " << password
              << ", Role: " << role << std::endl;
}
