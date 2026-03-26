#pragma once
#include "../modou.hpp"
#include "../compiler.hpp"

// 模型命名空间，负责题目数据的管理和加载
namespace ns_model{
    using namespace ns_util;
    
    // 题目结构体，存储题目的完整信息
    struct Question{
        std::string number;     // 题目编号
        std::string title;      // 题目标题
        std::string star;       // 题目难度
        int cpu_limit;          // CPU时间限制（秒）
        int mem_limit;          // 内存限制（MB）
        std::string desc;       // 题目描述
        std::string header;     // 代码头部（用户需要实现的函数）
        std::string tail;       // 代码尾部（测试用例和main函数）
    };
    
    // 题目列表文件路径
    // 说明：存储题目基本信息的文件，每行包含题目编号、标题、难度、CPU限制、内存限制
    const std::string question_list = "./question";
    
    // 题目文件路径前缀
    // 说明：存储题目详细信息的目录，包含题目描述、代码文件等
    const std::string question_path = "./questions/";
    
    // 模型类，负责加载和管理题目数据
    class Model{
    private:
        // 题目存储容器，使用unordered_map提高查找效率
        std::unordered_map<std::string, Question> questions;
        
    public:
        // 构造函数
        Model(){
            // 加载题目列表
            LoadQuestionlist(question_list);
        }
        
        // 析构函数
        ~Model(){}
        
        // 加载题目列表
        // 参数：question_list - 题目列表文件路径
        // 返回值：true表示加载成功，false表示加载失败
        bool LoadQuestionlist(const std::string &question_list){
            // 打开题目列表文件
            std::ifstream ifs(question_list);
            if(!ifs){
                // 文件打开失败，返回false
                return false;
            }
            
            std::string line;
            // 逐行读取题目信息
            while(getline(ifs, line)){
                std::vector<std::string> tonkens;
                // 分割行数据，使用空格作为分隔符
                StringUtil::SplitString(line, tonkens, " ");
                
                // 验证数据格式，每行应该有5个字段
                if(tonkens.size() != 5){
                    // 格式不正确，跳过该行
                    continue;
                }
                
                // 创建题目对象并填充基本信息
                Question q;
                q.number = tonkens[0];      // 题目编号
                q.title = tonkens[1];       // 题目标题
                q.star = tonkens[2];        // 题目难度
                q.cpu_limit = stoi(tonkens[3]);  // CPU限制
                q.mem_limit = stoi(tonkens[4]);  // 内存限制
                
                // 构建题目文件路径
                std::string desc_path = question_path + q.number + ".desc";
                std::string header_path = question_path + q.number + ".header.cpp";
                std::string tail_path = question_path + q.number + ".tail.cpp";
                
                // 读取题目详细信息
                q.desc = FileUtil::ReadFile(desc_path);
                q.header = FileUtil::ReadFile(header_path);
                q.tail = FileUtil::ReadFile(tail_path);
                
                // 将题目添加到容器中
                questions[q.number] = q;
            }
            
            // 关闭文件
            ifs.close();
            return true;
        }
        
        // 加载单个题目
        // 参数：number - 题目编号
        // 返回值：true表示加载成功，false表示加载失败
        bool LoadQuestion(const std::string &number){
            return true;
        }
        
        // 获取所有题目
        // 参数：questions - 存储题目列表的向量
        // 返回值：true表示获取成功，false表示获取失败
        bool GetAllQuestions(std::vector<Question> &questions){
            // 清空向量
            questions.clear();
            
            // 遍历容器，添加所有题目
            for(auto& qque : this->questions){
                questions.push_back(qque.second);
            }
            return true;
        }
        
        // 获取单个题目
        // 参数：number - 题目编号
        //       q - 存储题目信息的引用
        // 返回值：true表示获取成功，false表示题目不存在
        bool GetOneQuestion(const std::string &number, Question &q){
            // 查找题目
            const auto& que = questions.find(number);
            if(que == questions.end()){
                // 题目不存在
                return false;
            }
            
            // 复制题目信息
            q = que->second;
            return true;
        }
    };
}