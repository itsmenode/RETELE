#pragma once

#include "command.hpp"
#include "errorHandling.hpp"
#include "sessionManager.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utmpx.h>
#include <iomanip> 
#include <ctime>
#include <sys/types.h>

class GetProcInfoCommand: public Command{

    pid_t pid;

public:

    GetProcInfoCommand(pid_t pid1): pid(pid1) {}



    void execute() override {

        std::string path = "/proc/" + std::to_string(pid) + "/status";

        std::ifstream in(path);

        if (!in) {
            throw FileException("Unable to open the given path to check pid information.\n");
        }

        const std::string kName  = "Name:";
        const std::string kState = "State:";
        const std::string kPPid  = "PPid:";
        const std::string kUid   = "Uid:";
        const std::string kGid   = "Gid:";

        std::string vName, vState, vPPid, vUid, vGid;

        std::string line;

        int found = 0;
        while(std::getline(in, line)) {
            if (vName.empty() && line.rfind(kName, 0) == 0){
                vName = line.substr(kName.size());
                found++;
            } else if (vState.empty() && line.rfind(kState, 0) == 0) {
                vState = line.substr(kState.size());
                ++found;
            } else if (vPPid.empty() && line.rfind(kPPid, 0) == 0) {
                vPPid = line.substr(kPPid.size());
                ++found;
            } else if (vUid.empty() && line.rfind(kUid, 0) == 0) {
                vUid = line.substr(kUid.size());
                ++found;
            } else if (vGid.empty() && line.rfind(kGid, 0) == 0) {
                vGid = line.substr(kGid.size());
                ++found;
            }

            if (found == 5) break;
        }

        std::cout << "PID:   " << pid   << "\n";
        if (!vName.empty())  std::cout << "Name:  "  << vName  << "\n";
        if (!vState.empty()) std::cout << "State: "  << vState << "\n";
        if (!vPPid.empty())  std::cout << "PPid:  "  << vPPid  << "\n";
        if (!vUid.empty())   std::cout << "Uid:   "  << vUid   << "\n";
        if (!vGid.empty())   std::cout << "Gid:   "  << vGid   << "\n";

    }

};