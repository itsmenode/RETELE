#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

class QuitCommand: Command {

public:

    void execute() override {

        std::cout << "Shutting down...\n";

        kill(0, SIGKILL);
        std::exit(EXIT_SUCCESS);
    }

};