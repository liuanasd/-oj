#pragma once
/**
 * @file runner.hpp
 * @brief 程序运行器头文件
 * @details 负责限制程序运行时的资源使用，确保安全执行，包括CPU时间和内存限制
 * @author 项目开发者
 * @date 2026-03-26
 */

#include "modou.hpp"
#include "compiler.hpp"

/**
 * @namespace ns_runner
 * @brief 运行器命名空间
 * @details 负责限制程序运行时的资源使用，确保安全执行
 */
namespace ns_runner
{
    using namespace ns_log;
    using namespace ns_util;
    
    /**
     * @class Runner
     * @brief 运行器类
     * @details 提供静态方法设置资源限制和运行程序
     */
    class Runner
    {
    public:
        /**
         * @brief 设置进程资源限制
         * @param cpu_limit CPU时间限制（秒）
         * @param mem_limit 内存限制（MB）
         * @details 通过setrlimit系统调用设置进程的CPU和内存限制，确保程序不会无限占用系统资源
         */
        static void SetProcLimit(int cpu_limit, int mem_limit){
            // 约束CPU限制
            struct rlimit r;
            r.rlim_cur = cpu_limit; // 当前限制
            r.rlim_max = RLIM_INFINITY; // 最大限制（无限制）
            setrlimit(RLIMIT_CPU, &r);
            
            // 约束内存限制
            r.rlim_cur = mem_limit * 1024 * 1024; // 转换为字节
            r.rlim_max = RLIM_INFINITY; // 最大限制（无限制）
            setrlimit(RLIMIT_AS, &r); // RLIMIT_AS 限制地址空间大小
        }
        
        /**
         * @brief 构造函数
         */
        Runner();
        
        /**
         * @brief 析构函数
         */
        ~Runner();
        
        /**
         * @brief 运行程序
         * @param file_name 文件名（不带后缀）
         * @param cpu_limit CPU时间限制（秒）
         * @param mem_limit 内存限制（MB）
         * @return 运行结果
         * @retval 0 正常运行结束
         * @retval -1 文件打开失败
         * @retval -2 子进程创建失败
         * @retval 其他 程序异常退出的信号值
         * @details 在受限环境中运行程序，重定向输入输出，确保程序在安全的资源限制下执行
         */
        static int run(const std::string &file_name, int cpu_limit, int mem_limit)
        {
            // 生成文件路径
            std::string exepg = File_name::Exe(file_name); // 可执行文件路径
            std::string _Stdin = File_name::Stdin(file_name); // 标准输入文件路径
            std::string _Stdout = File_name::Stdout(file_name); // 标准输出文件路径
            std::string _Stderr = File_name::Stderr(file_name); // 标准错误文件路径
            
            // 设置文件权限掩码，确保生成的文件可写
            umask(0);
            
            // 打开输入输出文件
            int stdin_fd = open(_Stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int stdout_fd = open(_Stdout.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            int stderr_fd = open(_Stderr.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            
            // 检查文件打开是否成功
            if (stdin_fd < 0 || stdout_fd < 0 || stderr_fd < 0)
            {
                return -1; // 文件打开失败
            }
            
            // 创建子进程，隔离运行环境
            pid_t pid = fork();
            if (pid < 0)
            {
                // 子进程创建失败，关闭文件
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                return -2;
            }
            else if (pid == 0)
            {
                // 子进程：重定向标准输入输出
                dup2(stdin_fd, 0); // 重定向标准输入
                dup2(stdout_fd, 1); // 重定向标准输出
                dup2(stderr_fd, 2); // 重定向标准错误
                
                // 设置资源限制，确保程序不会占用过多系统资源
                SetProcLimit(cpu_limit, mem_limit);
                
                // 执行可执行文件
                execl(exepg.c_str(), exepg.c_str(), nullptr);
                
                // 如果execl返回，说明执行失败
                exit(1);
            }
            else
            {
                // 父进程：关闭文件
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                
                // 等待子进程结束
                int status = 0;
                waitpid(pid, &status, 0);
                
                // 返回子进程的退出状态，只保留低7位（信号值）
                return status & 0x7F;
            }
        }
    };
};
