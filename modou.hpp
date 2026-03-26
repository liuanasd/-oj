#pragma once
/**
 * @file modou.hpp
 * @brief 通用工具和定义头文件
 * @details 包含项目中使用的通用工具函数、命名空间定义和基础数据结构
 * @author 项目开发者
 * @date 2026-03-26
 */

#include <iostream>
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unordered_map>
#include <cassert>
#include <fstream>
#include <sstream>

/**
 * @namespace ns_util
 * @brief 工具函数命名空间
 * @details 包含字符串处理、文件操作等通用工具函数
 */
namespace ns_util {
    /**
     * @class StringUtil
     * @brief 字符串处理工具类
     * @details 提供字符串分割、转换等操作
     */
    class StringUtil {
    public:
        /**
         * @brief 分割字符串
         * @param str 待分割的字符串
         * @param out 存储分割结果的向量
         * @param sep 分隔符
         * @details 将字符串按照指定分隔符分割，并存储到向量中
         */
        static void SplitString(const std::string& str, std::vector<std::string>& out, const std::string& sep) {
            size_t start = 0;
            size_t end = str.find(sep);
            while (end != std::string::npos) {
                out.push_back(str.substr(start, end - start));
                start = end + sep.length();
                end = str.find(sep, start);
            }
            out.push_back(str.substr(start));
        }
    };

    /**
     * @class FileUtil
     * @brief 文件操作工具类
     * @details 提供文件读写、创建等操作
     */
    class FileUtil {
    public:
        /**
         * @brief 读取文件内容
         * @param file_path 文件路径
         * @return 文件内容字符串
         * @details 读取指定文件的全部内容并返回
         */
        static std::string ReadFile(const std::string& file_path) {
            std::ifstream ifs(file_path);
            if (!ifs) {
                return "";
            }
            std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            ifs.close();
            return content;
        }

        /**
         * @brief 读取文件内容到指定字符串
         * @param file_path 文件路径
         * @param content 存储文件内容的字符串指针
         * @param append 是否追加模式
         * @details 读取指定文件的全部内容并存储到指定字符串中
         */
        static void ReadFile(const std::string& file_path, std::string* content, bool append = false) {
            if (!content) {
                return;
            }
            std::ifstream ifs(file_path);
            if (!ifs) {
                return;
            }
            std::string file_content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            ifs.close();
            if (append) {
                *content += file_content;
            } else {
                *content = file_content;
            }
        }

        /**
         * @brief 写入文件内容
         * @param file_path 文件路径
         * @param content 要写入的内容
         * @return 是否写入成功
         * @details 将指定内容写入到指定文件中
         */
        static bool WriteFile(const std::string& file_path, const std::string& content) {
            std::ofstream ofs(file_path);
            if (!ofs) {
                return false;
            }
            ofs << content;
            ofs.close();
            return true;
        }
    };
}

/**
 * @namespace ns_log
 * @brief 日志命名空间
 * @details 提供日志输出功能
 */
namespace ns_log {
    /**
     * @brief 错误日志输出
     * @param file 文件名称
     * @param line 行号
     * @param time 时间戳
     * @param msg 错误信息
     * @details 输出格式化的错误日志
     */
    inline void Error(const std::string& file, int line, uint64_t time, const std::string& msg) {
        std::cerr << "[ERROR] " << file << " " << line << " " << time << " " << msg << std::endl;
    }

    /**
     * @brief 信息日志输出
     * @param file 文件名称
     * @param line 行号
     * @param time 时间戳
     * @param msg 信息内容
     * @details 输出格式化的信息日志
     */
    inline void Info(const std::string& file, int line, uint64_t time, const std::string& msg) {
        std::cout << "[INFO] " << file << " " << line << " " << time << " " << msg << std::endl;
    }
}
