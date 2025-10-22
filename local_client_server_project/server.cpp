#include "./commandsFile/command.hpp"
#include "./commandsFile/commandFactory.hpp"
#include "./commandsFile/getLoggedUsersCommand.hpp"
#include "./commandsFile/getProcInfoCommand.hpp"
#include "./commandsFile/loginCommand.hpp"
#include "./commandsFile/logoutCommand.hpp"
#include "./commandsFile/quitCommand.hpp"
#include "./commandsFile/signUpCommand.hpp"
#include "errorHandling.hpp"
#include "sessionManager.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

static string trim(string s) {
    const char* ws = " \t";
    size_t a = s.find_first_not_of(ws);
    size_t b = s.find_last_not_of(ws);
    if (a == string::npos) return "";
    return s.substr(a, b - a + 1);
}


static void parse_command(const string& input, string& command, string& arg1, string& arg2) {
    string s = input;
    size_t p1 = s.find(':');
    if (p1 == string::npos) {
        command = trim(s);
        arg1.clear();
        arg2.clear();
        return;
    }

    command = trim(s.substr(0, p1));
    string rest = (p1 + 1 < s.size()) ? s.substr(p1 + 1) : "";

    size_t p2 = rest.find(':');
    if (p2 == string::npos) {
        arg1 = trim(rest);
        arg2.clear();
        return;
    }

    arg1 = trim(rest.substr(0, p2));
    arg2 = trim(rest.substr(p2 + 1));
}


int main(){

    int client_to_server = open("client_to_server", O_WRONLY);

    int lungime_comanda;
    char comanda[10000];

    while (true) {

        read(client_to_server, &lungime_comanda, sizeof(int));
        read(client_to_server, comanda, sizeof(char) * lungime_comanda);


        int sv[2];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
            throw UncategorisedError("Could not create socket.\n");
        }

        pid_t pid = fork();
        if (pid < 0) {
            throw UncategorisedError("Could not create server sided child proccess.\n");
        }

        if (pid == 0) {
            close(sv[0]);
            char buf[256];
        } else {

            close(sv[1]);

            write(sv[0], comanda, strlen(comanda));
            
            close(sv[0]);
            waitpid(pid, NULL, 0);
        }

    }
}
