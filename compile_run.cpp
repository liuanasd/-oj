#include "compile_run.hpp"

namespace json_util {
    void parse(const std::string &json, std::string &code, std::string &input, int &cpu, int &mem) {
        // 初始化默认值
        code = "";
        input = "";
        cpu = 1; // 默认CPU限制为1秒，防止程序无限运行
        mem = 128; // 默认内存限制为128MB，防止内存溢出
        
        // 提取code字段
        size_t code_start = json.find("\"code\":\"");
        if (code_start != std::string::npos) {
            code_start += 8; // 跳过 "code":" 前缀
            size_t code_end = json.find("\"", code_start);
            if (code_end != std::string::npos) {
                code = json.substr(code_start, code_end - code_start);
            }
        }
        
        // 提取input字段
        size_t input_start = json.find("\"input\":\"");
        if (input_start != std::string::npos) {
            input_start += 9; // 跳过 "input":" 前缀
            size_t input_end = json.find("\"", input_start);
            if (input_end != std::string::npos) {
                input = json.substr(input_start, input_end - input_start);
            }
        }
        
        // 提取cpu字段
        size_t cpu_start = json.find("\"cpu\":");
        if (cpu_start != std::string::npos) {
            cpu_start += 6; // 跳过 "cpu": 前缀
            size_t cpu_end = json.find(",", cpu_start);
            if (cpu_end == std::string::npos) cpu_end = json.find("}", cpu_start);
            if (cpu_end != std::string::npos) {
                std::string cpu_str = json.substr(cpu_start, cpu_end - cpu_start);
                cpu = std::stoi(cpu_str);
            }
        }
        
        // 提取mem字段
        size_t mem_start = json.find("\"mem\":");
        if (mem_start != std::string::npos) {
            mem_start += 6; // 跳过 "mem": 前缀
            size_t mem_end = json.find(",", mem_start);
            if (mem_end == std::string::npos) mem_end = json.find("}", mem_start);
            if (mem_end != std::string::npos) {
                std::string mem_str = json.substr(mem_start, mem_end - mem_start);
                mem = std::stoi(mem_str);
            }
        }
    }
    
    std::string generate(int status, const std::string &reason, const std::string &output, const std::string &error) {
        std::ostringstream oss;
        oss << "{";
        oss << "\"status\": " << status << ",";
        oss << "\"reason\": \"" << reason << "\",";
        oss << "\"output\": \"" << output << ",";
        oss << "\"error\": \"" << error << "\"";
        oss << "}";
        return oss.str();
    }
}

namespace ns_compile_run {
    void CompileRun::Stat(const std::string &in_json, std::string &out_json) {
        std::string code, input;
        int cpu, mem;
        
        // 解析输入JSON，提取必要信息
        json_util::parse(in_json, code, input, cpu, mem);
        
        // 检查代码是否为空，为空则返回错误
        if(code.size() == 0){
            out_json = json_util::generate(1, "提交代码错误");
            return;
        }
        
        std::string file_name = "code"; // 使用固定文件名，简化处理
        
        // 写入代码文件到磁盘
        std::ofstream code_file(ns_util::File_name::Src(file_name));
        if(!code_file){
            out_json = json_util::generate(-2, "未知错误");
            return;
        }
        code_file << code;
        code_file.close();
        
        // 写入输入文件到磁盘
        std::ofstream input_file(ns_util::File_name::Stdin(file_name));
        input_file << input;
        input_file.close();
        
        // 编译文件，检查编译是否成功
        if(!ns_util::compilefile::CompileFile("code")){
            // 读取编译错误信息
            std::string err_content;
            ns_util::Fileutil::Readfile(ns_util::File_name::CompilerError(file_name), err_content, true);
            out_json = json_util::generate(-3, err_content);
            return;
        }
        
        // 运行文件，设置资源限制
        int run_code = ns_runner::Runner::run("code", cpu, mem);
        if(run_code < 0){
            // 运行过程中出现未知错误
            out_json = json_util::generate(-2, "未知错误");
            return;
        }
        else if(run_code > 0){
            // 程序运行出错（如段错误、超时等）
            out_json = json_util::generate(-4, "运行错误");
            return;
        }
        else{
            // 运行成功，读取输出和错误信息
            std::string _stdout;
            ns_util::Fileutil::Readfile(ns_util::File_name::Stdout(file_name), _stdout, true);
            std::string _stderr;
            ns_util::Fileutil::Readfile(ns_util::File_name::Stderr(file_name), _stderr, true);
            out_json = json_util::generate(0, "运行成功", _stdout, _stderr);
        }
    }
}


