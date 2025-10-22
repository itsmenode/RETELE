#pragma once

#include "command.hpp"
#include "../errorHandling.hpp"
#include "../sessionManager.hpp"
#include <string>
#include <iostream>
#include <utility>

class SignUpCommand : public Command {
    std::string user;
    std::string password;
public:
    SignUpCommand(std::string u, std::string p) : user(std::move(u)), password(std::move(p)) {}
    void execute() override {
        SessionManager::instance().signup(user, password);
        std::cout << "User '" << user << "' SIGNED UP. Please login next.\n";
    }
};