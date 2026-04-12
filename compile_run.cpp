#include "compile_run.hpp"

namespace json_util {
    
    std::string unescape(const std::string &s) {
        std::string res;
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == '\\' && i + 1 < s.size()) {
                switch (s[i + 1]) {
                    case 'n': res += '\n'; i++;
 break;
                    case 'r': res += '\r'; i++;
 break;
                    case 't': res += '\t'; i++;
 break;
                    case 'b': res += '\b'; i++;
 break;
                    case 'f': res += '\f'; i++;
 break;
                    case '"': res += '"'; i++;
 break;
                    case '\\': res += '\\'; i++;
 break;
                    default: res += s[i];
 break;
                }
            } else {
                res += s[i];
            }
        }
        return res;
    }

    void parse(const std::string &json, std::string &code, std::string &input, int &cpu, int &mem) {
        
        code = "";
        input = "";
        cpu = 1; 
        mem = 128; 
        
        
        size_t code_start = json.find("\"code\":\"");
        if (code_start != std::string::npos) {
            code_start += 8; 
            size_t code_end = code_start;
            int quote_count = 1;
            while (code_end < json.size() && quote_count > 0) {
                if (json[code_end] == '\\' && code_end + 1 < json.size() && json[code_end + 1] == '"') {
                    code_end += 2; 
                } else if (json[code_end] == '"') {
                    quote_count--;
                    code_end++;
                } else {
                    code_end++;
                }
            }
            if (quote_count == 0) {
                code = json.substr(code_start, code_end - code_start - 1);
                
                code = unescape(code);
            }
        }
        
        
        size_t input_start = json.find("\"input\":\"");
        if (input_start != std::string::npos) {
            input_start += 9; 
            size_t input_end = input_start;
            int quote_count = 1;
            while (input_end < json.size() && quote_count > 0) {
                if (json[input_end] == '\\' && input_end + 1 < json.size() && json[input_end + 1] == '"') {
                    input_end += 2; 
                } else if (json[input_end] == '"') {
                    quote_count--;
                    input_end++;
                } else {
                    input_end++;
                }
            }
            if (quote_count == 0) {
                input = json.substr(input_start, input_end - input_start - 1);
            }
        }
        
        
        size_t cpu_start = json.find("\"cpu\":");
        if (cpu_start != std::string::npos) {
            cpu_start += 6; 
            size_t cpu_end = json.find(",", cpu_start);
            if (cpu_end == std::string::npos) cpu_end = json.find("}", cpu_start);
            if (cpu_end != std::string::npos) {
                std::string cpu_str = json.substr(cpu_start, cpu_end - cpu_start);
                cpu = std::stoi(cpu_str);
            }
        }
        
        
        size_t mem_start = json.find("\"mem\":");
        if (mem_start != std::string::npos) {
            mem_start += 6; 
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
        oss << "\"output\": \"" << output << "\",";
        oss << "\"error\": \"" << error << "\"";
        oss << "}";
        return oss.str();
    }
}

namespace ns_compile_run {
    void CompileRun::Stat(const std::string &in_json, std::string &out_json) {
        std::string code, input;
        int cpu, mem;
        
        
        json_util::parse(in_json, code, input, cpu, mem);
        
        
        if(code.size() == 0){
            out_json = json_util::generate(1, "提交代码错误");
            return;
        }
        
        std::string file_name = ns_util::Fileutil::UniqFileName(); 
        
        
        std::ofstream code_file(ns_util::File_name::Src(file_name));
        if(!code_file){
            out_json = json_util::generate(-2, "未知错误");
            return;
        }
        code_file << code;
        code_file.close();
        
        
        std::ofstream input_file(ns_util::File_name::Stdin(file_name));
        input_file << input;
        input_file.close();
        
        
        if(!ns_util::compilefile::CompileFile(file_name)){
            
            std::string err_content;
            ns_util::Fileutil::Readfile(ns_util::File_name::CompilerError(file_name), err_content, true);
            out_json = json_util::generate(-3, err_content);
            return;
        }
        
        
        int run_code = ns_runner::Runner::run(file_name, cpu, mem);
        if(run_code < 0){
            
            out_json = json_util::generate(-2, "未知错误");
            return;
        }
        else if(run_code > 0){
            
            out_json = json_util::generate(-4, "运行错误");
            return;
        }
        else{
            
            std::string _stdout;
            ns_util::Fileutil::Readfile(ns_util::File_name::Stdout(file_name), _stdout, true);
            std::string _stderr;
            ns_util::Fileutil::Readfile(ns_util::File_name::Stderr(file_name), _stderr, true);
            out_json = json_util::generate(0, "运行成功", _stdout, _stderr);
        }
    }
}


