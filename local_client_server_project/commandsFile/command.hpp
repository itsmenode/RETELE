#pragma once

#include "../sessionManager.hpp"
#include <iostream>
#include <string>

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};