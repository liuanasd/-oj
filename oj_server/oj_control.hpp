#pragma once
#include "../modou.hpp"
#include "../log.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
namespace ns_control {
    using namespace ns_util;
    using namespace ns_log;
    using namespace ns_model;
    using namespace ns_view;
    class Control {
    public:
        Control() {}
        ~Control() {}
        Model model_;
        View view_;
        bool AllQuestions(std::string *html){
            std::vector<ns_model::Question> questions;
            if(model_.GetAllQuestions(questions)){
                view_.AllExpandHtml(questions, html);
            }
            else{
                *html="获取题目列表失败";
                return false;
            }
            return true;
        }
        bool GetQuestion(std::string number, std::string *html){
            ns_model::Question q;
            if(model_.GetOneQuestion(number, q)){
                view_.OneExpandHtml(q, html);
            }
            else{
                *html="题目不存在";
                return false;
            }
            return true;
        }
    };
}