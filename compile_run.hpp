/**
 * @file compile_run.hpp
 * @brief 编译运行核心功能头文件
 * @details 负责协调代码编译和运行的整个流程，包括JSON解析、代码编译、程序运行和结果返回
 * @author 项目开发者
 * @date 2026-03-26
 */

#include "compiler.hpp"
#include "modou.hpp"
#include "runner.hpp"
#include <fstream>
#include <sstream>

using namespace ns_util;
using namespace ns_log;
using namespace ns_runner;

/**
 * @namespace json_util
 * @brief JSON工具命名空间
 * @details 提供简单的JSON解析和生成功能，用于处理编译运行的输入输出
 */
namespace json_util {
    /**
     * @brief 解析JSON字符串
     * @param json 包含代码、输入、CPU限制和内存限制的JSON字符串
     * @param code 用于存储提取的代码内容
     * @param input 用于存储提取的输入内容
     * @param cpu 用于存储提取的CPU时间限制（秒）
     * @param mem 用于存储提取的内存限制（MB）
     * @details 使用简单的字符串操作解析JSON，实际项目中应使用更健壮的解析库
     */
    void parse(const std::string &json, std::string &code, std::string &input, int &cpu, int &mem);
    
    /**
     * @brief 生成JSON字符串
     * @param status 状态码（0表示成功，非0表示失败）
     * @param reason 原因描述
     * @param output 程序输出内容
     * @param error 错误信息
     * @return 生成的JSON字符串
     * @details 用于将编译运行结果转换为JSON格式，方便网络传输
     */
    std::string generate(int status, const std::string &reason, const std::string &output = "", const std::string &error = "");
}

/**
 * @namespace ns_compile_run
 * @brief 编译运行命名空间
 * @details 负责协调代码编译和运行的整个流程
 */
namespace ns_compile_run{
    /**
     * @class CompileRun
     * @brief 编译运行类
     * @details 提供静态方法处理编译运行请求，返回JSON格式的结果
     */
    class CompileRun{
    public:
        /**
         * @brief 处理编译运行请求
         * @param in_json 输入JSON字符串，包含代码、输入、CPU限制和内存限制
         * @param out_json 输出JSON字符串，用于存储运行结果
         * @details 完整处理从解析输入到返回结果的整个流程，包括代码编译、资源限制运行和结果收集
         */
        static void Stat(const std::string &in_json, std::string &out_json);
    };
}
