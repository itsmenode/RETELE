
#pragma once
#include "errorHandling.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <optional>

// Very small file‑backed user/session store.
// Format per line: username:password:active|inactive
class SessionManager {
public:
    static SessionManager& instance() {
        static SessionManager inst;
        return inst;
    }

    void set_db_path(const std::string& path) { db_path = path; }

    void signup(const std::string& user, const std::string& pass) {
        auto users = load();
        for (auto& u : users) {
            if (u.username == user) {
                throw UserException("User already exists: " + user);
            }
        }
        users.push_back({user, pass, false});
        save(users);
    }

    void login(const std::string& user, const std::string& pass) {
        auto users = load();
        bool found = false;
        for (auto& u : users) {
            if (u.username == user) {
                found = true;
                if (u.password != pass) throw AuthException("Bad credentials for user: " + user);
                u.active = true;
            }
        }
        if (!found) throw UserException("Unknown user: " + user);
        save(users);
    }

    void logout(const std::string& user) {
        auto users = load();
        bool found = false;
        for (auto& u : users) {
            if (u.username == user) {
                found = true;
                u.active = false;
            }
        }
        if (!found) throw UserException("Unknown user: " + user);
        save(users);
    }

    bool is_active(const std::string& user) {
        auto users = load();
        for (auto& u : users) if (u.username == user) return u.active;
        return false;
    }

private:
    struct Rec { std::string username; std::string password; bool active; };
    std::string db_path = "users.db";

    std::vector<Rec> load() {
        std::vector<Rec> v;
        std::ifstream in(db_path);
        if (!in.good()) return v;
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string a,b,c;
            if (std::getline(iss,a,':') && std::getline(iss,b,':') && std::getline(iss,c,':')) {
                bool active = (c == "active");
                v.push_back({a,b,active});
            }
        }
        return v;
    }

    void save(const std::vector<Rec>& v) {
        std::ofstream out(db_path, std::ios::trunc);
        if (!out) throw FileException("Cannot open db file: " + db_path);
        for (auto& u : v) {
            out << u.username << ":" << u.password << ":" << (u.active ? "active" : "inactive") << "\n";
        }
    }

    SessionManager() = default;
    ~SessionManager() = default;
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;
};