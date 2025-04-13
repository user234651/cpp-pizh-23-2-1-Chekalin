#include "App.h"
#include <iostream>

App::App(std::string n, double size, int maxU) : name(n), requiredSizeMB(size), userCount(0), maxUsers(maxU) {
    users = new User[maxUsers];
}

App::~App() {
    delete[] users;
}

void App::setApp(std::string n, double size) {
    name = n;
    requiredSizeMB = size;
}

void App::addUser(const User& u) {
    if (userCount < maxUsers) {
        users[userCount++] = u;
    } else {
        std::cout << "Cannot add more users. Limit reached." << std::endl;
    }
}

void App::displayApp() const {
    std::cout << "App name: " << name << ", Required size: " << requiredSizeMB << " MB\n";
    std::cout << "Users list:\n";
    for (int i = 0; i < userCount; ++i) {
        users[i].displayUser();
    }
}
