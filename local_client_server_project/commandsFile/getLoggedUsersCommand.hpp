
#pragma once
#include "command.hpp"
#include <utmpx.h>
#include <iostream>
#include <iomanip>
#include <ctime>

class GetLoggedUsersCommand : public Command {
public:
    void execute() override {
        setutxent();
        struct utmpx* u;
        while ((u = getutxent()) != nullptr) {
            if (u->ut_type != USER_PROCESS) continue;
            std::time_t t = u->ut_tv.tv_sec;
            std::tm lt{};
            localtime_r(&t, &lt);
            std::cout << std::left << std::setw(12) << u->ut_user << std::setw(12) << u->ut_line << std::setw(20) << u->ut_host << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << "\n";
        }
        endutxent();
    }
};