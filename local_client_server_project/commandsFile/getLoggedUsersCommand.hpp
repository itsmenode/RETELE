#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include "sessionManager.hpp"
#include <fstream>
#include <string>
#include <utmpx.h>
#include <iomanip> 
#include <ctime>

class GetLoggedUsersCommand: public Command{

public:

    GetLoggedUsersCommand() = default;

    void execute() override {
        if (SessionManager::instance().isAnyUserAuthenticated() == false) {
            throw AccessDeniedException("No user authenticated in the system.\n");
        }

        std::cout << "USER\tTTY\tHOST\tTIME\n";

        setutxent();
        for (utmpx* u = getutxent(); u != nullptr; u = getutxent()) {
            if (u -> ut_type != USER_PROCESS) continue;

            std::string user = u -> ut_user;
            std::string line = u -> ut_line;
            std::string host = *u -> ut_host ? u -> ut_host : "-";

            std::time_t t = u -> ut_tv.tv_sec;
            std::tm lt{};

            localtime_r(&t, &lt);

            std::cout << user << '\t' << line << '\t' << host << '\t' << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << '\t' << '\n';

        }

        endutxent();

    }

};