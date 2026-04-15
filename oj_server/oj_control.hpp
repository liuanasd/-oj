#pragma once

#include "../compile_run.hpp"
#include "../log.hpp"
#include "../modou.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"

namespace ns_control {
using namespace ns_util;
using namespace ns_log;
using namespace ns_model;
using namespace ns_view;

class Control {
private:
    static void RemoveUserMain(std::string *code) {
        if (code == nullptr) {
            return;
        }

        size_t main_pos = code->find("int main(");
        if (main_pos == std::string::npos) {
            return;
        }

        size_t brace_pos = code->find('{', main_pos);
        if (brace_pos == std::string::npos) {
            return;
        }

        int deep = 0;
        size_t end_pos = brace_pos;
        for (; end_pos < code->size(); ++end_pos) {
            if ((*code)[end_pos] == '{') {
                ++deep;
            } else if ((*code)[end_pos] == '}') {
                --deep;
                if (deep == 0) {
                    ++end_pos;
                    break;
                }
            }
        }

        if (deep == 0 && end_pos <= code->size()) {
            code->erase(main_pos, end_pos - main_pos);
        }
    }

    static void ReplaceAll(std::string *text, const std::string &from, const std::string &to) {
        if (text == nullptr || from.empty()) {
            return;
        }

        size_t pos = 0;
        while ((pos = text->find(from, pos)) != std::string::npos) {
            text->replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    static bool BuildRunCode(const Question &q, const std::string &user_code, std::string *full_code) {
        if (full_code == nullptr) {
            return false;
        }

        *full_code = user_code;
        RemoveUserMain(full_code);

        std::string tail_code = q.tail;
        std::string old_include = "#include \"" + q.number + ".header.cpp\"";
        ReplaceAll(&tail_code, old_include, "");

        if (!full_code->empty() && full_code->back() != '\n') {
            *full_code += "\n";
        }
        *full_code += tail_code;
        return true;
    }

public:
    Model model_;
    View view_;

    bool AllQuestions(std::string *html) {
        std::vector<Question> questions;
        if (!model_.GetAllQuestions(questions)) {
            if (html != nullptr) {
                *html = "get question list failed";
            }
            return false;
        }

        view_.AllExpandHtml(questions, html);
        return true;
    }

    bool GetQuestion(const std::string &number, std::string *html) {
        Question q;
        if (!model_.GetOneQuestion(number, q)) {
            if (html != nullptr) {
                *html = "question not found";
            }
            return false;
        }

        view_.OneExpandHtml(q, html);
        return true;
    }

    bool GetJudgePage(const std::string &number, std::string *html) {
        Question q;
        if (!model_.GetOneQuestion(number, q)) {
            if (html != nullptr) {
                *html = "question not found";
            }
            return false;
        }

        view_.JudgePageHtml(q, html);
        return true;
    }

    bool Judge(const std::string &number, const std::string &code, ns_compile_run::JudgeResult *result, std::string *err_msg) {
        if (result == nullptr) {
            if (err_msg != nullptr) {
                *err_msg = "judge result is null";
            }
            return false;
        }

        if (code.empty()) {
            if (err_msg != nullptr) {
                *err_msg = "code is empty";
            }
            result->status = ns_compile_run::JUDGE_PARAM_ERROR;
            result->reason = "empty code";
            return false;
        }

        Question q;
        if (!model_.GetOneQuestion(number, q)) {
            if (err_msg != nullptr) {
                *err_msg = "question not found";
            }
            result->status = ns_compile_run::JUDGE_PARAM_ERROR;
            result->reason = "question not found";
            return false;
        }

        std::string full_code;
        if (!BuildRunCode(q, code, &full_code)) {
            if (err_msg != nullptr) {
                *err_msg = "build run code failed";
            }
            result->status = ns_compile_run::JUDGE_SYSTEM_ERROR;
            result->reason = "build run code failed";
            return false;
        }

        ns_compile_run::JudgeRequest request;
        request.code = full_code;
        request.input = "";
        request.cpu_limit = q.cpu_limit;
        request.mem_limit = q.mem_limit;

        bool ok = ns_compile_run::CompileRun::Judge(request, result);
        if (!ok && err_msg != nullptr && err_msg->empty()) {
            *err_msg = result->reason;
        }
        return ok;
    }

    void GetJudgeResult(const std::string &number, const ns_compile_run::JudgeResult &result, std::string *html) {
        view_.JudgeResultHtml(number, result, html);
    }
};
}
