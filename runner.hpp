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
            
            struct rlimit r;
            r.rlim_cur = cpu_limit; 
            r.rlim_max = RLIM_INFINITY; 
            setrlimit(RLIMIT_CPU, &r);
            
            
            r.rlim_cur = mem_limit * 1024 * 1024; 
            r.rlim_max = RLIM_INFINITY; 
            setrlimit(RLIMIT_AS, &r); 
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
            
            std::string exepg = File_name::Exe(file_name); 
            std::string _Stdin = File_name::Stdin(file_name); 
            std::string _Stdout = File_name::Stdout(file_name); 
            std::string _Stderr = File_name::Stderr(file_name); 
            
            
            umask(0);
            
            
            int stdin_fd = open(_Stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int stdout_fd = open(_Stdout.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            int stderr_fd = open(_Stderr.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            
            
            if (stdin_fd < 0 || stdout_fd < 0 || stderr_fd < 0)
            {
                return -1; 
            }
            
            
            pid_t pid = fork();
            if (pid < 0)
            {
                
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                return -2;
            }
            else if (pid == 0)
            {
                
                dup2(stdin_fd, 0); 
                dup2(stdout_fd, 1); 
                dup2(stderr_fd, 2); 
                
                
                SetProcLimit(cpu_limit, mem_limit);
                
                
                execl(exepg.c_str(), exepg.c_str(), nullptr);
                
                
                exit(1);
            }
            else
            {
                
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                
                
                int status = 0;
                waitpid(pid, &status, 0);
                
                
                return status & 0x7F;
            }
        }
    };
};
