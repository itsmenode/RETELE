#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include <string>
#include <iostream>

class LoginCommand: Command {
    std::string user;
    std::string password;

public:
    LoginCommand(std::string user, std::string password) : user(move(user)), password(move(password)) {}

    void execute() override {
        SessionManager::instance().login(user, password);
        std::cout << "User: " << user << " Status: ACTIVE.\n";
    }

};