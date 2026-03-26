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
    // 临时文件路径
    const std::string temp_path = "./";
    
    // File_name类实现
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
    
    // Fileutil类实现
    bool Fileutil::iffileexe(const std::string &path_name) {
        struct stat st;
        return stat(path_name.c_str(), &st) == 0 && (st.st_mode & S_IXUSR);
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
    
    // timeutil类实现
    std::string timeutil::gettime() {
        time_t t = time(nullptr);
        struct tm *tm = localtime(&t);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
        return buf;
    }

// 编译文件
// 参数：
// - file_name: 文件名（不带后缀）
// 返回值：
// - true: 编译成功
// - false: 编译失败
// 说明：使用子进程执行g++编译命令，将错误输出重定向到文件
bool compilefile::CompileFile(const std::string &file_name)
{
    // 创建子进程，用于隔离编译环境
    pid_t pid = fork();
    if (pid < 0)
    {
        LOG_ERROR("创建子进程失败");
        return false;
    }
    else if (pid == 0)
    {
        // 子进程：设置文件权限掩码，确保生成的文件可写
        umask(0);
        
        // 打开编译错误文件，用于存储编译错误信息
        int _stderr = open(File_name::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (_stderr < 0)
        {
            LOG_ERROR("没有成功创建stderr文件");
            exit(1);
        }
        
        // 重定向标准错误到编译错误文件，这样编译错误会被保存
        dup2(_stderr, 2);
        close(_stderr);

        // 执行g++编译命令
        // 参数说明：
        // - g++: 编译器命令
        // - -o: 指定输出文件名
        // - File_name::Exe(file_name): 可执行文件名
        // - File_name::Src(file_name): 源代码文件名
        // - -std=c++11: 使用C++11标准
        execlp("g++", "g++", "-o", File_name::Exe(file_name).c_str(),
               File_name::Src(file_name).c_str(), "-std=c++11", nullptr);

        // 如果execlp返回，说明执行失败
        LOG_ERROR("g++启动失败");
        exit(1);
    }
    else
    {
        // 父进程：等待子进程结束，获取编译结果
        waitpid(pid, nullptr, 0);
        
        // 检查可执行文件是否生成，判断编译是否成功
        if (Fileutil::iffileexe(File_name::Exe(file_name)))
        {
            LOG_INFO("编译成功");
            return true;
        }
    }
    
    // 编译失败，返回false
    LOG_ERROR("没有可执行程序");
    return false;
}

// 编译文件（File类包装方法）
// 参数：
// - file_name: 文件名（不带后缀）
// 返回值：
// - true: 编译成功
// - false: 编译失败
// 说明：包装compilefile::CompileFile方法，提供统一接口
bool File::FFile(const std::string &file_name)
{
    return compilefile::CompileFile(file_name);
}

}
