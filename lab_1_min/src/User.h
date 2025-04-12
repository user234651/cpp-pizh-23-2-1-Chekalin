#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string login;
    std::string password;
    std::string role;

public:
    User();
    User(std::string l, std::string p, std::string r);

    void setUser(std::string l, std::string p, std::string r);
    void displayUser() const;
};

#endif
