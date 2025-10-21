#pragma once

#include "command.hpp"
#include "loginCommand.hpp"
#include "logoutCommand.hpp"
#include "quitCommand.hpp"
#include "signUpCommand.hpp"
#include "getLoggedUsersCommand.hpp"
#include "getProcInfoCommand.hpp"
#include <iostream>
#include <string>
#include <memory>

class CommandFactory{
public:

    struct Command* createCommand(const std::string& comm, const std::string& value1, const std::string& value2) {
        if (comm == "login"){
            return new LoginCommand(value1, value2);
        } else if (comm == "logout") {
            return new LogoutCommand(value1);
        } else if (comm == "singup") {
            return new SignUpCommand(value1, value2);
        } else if (comm == "quit") {
            return new QuitCommand();
        }
    }

};