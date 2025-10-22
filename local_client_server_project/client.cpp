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
#include <sys/stat.h>
#include <stdio.h>

using namespace std;

int main(){

    mkfifo("client_to_server", 0666);
    int client_to_server = open("client_to_server", O_WRONLY);

    while(true){

        string input;
        cout << "Enter Command: ";
        getline(cin, input);
        int lungime_comanda = input.size();

        int send_for_review[2];
        if (pipe(send_for_review) == -1) {
            throw UncategorisedError("Could not create pipe for communication between father and child.\n");
        }

        pid_t pid = fork();
        if (pid < 0) {
            throw UncategorisedError("Could not create child proccess.]n");
        }

        if (pid == 0) {

            close(send_for_review[1]);

            string line;
            line.resize(1024);
            ssize_t n = read(send_for_review[0], line.data(), line.size());
            if (n < 0) {
                throw UncategorisedError("Could not read from pipe.\n");
            }

            line.resize(static_cast<size_t>(n));
            int marime = static_cast<int>(line.size());

            write(client_to_server, &marime, sizeof(int));
            if (marime > 0) {
                write(client_to_server, line.data(), static_cast<size_t>(marime));
            }

            close(send_for_review[0]);

        } else {

            close(send_for_review[0]);

            ssize_t n = write(send_for_review[1], input.data(), static_cast<size_t>(lungime_comanda));
            if (n < 0) {
                close(send_for_review[1]);
                throw UncategorisedError("Could not write in the pipe.\n");
            }

            close(send_for_review[1]);
            waitpid(pid, NULL, 0);

        }
    }
}
