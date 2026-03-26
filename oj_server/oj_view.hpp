#pragma once
#include "../modou.hpp"
#include "../log.hpp"
namespace ns_view{
    using namespace ns_model;
    class View{
        public:
            View() {}
            ~View() {}
            
            // 生成题目列表HTML
            void AllExpandHtml(std::vector<Question> questions, std::string *html){
                if (!html) return;
                
                *html = "<html>";
                *html += "<head>";
                *html += "<title>题目列表</title>";
                *html += "<style>";
                *html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
                *html += ".container { max-width: 1000px; margin: 0 auto; }";
                *html += "h1 { color: #1a1a1a; margin-bottom: 30px; }";
                *html += ".question { background-color: white; border: 1px solid #e1e1e1; padding: 20px; margin: 10px 0; border-radius: 8px; transition: all 0.3s ease; }";
                *html += ".question:hover { box-shadow: 0 2px 8px rgba(0,0,0,0.1); transform: translateY(-1px); }";
                *html += ".question-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; }";
                *html += ".question-info { display: flex; align-items: center; }";
                *html += ".number { font-weight: bold; color: #007bff; margin-right: 15px; font-size: 16px; }";
                *html += ".title { font-size: 18px; font-weight: 500; }";
                *html += ".difficulty { display: inline-block; padding: 2px 12px; border-radius: 10px; font-size: 12px; font-weight: 500; }";
                *html += ".difficulty.easy { background-color: #d4edda; color: #155724; }";
                *html += ".difficulty.medium { background-color: #fff3cd; color: #856404; }";
                *html += ".difficulty.hard { background-color: #f8d7da; color: #721c24; }";
                *html += ".question-footer { display: flex; align-items: center; justify-content: space-between; margin-top: 10px; }";
                *html += ".limits { font-size: 14px; color: #666; }";
                *html += "a { text-decoration: none; color: #007bff; }";
                *html += "a:hover { text-decoration: underline; }";
                *html += "</style>";
                *html += "</head>";
                *html += "<body>";
                *html += "<div class='container'>";
                *html += "<h1>题目列表</h1>";
                
                for (const auto& q : questions) {
                    *html += "<div class='question'>";
                    *html += "<div class='question-header'>";
                    *html += "<div class='question-info'>";
                    *html += "<div class='number'>" + q.number + ".</div>";
                    *html += "<div class='title'><a href='/question/" + q.number + "'>" + q.title + "</a></div>";
                    *html += "</div>";
                    std::string difficulty_class = "easy";
                    if (q.star == "中等") difficulty_class = "medium";
                    else if (q.star == "困难") difficulty_class = "hard";
                    *html += "<div class='difficulty " + difficulty_class + "'>" + q.star + "</div>";
                    *html += "</div>";
                    *html += "<div class='question-footer'>";
                    *html += "<div class='limits'>CPU: " + std::to_string(q.cpu_limit) + "s, 内存: " + std::to_string(q.mem_limit) + "MB</div>";
                    *html += "</div>";
                    *html += "</div>";
                }
                
                *html += "</div>";
                *html += "</body>";
                *html += "</html>";
            }
            
            // 生成单个题目HTML
            void OneExpandHtml(const struct Question q, std::string *html){
                if (!html) return;
                
                *html = "<html>";
                *html += "<head>";
                *html += "<title>" + q.title + "</title>";
                *html += "<style>";
                *html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
                *html += ".container { max-width: 1000px; margin: 0 auto; background-color: white; border-radius: 8px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); padding: 30px; }";
                *html += "h1 { color: #1a1a1a; margin-bottom: 20px; font-size: 24px; }";
                *html += ".question-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 30px; padding-bottom: 15px; border-bottom: 1px solid #e1e1e1; }";
                *html += ".question-info { display: flex; align-items: center; }";
                *html += ".number { font-weight: bold; color: #007bff; margin-right: 15px; font-size: 16px; }";
                *html += ".difficulty { display: inline-block; padding: 4px 16px; border-radius: 12px; font-size: 14px; font-weight: 500; }";
                *html += ".difficulty.easy { background-color: #d4edda; color: #155724; }";
                *html += ".difficulty.medium { background-color: #fff3cd; color: #856404; }";
                *html += ".difficulty.hard { background-color: #f8d7da; color: #721c24; }";
                *html += ".info { margin: 20px 0; padding: 15px; background-color: #f8f9fa; border-radius: 6px; font-size: 14px; }";
                *html += ".info p { margin: 5px 0; }";
                *html += ".desc { margin: 20px 0; padding: 20px; border: 1px solid #e1e1e1; border-radius: 6px; line-height: 1.6; }";
                *html += ".desc h3 { margin-top: 0; color: #333; }";
                *html += ".desc pre { background-color: #f5f5f5; padding: 10px; border-radius: 4px; overflow-x: auto; }";
                *html += ".code { margin: 20px 0; padding: 20px; background-color: #f8f9fa; border: 1px solid #e1e1e1; border-radius: 6px; }";
                *html += ".code h3 { margin-top: 0; color: #333; }";
                *html += ".code pre { background-color: #2d2d2d; color: #f8f8f2; padding: 15px; border-radius: 4px; overflow-x: auto; font-family: 'Consolas', 'Monaco', 'Courier New', monospace; font-size: 14px; }";
                *html += ".actions { margin-top: 30px; display: flex; justify-content: space-between; }";
                *html += ".btn { display: inline-block; padding: 10px 20px; border-radius: 4px; text-decoration: none; font-weight: 500; transition: all 0.3s ease; }";
                *html += ".btn-primary { background-color: #007bff; color: white; }";
                *html += ".btn-primary:hover { background-color: #0069d9; }";
                *html += ".btn-secondary { background-color: #6c757d; color: white; }";
                *html += ".btn-secondary:hover { background-color: #5a6268; }";
                *html += "</style>";
                *html += "</head>";
                *html += "<body>";
                *html += "<div class='container'>";
                *html += "<div class='question-header'>";
                *html += "<div class='question-info'>";
                *html += "<div class='number'>" + q.number + "</div>";
                *html += "<h1>" + q.title + "</h1>";
                *html += "</div>";
                std::string difficulty_class = "easy";
                if (q.star == "中等") difficulty_class = "medium";
                else if (q.star == "困难") difficulty_class = "hard";
                *html += "<div class='difficulty " + difficulty_class + "'>" + q.star + "</div>";
                *html += "</div>";
                
                *html += "<div class='info'>";
                *html += "<p><strong>CPU限制:</strong> " + std::to_string(q.cpu_limit) + "秒</p>";
                *html += "<p><strong>内存限制:</strong> " + std::to_string(q.mem_limit) + "MB</p>";
                *html += "</div>";
                
                *html += "<div class='desc'>";
                *html += q.desc;
                *html += "</div>";
                
                *html += "<div class='code'>";
                *html += "<h3>代码模板</h3>";
                *html += "<pre>" + q.header + "</pre>";
                *html += "</div>";
                
                *html += "<div class='actions'>";
                *html += "<a href='/' class='btn btn-secondary'>返回题目列表</a>";
                *html += "<a href='/judge/" + q.number + "' class='btn btn-primary'>提交解答</a>";
                *html += "</div>";
                
                *html += "</div>";
                *html += "</body>";
                *html += "</html>";
            }
    };
}