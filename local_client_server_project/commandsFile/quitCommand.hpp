#pragma once

#include "command.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>

class QuitCommand : public Command {
public:
    void execute() override {
        std::cout << "Server shutting down...\n";
        kill(0, SIGKILL);
    }
};