#ifndef APP_H
#define APP_H

#include <string>
#include "User.h"

class App {
private:
    std::string name;
    double requiredSizeMB;
    User* users;
    int userCount;
    int maxUsers;

public:
    App(std::string n = "", double size = 0.0, int maxU = 10);
    ~App();

    void setApp(std::string n, double size);
    void addUser(const User& u);
    void displayApp() const;
};

#endif
