#pragma once

#include "errorHandling.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

class SessionManager {

private:
    

    SessionManager() = default;
    ~SessionManager() = default;

public:

    static SessionManager& instance() {
        static SessionManager inst;
        return inst;
    }

    void signup(const std::string& user, const std::string& password) {
        if (userExists(user)){
            throw UserException("User: " + user + " does ALREADY EXISTS.");
        }

        std::ifstream in("users.txt");
        if(!in) {
            throw FileException("Could NOT OPEN file: users.txt for INPUT");
        }

        std::ofstream out("users.txt", std::ios::app);
        if (!out) {
            throw FileException("Could NOT OPEN file: users.txt for OUTPUT");
        }

        out << user << ':' << password << ':' << "inactive" << '\n';

        out.close();
    }

    void login(const std::string& user, const std::string& password){

        if (userExists(user) == false) {
            throw UserException("User: " + user + " does NOT EXIST.");
        }

        std::ifstream in("users.txt");
        if(!in) {
            throw FileException("Could NOT OPEN file: users.txt for INPUT");
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(in, line)) {
            lines.push_back(line);
        }
        in.close();


        for (std::string& l : lines) {
            std::size_t usrPosition = line.find(':');
            if (usrPosition == std::string::npos) continue;

            std::size_t pwdPosition = line.find(':', usrPosition + 1);
            if (pwdPosition == std::string::npos) continue;

            std::string usr = line.substr(0, usrPosition);
            std::string pwd = line.substr(usrPosition + 1, pwdPosition - (usrPosition + 1));

            if (usr == user && pwd == password) {
                l = usr + ":" + pwd + ":" + "active";
            }
            else {
                throw AuthException("User: " + user + " could NOT BE LOGGED IN.");
            }

        }


        std::ofstream out("users.txt", std::ios::trunc);
        if (!out) {
            throw FileException("Could NOT OPEN file: users.txt for OUTPUT");
        }

        for (const std::string& l : lines) {
            out << l << '\n';
        }
        out.close();

    }

    void logout(const std::string& user){

        if (userExists(user) == false) {
            throw UserException("User: " + user + " does NOT EXIST.");
        }

        if (isAuthenticated(user) == false) {
            throw AuthException("User: " + user + " NOT AUTHENTICATED.");
        }

        std::ifstream in("users.txt");
        if(!in) {
            throw FileException("Could NOT OPEN file: users.txt for INPUT");
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(in, line)) {
            lines.push_back(line);
        }
        in.close();


        for (std::string& l : lines) {
            std::size_t usrPosition = line.find(':');
            if (usrPosition == std::string::npos) continue;

            std::size_t pwdPosition = line.find(':', usrPosition + 1);
            if (pwdPosition == std::string::npos) continue;

            std::string usr = line.substr(0, usrPosition);
            std::string pwd = line.substr(usrPosition + 1, pwdPosition - (usrPosition + 1));

            if (usr == user) {
                l = usr + ":" + pwd + ":" + "inactive";
            }
            else {
                throw AuthException("User: " + user + " could NOT BE LOGGED OUT.");
            }

        }

        std::ofstream out("users.txt", std::ios::trunc);
        if (!out) {
            throw FileException("Could NOT OPEN file: users.txt for OUTPUT");
        }

        for (const std::string& l : lines) {
            out << l << '\n';
        }
        out.close();

    }

    bool isAuthenticated(const std::string &user) {

        if (userExists(user) == false) {
            return false;
        }

        std::ifstream in("user.txt");
        if(!in) {
            return false;
        }

        std::string line;
        while (std::getline(in, line)) {

            if (line.empty() || line[0] == '#') continue;

            std::size_t usrPosition = line.find(':');
            if (usrPosition == std::string::npos) continue;

            std::size_t pwdPosition = line.find(':', usrPosition + 1);
            if (pwdPosition == std::string::npos) continue;

            std::string usr = line.substr(0, usrPosition);
            std::string status = line.substr(pwdPosition + 1);

            if (user == usr) {
                if(status == "active") return true;
                else return false;
            }

        }

    }

    bool userExists(const std::string &user) {

        std::ifstream in("users.txt");
        if(!in) {
            return false;
        }

        std::string line;
        while (std::getline(in, line)) {

            if (line.empty() || line[0] == '#') continue;
            
            std:size_t pos = line.find(':');
            if (pos == std::string::npos) continue;

            if (line.substr(0, pos) == user) return true;
        }

        return false;

    }

    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

private:
    SessionManager() = default;
    ~SessionManager() = default;


};