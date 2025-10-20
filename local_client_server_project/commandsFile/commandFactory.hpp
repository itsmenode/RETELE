#include "command.hpp"
#include <iostream>
#include <string>
#include <memory>

class CommandFactory{
public:

    struct Command* createCommand(const std::string& comm) {
        if (comm == "login") {
            
        }
    }

};