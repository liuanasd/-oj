#include "compiler.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include "log.hpp"

using namespace ns_log;
using namespace ns_util;

namespace ns_util {
    
    const std::string temp_path = "./";
    
    
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
    
    std::string Fileutil::UniqFileName() {
        std::stringstream ss;
        ss << time(nullptr) << "_" << rand();
        return ss.str();
    }
    
    bool Fileutil::WriteFile(const std::string &file_name, const std::string &code) {
        std::ofstream ofs(file_name);
        if (!ofs) {
            return false;
        }
        ofs << code;
        ofs.close();
        return true;
    }
    
    bool Fileutil::Readfile(const std::string &target, std::string &code, bool keep) {
        std::ifstream ifs(target);
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
        struct tm *tm = localtime(&t);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
        return buf;
    }








bool compilefile::CompileFile(const std::string &file_name)
{
    
    unlink(File_name::Exe(file_name).c_str());
    
    
    pid_t pid = fork();
    if (pid < 0)
    {
        LOG_ERROR("创建子进程失败");
        return false;
    }
    else if (pid == 0)
    {
        
        umask(0);
        
        
        int _stderr = open(File_name::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (_stderr < 0)
        {
            LOG_ERROR("没有成功创建stderr文件");
            exit(1);
        }
        
        
        dup2(_stderr, 2);
        close(_stderr);

        
        
        
        
        
        
        
        execlp("g++", "g++", "-o", File_name::Exe(file_name).c_str(),
               File_name::Src(file_name).c_str(), "-std=c++11", nullptr);

        
        LOG_ERROR("g++启动失败");
        exit(1);
    }
    else
    {
        
        int status = 0;
        waitpid(pid, &status, 0);
        
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && Fileutil::iffileexe(File_name::Exe(file_name)))
        {
            LOG_INFO("编译成功");
            return true;
        }
    }
    
    
    LOG_ERROR("没有可执行程序");
    return false;
}








bool File::FFile(const std::string &file_name)
{
    return compilefile::CompileFile(file_name);
}

}
