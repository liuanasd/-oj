#include "modou.hpp"
#include "compiler.hpp"
#include "log.hpp"
#include <fstream>

using namespace ns_util;
using namespace ns_log;

std::string tete = "./";

namespace ns_util {

std::string File_name::add(const std::string &file_name, const std::string &suffix)
{
    std::string path_name;
    path_name += tete;
    path_name += file_name;
    path_name += suffix;
    return path_name; 
}

std::string File_name::Src(const std::string &file_name)
{
    return add(file_name, ".cpp");
}

std::string File_name::Exe(const std::string &file_name)
{
    return add(file_name, "");
}

std::string File_name::Stderr(const std::string &file_name)
{
    return add(file_name, ".stderr");
}

std::string File_name::Stdin(const std::string &file_name)
{
    return add(file_name, ".stdin");
}

std::string File_name::Stdout(const std::string &file_name)
{
    return add(file_name, ".stdout");
}

std::string File_name::CompilerError(const std::string &file_name)
{
    return add(file_name, ".compiler_error");
}

bool Fileutil::iffileexe(const std::string &path_name)
{
    struct stat st;
    if (stat(path_name.c_str(), &st) == 0)
    {
        return true;
    }
    return false;
}

std::string timeutil::gettime()
{
    struct timeval _time;
    gettimeofday(&_time, nullptr);
    return std::to_string(_time.tv_sec);
}

bool Fileutil::WriteFile(const std::string &file_name, const std::string &code){
    std::ofstream out(file_name);
    if(!out){
        std::cerr << "WriteFile: " << file_name << std::endl;
        return false;
    }
    out.write(code.c_str(), code.size());
    out.close();
    return true;
}

bool Fileutil::Readfile(const std::string &target, std::string &code, bool keep){
    std::ifstream in(target);
    if(!in.is_open()){
        std::cerr << "Readfile: " << target << std::endl;
        return false;
    }
    std::string line;
    while(std::getline(in, line)){
        code += line;
        code += (keep ? "\n" : "");
    }
    in.close();
    return true;
}
class StringUtil{
    public:
    static void SplitString(const std::string &str, std::vector<std::string> &parts, std::string ser){
       
    }
    static void SplitSting(const std::string &str, std::vector<std::string>* target, const std::string &sep){
        if (!target) return;
        
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, sep[0])) {
            if (!token.empty()) {
                target->push_back(token);
            }
        }
    }
};
}


