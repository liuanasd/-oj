#include "compiler.hpp"
#include <atomic>
#include <cerrno>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace ns_log;

namespace {
std::atomic<unsigned long long> g_file_seq(0);
}

namespace ns_util {
const std::string temp_path = "./temp/";

std::string File_name::add(const std::string &file_name, const std::string &suffix) {
    return file_name + suffix;
}

std::string File_name::Src(const std::string &file_name) {
    return temp_path + file_name + ".cpp";
}

std::string File_name::Exe(const std::string &file_name) {
    return temp_path + file_name;
}

std::string File_name::Stderr(const std::string &file_name) {
    return temp_path + file_name + ".stderr";
}

std::string File_name::Stdin(const std::string &file_name) {
    return temp_path + file_name + ".stdin";
}

std::string File_name::Stdout(const std::string &file_name) {
    return temp_path + file_name + ".stdout";
}

std::string File_name::CompilerError(const std::string &file_name) {
    return temp_path + file_name + ".compiler_error";
}

bool Fileutil::iffileexe(const std::string &path_name) {
    struct stat st;
    return stat(path_name.c_str(), &st) == 0;
}

bool Fileutil::EnsureDir(const std::string &path_name) {
    if (mkdir(path_name.c_str(), 0755) == 0) {
        return true;
    }

    if (errno == EEXIST) {
        return true;
    }

    return false;
}

std::string Fileutil::UniqFileName() {
    if (!EnsureDir(temp_path)) {
        return "";
    }

    std::ostringstream dir_name;
    dir_name << time(nullptr) << "_" << getpid() << "_" << g_file_seq.fetch_add(1);

    std::string task_dir = temp_path + dir_name.str();
    if (!EnsureDir(task_dir)) {
        return "";
    }

    return dir_name.str() + "/main";
}

bool Fileutil::WriteFile(const std::string &file_name, const std::string &code) {
    std::ofstream ofs(file_name.c_str(), std::ios::out | std::ios::trunc);
    if (!ofs) {
        return false;
    }

    ofs << code;
    ofs.close();
    return true;
}

bool Fileutil::Readfile(const std::string &target, std::string &code, bool keep) {
    std::ifstream ifs(target.c_str());
    if (!ifs) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    if (keep) {
        code += content;
    } else {
        code = content;
    }
    return true;
}

std::string timeutil::gettime() {
    time_t t = time(nullptr);
    struct tm *tm_info = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

bool compilefile::CompileFile(const std::string &file_name) {
    unlink(File_name::Exe(file_name).c_str());

    pid_t pid = fork();
    if (pid < 0) {
        LOG_ERROR("fork failed");
        return false;
    }

    if (pid == 0) {
        umask(0);

        int stderr_fd = open(File_name::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (stderr_fd < 0) {
            LOG_ERROR("open compiler error file failed");
            exit(1);
        }

        dup2(stderr_fd, 2);
        close(stderr_fd);

        execlp("g++", "g++", "-o", File_name::Exe(file_name).c_str(),
               File_name::Src(file_name).c_str(), "-std=c++11", "-I.", nullptr);

        LOG_ERROR("start g++ failed");
        exit(1);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && Fileutil::iffileexe(File_name::Exe(file_name))) {
        LOG_INFO("compile success");
        return true;
    }

    LOG_ERROR("compile failed");
    return false;
}

bool File::FFile(const std::string &file_name) {
    return compilefile::CompileFile(file_name);
}
}
