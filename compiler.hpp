#pragma once
/**
 * @file compiler.hpp
 * @brief 编译器相关工具头文件
 * @details 包含文件名处理、文件操作、时间工具和编译功能的声明
 * @author 项目开发者
 * @date 2026-03-26
 */

#include "modou.hpp"
#include "log.hpp"

/**
 * @namespace ns_util
 * @brief 工具函数命名空间
 * @details 包含编译器相关的工具类和函数
 */
namespace ns_util {
    /**
     * @var temp_path
     * @brief 临时文件路径
     * @details 存储临时文件的目录路径
     */
    extern const std::string temp_path;

    /**
     * @class File_name
     * @brief 文件名处理类
     * @details 提供生成不同类型文件名的方法
     */
    class File_name {
    public:
        /**
         * @brief 为文件名添加后缀
         * @param file_name 基础文件名
         * @param suffix 后缀
         * @return 添加后缀后的完整文件名
         */
        static std::string add(const std::string &file_name, const std::string &suffix);

        /**
         * @brief 生成源文件路径
         * @param file_name 基础文件名
         * @return 源文件完整路径
         */
        static std::string Src(const std::string &file_name);

        /**
         * @brief 生成可执行文件路径
         * @param file_name 基础文件名
         * @return 可执行文件完整路径
         */
        static std::string Exe(const std::string &file_name);

        /**
         * @brief 生成标准错误输出文件路径
         * @param file_name 基础文件名
         * @return 标准错误输出文件完整路径
         */
        static std::string Stderr(const std::string &file_name);

        /**
         * @brief 生成标准输入文件路径
         * @param file_name 基础文件名
         * @return 标准输入文件完整路径
         */
        static std::string Stdin(const std::string &file_name);

        /**
         * @brief 生成标准输出文件路径
         * @param file_name 基础文件名
         * @return 标准输出文件完整路径
         */
        static std::string Stdout(const std::string &file_name);

        /**
         * @brief 生成编译错误文件路径
         * @param file_name 基础文件名
         * @return 编译错误文件完整路径
         */
        static std::string CompilerError(const std::string &file_name);
    };

    /**
     * @class Fileutil
     * @brief 文件操作工具类
     * @details 提供文件存在性检查、唯一文件名生成、文件读写等功能
     */
    class Fileutil {
    public:
        /**
         * @brief 检查文件是否存在且可执行
         * @param path_name 文件路径
         * @return 是否存在且可执行
         */
        static bool iffileexe(const std::string &path_name);

        /**
         * @brief 生成唯一文件名
         * @return 唯一文件名
         * @details 使用时间戳和随机数生成唯一文件名
         */
        static std::string UniqFileName();

        /**
         * @brief 写入文件
         * @param file_name 文件路径
         * @param code 要写入的内容
         * @return 是否写入成功
         */
        static bool WriteFile(const std::string &file_name, const std::string &code);

        /**
         * @brief 读取文件
         * @param target 文件路径
         * @param code 存储文件内容的字符串
         * @param keep 是否保持原有内容
         * @return 是否读取成功
         */
        static bool Readfile(const std::string &target, std::string &code, bool keep = false);
    };

    /**
     * @class timeutil
     * @brief 时间工具类
     * @details 提供时间相关的功能
     */
    class timeutil {
    public:
        /**
         * @brief 获取当前时间
         * @return 当前时间字符串
         */
        static std::string gettime();
    };

    /**
     * @class compilefile
     * @brief 编译文件类
     * @details 提供文件编译功能
     */
    class compilefile {
    public:
        /**
         * @brief 编译文件
         * @param file_name 要编译的文件名
         * @return 是否编译成功
         */
        static bool CompileFile(const std::string &file_name);
    };

    /**
     * @class File
     * @brief 文件检查类
     * @details 提供文件存在性检查功能
     */
    class File {
    public:
        /**
         * @brief 检查文件是否存在
         * @param file_name 文件路径
         * @return 是否存在
         */
        bool FFile(const std::string &file_name);
    };

    /**
     * @def NO_REDEFINE
     * @brief 防止重复定义的宏
     */
    #define NO_REDEFINE 1
}
