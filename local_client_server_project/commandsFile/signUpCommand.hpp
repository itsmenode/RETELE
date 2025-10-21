#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include <string>
#include <iostream>

class SignUpCommand: public Command {
    std::string user;
    std::string password;

public:
    SignUpCommand(std::string user, std::string password) : user(move(user)), password(move(password)) {}

    void execute() override {
        SessionManager::instance().signup(user, password);
        std::cout << "User: " << user << " SIGNED UP.\n";
        std::cout << "To continue you must log in.\n";
    }

};