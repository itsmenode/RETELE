#pragma once

#include "command.hpp"
#include "../errorHandling.hpp"
#include "../sessionManager.hpp"
#include <string>
#include <iostream>
#include <utility>

class LogoutCommand : public Command {
    std::string user;
public:
    explicit LogoutCommand(std::string u) : user(std::move(u)) {}
    void execute() override {
        SessionManager::instance().logout(user);
        std::cout << "User '" << user << "' logged out (INACTIVE)\n";
    }
};