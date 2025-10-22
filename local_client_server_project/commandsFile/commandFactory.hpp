
#pragma once
#include "command.hpp"
#include "loginCommand.hpp"
#include "logoutCommand.hpp"
#include "quitCommand.hpp"
#include "signUpCommand.hpp"
#include "getLoggedUsersCommand.hpp"
#include "getProcInfoCommand.hpp"
#include "../errorHandling.hpp"
#include <string>
#include <cctype>
#include <sstream>

class CommandFactory {
public:
    static Command* create(const std::string& comm, const std::string& username = "", const std::string& password = "", pid_t pid = 0) {
        if (comm == "login") {
            return new LoginCommand(username, password);
        } else if (comm == "logout") {
            return new LogoutCommand(username);
        } else if (comm == "signup") {
            return new SignUpCommand(username, password);
        } else if (comm == "quit") {
            return new QuitCommand();
        } else if (comm == "get-logged-users") {
            return new GetLoggedUsersCommand();
        } else if (comm == "get-proc-info") {
            return new GetProcInfoCommand(pid);
        }
        throw InvalidCommandException("Unknown command: " + comm);
    }
};