#pragma once

#include "../compile_run.hpp"
#include "../log.hpp"
#include "oj_model.hpp"

namespace ns_view {
using namespace ns_model;

class View {
private:
    static std::string EscapeHtml(const std::string &text) {
        std::string result;
        for (size_t i = 0; i < text.size(); ++i) {
            switch (text[i]) {
            case '&':
                result += "&amp;";
                break;
            case '<':
                result += "&lt;";
                break;
            case '>':
                result += "&gt;";
                break;
            case '"':
                result += "&quot;";
                break;
            case '\'':
                result += "&#39;";
                break;
            default:
                result += text[i];
                break;
            }
        }
        return result;
    }

    static std::string HtmlText(const std::string &text) {
        std::string result = EscapeHtml(text);
        std::string::size_type pos = 0;
        while ((pos = result.find('\n', pos)) != std::string::npos) {
            result.replace(pos, 1, "<br/>");
            pos += 5;
        }
        return result;
    }

    static std::string DifficultyClass(const std::string &star) {
        if (star == "中等") {
            return "medium";
        }
        if (star == "困难") {
            return "hard";
        }
        return "easy";
    }

public:
    void AllExpandHtml(const std::vector<Question> &questions, std::string *html) {
        if (html == nullptr) {
            return;
        }

        *html = "<html>";
        *html += "<head>";
        *html += "<meta charset='utf-8'>";
        *html += "<title>题目列表</title>";
        *html += "<style>";
        *html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
        *html += ".container { max-width: 1000px; margin: 0 auto; }";
        *html += ".question { background-color: white; border: 1px solid #ddd; padding: 20px; margin: 12px 0; border-radius: 8px; }";
        *html += ".question-top { display: flex; justify-content: space-between; align-items: center; }";
        *html += ".question-title { font-size: 18px; font-weight: bold; }";
        *html += ".question-title a { text-decoration: none; color: #1f6feb; }";
        *html += ".question-title a:hover { text-decoration: underline; }";
        *html += ".tag { padding: 4px 12px; border-radius: 12px; font-size: 12px; }";
        *html += ".easy { background: #e7f6ec; color: #2b8a3e; }";
        *html += ".medium { background: #fff3cd; color: #ad6800; }";
        *html += ".hard { background: #fde2e1; color: #c92a2a; }";
        *html += ".question-bottom { margin-top: 12px; color: #666; font-size: 14px; }";
        *html += "</style>";
        *html += "</head>";
        *html += "<body><div class='container'><h1>题目列表</h1>";

        for (size_t i = 0; i < questions.size(); ++i) {
            const Question &q = questions[i];
            *html += "<div class='question'>";
            *html += "<div class='question-top'>";
            *html += "<div class='question-title'>" + EscapeHtml(q.number) + ". <a href='/question/" + EscapeHtml(q.number) + "'>" + EscapeHtml(q.title) + "</a></div>";
            *html += "<div class='tag " + DifficultyClass(q.star) + "'>" + EscapeHtml(q.star) + "</div>";
            *html += "</div>";
            *html += "<div class='question-bottom'>CPU: " + std::to_string(q.cpu_limit) + "s | 内存: " + std::to_string(q.mem_limit) + "MB</div>";
            *html += "</div>";
        }

        *html += "</div></body></html>";
    }

    void OneExpandHtml(const Question &q, std::string *html) {
        if (html == nullptr) {
            return;
        }

        *html = "<html>";
        *html += "<head>";
        *html += "<meta charset='utf-8'>";
        *html += "<title>" + EscapeHtml(q.title) + "</title>";
        *html += "<style>";
        *html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
        *html += ".container { max-width: 1000px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; }";
        *html += ".top { display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid #eee; padding-bottom: 16px; }";
        *html += ".tag { padding: 4px 12px; border-radius: 12px; font-size: 12px; }";
        *html += ".easy { background: #e7f6ec; color: #2b8a3e; }";
        *html += ".medium { background: #fff3cd; color: #ad6800; }";
        *html += ".hard { background: #fde2e1; color: #c92a2a; }";
        *html += ".block { margin-top: 24px; }";
        *html += ".block h3 { margin-bottom: 12px; }";
        *html += ".desc, .code { background: #fafafa; border: 1px solid #eee; border-radius: 6px; padding: 16px; }";
        *html += ".code pre { margin: 0; white-space: pre-wrap; word-break: break-all; }";
        *html += ".actions { margin-top: 24px; }";
        *html += ".btn { display: inline-block; padding: 10px 18px; margin-right: 10px; text-decoration: none; border-radius: 6px; }";
        *html += ".btn-back { background: #e9ecef; color: #333; }";
        *html += ".btn-submit { background: #1f6feb; color: white; }";
        *html += "</style>";
        *html += "</head>";
        *html += "<body><div class='container'>";
        *html += "<div class='top'>";
        *html += "<h1>" + EscapeHtml(q.number) + ". " + EscapeHtml(q.title) + "</h1>";
        *html += "<div class='tag " + DifficultyClass(q.star) + "'>" + EscapeHtml(q.star) + "</div>";
        *html += "</div>";
        *html += "<div class='block'><strong>时间限制:</strong> " + std::to_string(q.cpu_limit) + "s &nbsp;&nbsp; <strong>内存限制:</strong> " + std::to_string(q.mem_limit) + "MB</div>";
        *html += "<div class='block'><h3>题目描述</h3><div class='desc'>" + HtmlText(q.desc) + "</div></div>";
        *html += "<div class='block'><h3>代码模板</h3><div class='code'><pre>" + EscapeHtml(q.header) + "</pre></div></div>";
        *html += "<div class='actions'>";
        *html += "<a class='btn btn-back' href='/'>返回题目列表</a>";
        *html += "<a class='btn btn-submit' href='/judge/" + EscapeHtml(q.number) + "'>去提交</a>";
        *html += "</div>";
        *html += "</div></body></html>";
    }

