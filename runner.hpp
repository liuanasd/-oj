#pragma once

#include "modou.hpp"
#include "compiler.hpp"

namespace ns_runner {
using namespace ns_log;
using namespace ns_util;

class Runner {
public:
    static void SetProcLimit(int cpu_limit, int mem_limit) {
        struct rlimit r;
        r.rlim_cur = cpu_limit;
        r.rlim_max = RLIM_INFINITY;
        setrlimit(RLIMIT_CPU, &r);

        r.rlim_cur = mem_limit * 1024 * 1024;
        r.rlim_max = RLIM_INFINITY;
        setrlimit(RLIMIT_AS, &r);
    }

    static int run(const std::string &file_name, int cpu_limit, int mem_limit) {
        std::string exe_file = File_name::Exe(file_name);
        std::string stdin_file = File_name::Stdin(file_name);
        std::string stdout_file = File_name::Stdout(file_name);
        std::string stderr_file = File_name::Stderr(file_name);

        umask(0);

        int stdin_fd = open(stdin_file.c_str(), O_CREAT | O_RDONLY, 0644);
        int stdout_fd = open(stdout_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        int stderr_fd = open(stderr_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (stdin_fd < 0 || stdout_fd < 0 || stderr_fd < 0) {
            return -1;
        }

        pid_t pid = fork();
        if (pid < 0) {
            close(stdin_fd);
            close(stdout_fd);
            close(stderr_fd);
            return -2;
        }

        if (pid == 0) {
            dup2(stdin_fd, 0);
            dup2(stdout_fd, 1);
            dup2(stderr_fd, 2);

            close(stdin_fd);
            close(stdout_fd);
            close(stderr_fd);

            SetProcLimit(cpu_limit, mem_limit);
            execl(exe_file.c_str(), exe_file.c_str(), nullptr);
            exit(1);
        }

        close(stdin_fd);
        close(stdout_fd);
        close(stderr_fd);

        int status = 0;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }

        if (WIFSIGNALED(status)) {
            return WTERMSIG(status);
        }

        return -3;
    }
};
}
