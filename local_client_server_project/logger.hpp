#pragma once

#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "errorHandling.hpp"

class Logger {

public:

    static Logger& instance() {
        static Logger inst;
        return inst;
    }

    void logUserLogin(const std::string& user, const std::string& password){
        
        std::time_t now = std::time(nullptr);
        std::tm* lt = std::localtime(&now);

        std::ofstream out("userLogin_Log.txt", std::ios::app);

        if (!out) {
            throw FileException("Could not open userLogin_Log.txt file for keeping log.\n");
        }

        out << "User: " << user << " Login Status: [ACTIVE] || " << "Time: [ ";

        ///std::ofstream out("activity_Log.txt", std::ios::app);

    }
    void logUserSignUp(const std::string& user, const std::string& password);
    void logUserLogout(const std::string& user);
    void logCommand(const std::string& comm);


    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:

    Logger() = default;
    ~Logger() = default;

};