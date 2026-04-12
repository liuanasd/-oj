#include "compiler.hpp"
#include "modou.hpp"
#include "runner.hpp"
#include <fstream>
#include <iostream>
using namespace ns_util;
using namespace ns_runner;
using namespace ns_log;

/**
 * @brief 读取并打印文件内容
 * @param file_name 文件路径
 * @param description 文件描述
 */
void read_and_print_file(const std::string &file_name, const std::string &description) {
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::cout << description << ":" << std::endl;
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "Could not open " << file_name << std::endl;
    }
}

/**
 * @brief 主函数
 * @return 退出状态码
 */
int main(){
    
    compilefile::CompileFile("code");
    
    
    Runner::run("code", 1, 128);
    
    
    read_and_print_file(File_name::Stdout("code"), "Standard Output");
    read_and_print_file(File_name::Stderr("code"), "Standard Error");
    
    return 0;
}
