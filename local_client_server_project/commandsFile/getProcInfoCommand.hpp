
#pragma once
#include "command.hpp"
#include <fstream>
#include <iostream>
#include <string>

class GetProcInfoCommand : public Command {

    pid_t pid;

public:

    explicit GetProcInfoCommand(pid_t p) : pid(p) {}

    void execute() override {
        std::string path = "/proc/" + std::to_string(pid) + "/status";
        std::ifstream in(path);
        if (!in) {
            std::cout << "Cannot open " << path << "\n";
            return;
        }
        std::string line;
        while (std::getline(in, line)) {
            if (line.rfind("Name:",0)==0 || line.rfind("State:",0)==0 || line.rfind("PPid:",0)==0 || line.rfind("Uid:",0)==0 || line.rfind("Gid:",0)==0) {
                std::cout << line << "\n";
            }
        }
    }
};