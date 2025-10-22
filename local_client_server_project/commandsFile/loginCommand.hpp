#pragma once

#include "command.hpp"
#include "../errorHandling.hpp"
#include "../sessionManager.hpp"
#include <string>
#include <iostream>
#include <utility>

class LoginCommand : public Command {
    std::string user;
    std::string password;
public:
    LoginCommand(std::string u, std::string p) : user(std::move(u)), password(std::move(p)) {}
    void execute() override {
        SessionManager::instance().login(user, password);
        std::cout << "User '" << user << "' logged in (ACTIVE)\n";
    }
};