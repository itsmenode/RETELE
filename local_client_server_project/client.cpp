#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <cstdint>

static const char* CMD_FIFO = "/tmp/cmd_fifo";
static const char* RESP_FIFO = "/tmp/resp_fifo";

static void write_all(int fd, const void* buf, size_t len) {
    const char* p = static_cast<const char*>(buf);
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n <= 0) {
            perror("write");
            _exit(1);
        }
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

    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(pfd[1]);

        int cmd_fd = open(CMD_FIFO, O_WRONLY);
        if (cmd_fd < 0) {
            perror("open cmd fifo");
            return 1;
        }
        int resp_fd = open(RESP_FIFO, O_RDONLY);
        if (resp_fd < 0) {
            perror("open resp fifo");
            return 1;
        }

        while (true) {
            uint32_t len = 0;
            if (!read_all(pfd[0], &len, sizeof(len))) break;

            std::string line(len, '\0');
            if (len && !read_all(pfd[0], line.data(), len)) break;

            
            write_all(cmd_fd, &len, sizeof(len));
            if (len) write_all(cmd_fd, line.data(), len);

            
            uint32_t out_len = 0;
            if (!read_all(resp_fd, &out_len, sizeof(out_len))) break;
            std::string out(out_len, '\0');
            if (out_len && !read_all(resp_fd, out.data(), out_len)) break;

            if (!out.empty()) std::cout << out;

            if (line == "quit") break;
        }
        
        std::cout.flush();
        std::cerr.flush();
        close(cmd_fd);
        close(resp_fd);
        close(pfd[0]);
        _exit(0);

    } else {
        close(pfd[0]);

        std::string line;
        std::cout << "Enter commands (e.g. 'signup: user pass', 'login: user pass', 'logout: user', 'get-logged-users', 'get-proc-info: 1', 'quit')\n";
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;

            uint32_t len = (uint32_t)line.size();
            write_all(pfd[1], &len, sizeof(len));
            if (len) write_all(pfd[1], line.data(), len);

            if (line == "quit") break;
        }
        close(pfd[1]);
        waitpid(pid, nullptr, 0);
    }
    return 0;
}