#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include <string>
#include <iostream>

class LogoutCommand: Command {
    std::string user;

public:
    LogoutCommand(std::string user) : user(move(user)) {}

    void execute() override {
        SessionManager::instance().logout(user);
        std::cout << "User: " << user << " Status: INACTIVE.\n";
    }

};