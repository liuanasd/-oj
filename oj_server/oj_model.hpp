#pragma once
#include "../modou.hpp"
#include "../compiler.hpp"


namespace ns_model{
    using namespace ns_util;
    
    
    struct Question{
        std::string number;     
        std::string title;      
        std::string star;       
        int cpu_limit;          
        int mem_limit;          
        std::string desc;       
        std::string header;     
        std::string tail;       
    };
    
    
    
    const std::string question_list = "./question";
    
    
    
    const std::string question_path = "./questions/";
    
    
    class Model{
    private:
        
        std::unordered_map<std::string, Question> questions;
        
    public:
        
        Model(){
            
            LoadQuestionlist(question_list);
        }
        
        
        ~Model(){}
        
        
        
        
        bool LoadQuestionlist(const std::string &question_list){
            
            std::ifstream ifs(question_list);
            if(!ifs){
                
                return false;
            }
            
            std::string line;
            
            while(getline(ifs, line)){
                std::vector<std::string> tonkens;
                
                StringUtil::SplitString(line, tonkens, " ");
                
                
                if(tonkens.size() != 5){
                    
                    continue;
                }
                
                
                Question q;
                q.number = tonkens[0];      
                q.title = tonkens[1];       
                q.star = tonkens[2];        
                q.cpu_limit = stoi(tonkens[3]);  
                q.mem_limit = stoi(tonkens[4]);  
                
                
                std::string desc_path = question_path + q.number + ".desc";
                std::string header_path = question_path + q.number + ".header.cpp";
                std::string tail_path = question_path + q.number + ".tail.cpp";
                
                
                q.desc = FileUtil::ReadFile(desc_path);
                q.header = FileUtil::ReadFile(header_path);
                q.tail = FileUtil::ReadFile(tail_path);
                
                
                questions[q.number] = q;
            }
            
            
            ifs.close();
            return true;
        }
        
        
        
        
        bool LoadQuestion(const std::string &number){
            return true;
        }
        
        
        
        
        bool GetAllQuestions(std::vector<Question> &questions){
            
            questions.clear();
            
            
            for(auto& qque : this->questions){
                questions.push_back(qque.second);
            }
            return true;
        }
        
        
        
        
        
        bool GetOneQuestion(const std::string &number, Question &q){
            
            const auto& que = questions.find(number);
            if(que == questions.end()){
                
                return false;
            }
            
            
            q = que->second;
            return true;
        }
    };
}