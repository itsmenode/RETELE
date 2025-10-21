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

    struct Command* createCommand(const std::string& comm, const std::string& username, const std::string& password, const pid_t& pid) {
        if (comm == "login"){
            return new LoginCommand(username, password);
        } else if (comm == "logout") {
            return new LogoutCommand(username);
        } else if (comm == "singup") {
            return new SignUpCommand(username, password);
        } else if (comm == "quit") {
            return new QuitCommand();
        } else if (comm == "get-logged-users") {
            return new GetLoggedUsersCommand();
        } else if (comm == "get_proc_info") {
            return new GetProcInfoCommand(pid);
        } else if (comm == "quit") {
            return new QuitCommand();
        }
    }

};