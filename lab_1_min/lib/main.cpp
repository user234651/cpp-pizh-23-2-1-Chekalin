#include <iostream>
#include "App.h"
#include "User.h"

int main() {
    App myApp("UserManager", 120.5, 3);

    User u1("admin", "1234", "Administrator");
    User u2("user1", "abcd", "Editor");
    User u3("guest", "guest", "Viewer");

    myApp.addUser(u1);
    myApp.addUser(u2);
    myApp.addUser(u3);

    myApp.displayApp();

    return 0;
}
