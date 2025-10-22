
#include "./commandsFile/command.hpp"
#include "./commandsFile/commandFactory.hpp"
#include "sessionManager.hpp"
#include "errorHandling.hpp"
#include "protocol.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

static const char* CMD_FIFO = "/tmp/cmd_fifo";
static const char* RESP_FIFO = "/tmp/resp_fifo";

static void write_all(int fd, const void* buf, size_t len) {
    const char* p = static_cast<const char*>(buf);
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n <= 0) _exit(1);
        p += n; len -= n;
    }
}
static bool read_all(int fd, void* buf, size_t len) {
    char* p = static_cast<char*>(buf);
    while (len > 0) {
        ssize_t n = read(fd, p, len);
        if (n <= 0) return false;
        p += n; len -= n;
    }
    return true;
}

int main() {

    mkfifo(CMD_FIFO, 0666);
    mkfifo(RESP_FIFO, 0666);

    SessionManager::instance().set_db_path("users.db");

    int cmd_fd = open(CMD_FIFO, O_RDONLY);
    if (cmd_fd < 0) {
        perror("open cmd fifo");
        return 1;
    }

    int resp_fd = open(RESP_FIFO, O_WRONLY);
    if (resp_fd < 0) {
        perror("open resp fifo");
        return 1;
    }

    while (true) {
        uint32_t len = 0;
        if (!read_all(cmd_fd, &len, sizeof(len))) break;

        std::string cmd(len, '\0');
        if (!read_all(cmd_fd, cmd.data(), len)) break;

        cmd = trim(cmd);

        int sv[2];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
            perror("socketpair"); break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            close(sv[0]);
            dup2(sv[1], STDOUT_FILENO);
            dup2(sv[1], STDERR_FILENO);
            close(sv[1]);

            
            try {std::string name = cmd;
                std::string arg = "";
                auto parts = split(cmd, ':');
                if (parts.size() >= 1) name = trim(parts[0]);
                if (parts.size() >= 2) arg = trim(parts[1]);

                std::string user, pass;
                pid_t cpid = 0;

                if (name == "login" || name == "signup" || name == "singup") {
                    auto toks = split(arg, ' ');
                    if (toks.size() < 2) throw InvalidCommandException("Usage: login: <user> <pass>");
                    user = toks[0];
                    pass = toks[1];
                } else if (name == "logout") {
                    user = trim(arg);
                    if (user.empty()) throw InvalidCommandException("Usage: logout: <user>");
                } else if (name == "get-proc-info") {
                    cpid = static_cast<pid_t>(std::stol(arg));
                }

                Command* c = CommandFactory::create(name, user, pass, cpid);
                c->execute();
                delete c;
            } catch (const std::exception& e) {
                std::cout << "ERROR: " << e.what() << "\n";
            }
            _exit(0);
        } else if (pid > 0) {

            close(sv[1]);

            std::string buf;
            char tmp[4096];

            ssize_t n;
            while ((n = read(sv[0], tmp, sizeof(tmp))) > 0) {
                buf.append(tmp, tmp + n);
            }

            close(sv[0]);
            waitpid(pid, nullptr, 0);

            uint32_t out_len = (uint32_t)buf.size();
            
            write_all(resp_fd, &out_len, sizeof(out_len));
            if (out_len) write_all(resp_fd, buf.data(), out_len);

            if (cmd == "quit") {
                break;
            }

        } else {
            perror("fork");
            break;
        }
    }

    close(cmd_fd);
    close(resp_fd);
    unlink(CMD_FIFO);
    unlink(RESP_FIFO);

    return 0;
}