    void JudgePageHtml(const Question &q, std::string *html) {
        if (html == nullptr) {
            return;
        }

        *html = "<html>";
        *html += "<head>";
        *html += "<meta charset='utf-8'>";
        *html += "<title>提交代码</title>";
        *html += "<style>";
        *html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
        *html += ".container { max-width: 1000px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; }";
        *html += "textarea { width: 100%; min-height: 420px; padding: 12px; border: 1px solid #ddd; border-radius: 6px; font-family: Consolas, monospace; font-size: 14px; box-sizing: border-box; }";
        *html += ".submit-btn { margin-top: 16px; padding: 10px 20px; border: none; background: #1f6feb; color: white; border-radius: 6px; cursor: pointer; }";
        *html += ".links { margin-top: 18px; }";
        *html += ".links a { margin-right: 12px; color: #1f6feb; text-decoration: none; }";
        *html += "</style>";
        *html += "</head>";
        *html += "<body><div class='container'>";
        *html += "<h1>提交代码 - " + EscapeHtml(q.title) + "</h1>";
        *html += "<form action='/judge/" + EscapeHtml(q.number) + "' method='POST'>";
        *html += "<textarea name='code'>" + EscapeHtml(q.header) + "</textarea>";
        *html += "<br/><input class='submit-btn' type='submit' value='提交运行'>";
        *html += "</form>";
        *html += "<div class='links'><a href='/question/" + EscapeHtml(q.number) + "'>返回题目</a><a href='/'>返回列表</a></div>";
        *html += "</div></body></html>";
    }

    void JudgeResultHtml(const std::string &number, const ns_compile_run::JudgeResult &result, std::string *html) {
        if (html == nullptr) {
            return;
        }

        std::string title = result.status == ns_compile_run::JUDGE_OK ? "运行成功" : "运行失败";
        std::string result_class = result.status == ns_compile_run::JUDGE_OK ? "ok" : "fail";

        *html = "<html>";
        *html += "<head>";
        *html += "<meta charset='utf-8'>";
        *html += "<title>判题结果</title>";
        *html += "<style>";
        *html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
        *html += ".container { max-width: 1000px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; }";
        *html += ".ok { background: #e7f6ec; border: 1px solid #c3e6cb; }";
        *html += ".fail { background: #fde2e1; border: 1px solid #f5c2c7; }";
        *html += ".panel { margin-top: 18px; padding: 16px; border-radius: 6px; }";
        *html += ".block { margin-top: 18px; }";
        *html += ".block pre { margin: 0; background: #f8f9fa; border: 1px solid #eee; border-radius: 6px; padding: 12px; white-space: pre-wrap; word-break: break-all; }";
        *html += ".links { margin-top: 20px; }";
        *html += ".links a { margin-right: 12px; color: #1f6feb; text-decoration: none; }";
        *html += "</style>";
        *html += "</head>";
        *html += "<body><div class='container'>";
        *html += "<h1>" + title + "</h1>";
        *html += "<div class='panel " + result_class + "'><strong>状态码:</strong> " + std::to_string(result.status) + "<br/><strong>原因:</strong> " + EscapeHtml(result.reason) + "</div>";

        if (!result.output.empty()) {
            *html += "<div class='block'><h3>标准输出</h3><pre>" + EscapeHtml(result.output) + "</pre></div>";
        }

        if (!result.error.empty()) {
            *html += "<div class='block'><h3>错误信息</h3><pre>" + EscapeHtml(result.error) + "</pre></div>";
        }

        *html += "<div class='links'><a href='/judge/" + EscapeHtml(number) + "'>重新提交</a><a href='/question/" + EscapeHtml(number) + "'>返回题目</a></div>";
        *html += "</div></body></html>";
    }
};
}
