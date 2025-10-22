#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

inline std::string trim(const std::string& s) {
    const char* ws = " \t\r\n";
    auto a = s.find_first_not_of(ws);
    if (a == std::string::npos) return "";
    auto b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

inline std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string item;
    std::istringstream iss(s);
    while (std::getline(iss, item, sep)) out.push_back(item);
    return out;
}