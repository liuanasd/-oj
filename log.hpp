#pragma once
/**
 * @file log.hpp
 * @brief 日志系统头文件
 * @details 提供不同级别的日志输出功能，支持自动添加文件、行号和时间戳
 * @author 项目开发者
 * @date 2026-03-26
 */

#include "modou.hpp"

/**
 * @namespace ns_log
 * @brief 日志命名空间
 * @details 提供日志输出相关的枚举、函数和宏定义
 */
namespace ns_log {
    /**
     * @enum 日志级别枚举
     * @details 定义不同级别的日志
     */
    enum {
        INFO,    
        DEBUG,   
        ERROR,   
        FATAL    
    };

    /**
     * @brief 日志输出函数
     * @param level 日志级别字符串
     * @param file_name 文件名
     * @param line 行号
     * @return 标准输出流引用
     * @details 生成带有时间戳、文件和行号的日志前缀
     */
    inline std::ostream& LOG(const std::string &level, const std::string &file_name, int line) {
        std::string message;
        time_t t = time(nullptr);
        message += "[" + level + "] "; 
        message += file_name + " "; 
        message += std::to_string(line) + " "; 
        message += std::to_string(t); 
        std::cout << message << " "; 
        return std::cout;
    }

    /**
     * @brief 信息级别日志宏
     * @param msg 日志消息
     * @details 自动添加INFO级别前缀和位置信息
     */
    #define LOG_INFO(msg)    ns_log::LOG("INFO", __FILE__, __LINE__) << msg << std::endl

    /**
     * @brief 调试级别日志宏
     * @param msg 日志消息
     * @details 自动添加DEBUG级别前缀和位置信息
     */
    #define LOG_DEBUG(msg)   ns_log::LOG("DEBUG", __FILE__, __LINE__) << msg << std::endl

    /**
     * @brief 错误级别日志宏
     * @param msg 日志消息
     * @details 自动添加ERROR级别前缀和位置信息
     */
    #define LOG_ERROR(msg)   ns_log::LOG("ERROR", __FILE__, __LINE__) << msg << std::endl

    /**
     * @brief 致命错误级别日志宏
     * @param msg 日志消息
     * @details 自动添加FATAL级别前缀和位置信息
     */
    #define LOG_FATAL(msg)   ns_log::LOG("FATAL", __FILE__, __LINE__) << msg << std::endl
};
