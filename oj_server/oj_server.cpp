#include <iostream>
#include "../httplib.h"
#include "oj_control.hpp"
#include "../compile_run.hpp"
using namespace ns_control;
using namespace httplib;
int main(){
    Server svr;
    Control ctrl;
    
    // 题目列表路由
    svr.Get("/", [&ctrl](const Request& req, Response& res){
        std::string html;
        ctrl.AllQuestions(&html);
        res.set_content(html, "text/html;charset=utf-8");
    });
    
    // 设置静态文件服务
    svr.set_mount_point("/static", "../");
    
    // 单个题目路由
    svr.Get("/question/([0-9]+)", [&ctrl](const Request& req, Response& res){
       std::string number=req.matches[1];
       std::string html;
       ctrl.GetQuestion(number, &html);
        res.set_content(html, "text/html;charset=utf-8");
    });
    
    // 判题路由 - GET请求用于显示提交页面
    svr.Get("/judge/([0-9]+)", [&ctrl](const Request& req, Response& res){
        std::string number=req.matches[1];
        ns_model::Question q;
        if(ctrl.model_.GetOneQuestion(number, q)){
            std::string html;
            html = "<html>";
            html += "<head>";
            html += "<title>提交解答 - " + q.title + "</title>";
            html += "<style>";
            html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
            html += ".container { max-width: 1000px; margin: 0 auto; background-color: white; border-radius: 8px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); padding: 30px; }";
            html += "h1 { color: #1a1a1a; margin-bottom: 30px; font-size: 24px; }";
            html += ".form-group { margin-bottom: 20px; }";
            html += ".form-group label { display: block; margin-bottom: 8px; font-weight: 500; }";
            html += ".form-group textarea { width: 100%; height: 400px; padding: 15px; border: 1px solid #e1e1e1; border-radius: 4px; font-family: 'Consolas', 'Monaco', 'Courier New', monospace; font-size: 14px; resize: vertical; }";
            html += ".form-group input[type='submit'] { background-color: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; font-size: 16px; font-weight: 500; cursor: pointer; transition: all 0.3s ease; }";
            html += ".form-group input[type='submit']:hover { background-color: #0069d9; }";
            html += ".back { margin-top: 20px; }";
            html += "a { text-decoration: none; color: #007bff; }";
            html += "a:hover { text-decoration: underline; }";
            html += ".result { margin-top: 30px; padding: 20px; background-color: #f8f9fa; border: 1px solid #e1e1e1; border-radius: 6px; }";
            html += ".result h3 { margin-top: 0; color: #333; }";
            html += ".result pre { background-color: #f5f5f5; padding: 10px; border-radius: 4px; overflow-x: auto; font-family: 'Consolas', 'Monaco', 'Courier New', monospace; font-size: 14px; }";
            html += "</style>";
            html += "</head>";
            html += "<body>";
            html += "<div class='container'>";
            html += "<h1>提交解答 - " + q.title + "</h1>";
            html += "<form action='/judge/" + number + "' method='POST'>";
            html += "<div class='form-group'>";
            html += "<label for='code'>代码：</label>";
            html += "<textarea id='code' name='code' placeholder='请输入C++代码...'>" + q.header + "</textarea>";
            html += "</div>";
            html += "<div class='form-group'>";
            html += "<input type='submit' value='提交解答'>";
            html += "</div>";
            html += "</form>";
            html += "<div class='back'>";
            html += "<a href='/question/" + number + "'>返回题目详情</a> | <a href='/'>返回题目列表</a>";
            html += "</div>";
            html += "</div>";
            html += "</body>";
            html += "</html>";
            res.set_content(html, "text/html;charset=utf-8");
        } else {
            res.set_content("题目不存在", "text/plain;charset=utf-8");
        }
    });
    
    // 判题路由 - POST请求用于处理代码提交
    svr.Post("/judge/([0-9]+)", [](const Request& req, Response& res){
        std::string number=req.matches[1];
        std::string code = req.get_param_value("code");
        
        if(code.empty()){
            res.set_content("代码不能为空", "text/plain;charset=utf-8");
            return;
        }
        
        // 构建完整的代码，包含题目对应的tail.cpp
        std::string full_code = "#define COMPILER_ONLINE\n";
        full_code += code + "\n";
        
        // 读取对应的tail.cpp文件
        std::string tail_path = "./questions/" + number + ".tail.cpp";
        std::ifstream tail_file(tail_path);
        if(tail_file){
            std::string tail_content((std::istreambuf_iterator<char>(tail_file)), std::istreambuf_iterator<char>());
            full_code += tail_content;
            tail_file.close();
        } else {
            res.set_content("无法读取测试文件", "text/plain;charset=utf-8");
            return;
        }
        
        // 使用CompileRun::Stat函数处理代码编译和运行
        std::string in_json = "{\"code\":\"" + full_code + "\",\"input\":\"\",\"cpu\":1,\"mem\":128}";
        std::string out_json;
        ns_compile_run::CompileRun::Stat(in_json, out_json);
        
        // 解析输出JSON
        int status = 0;
        std::string reason, output, error;
        
        // 简单解析JSON
        size_t status_start = out_json.find("\"status\":");
        if(status_start != std::string::npos){
            status_start += 9;
            size_t status_end = out_json.find(",", status_start);
            if(status_end == std::string::npos) status_end = out_json.find("}", status_start);
            if(status_end != std::string::npos){
                std::string status_str = out_json.substr(status_start, status_end - status_start);
                status = std::stoi(status_str);
            }
        }
        
        size_t reason_start = out_json.find("\"reason\":\"");
        if(reason_start != std::string::npos){
            reason_start += 10;
            size_t reason_end = out_json.find("\"", reason_start);
            if(reason_end != std::string::npos){
                reason = out_json.substr(reason_start, reason_end - reason_start);
            }
        }
        
        size_t output_start = out_json.find("\"output\":\"");
        if(output_start != std::string::npos){
            output_start += 10;
            size_t output_end = out_json.find("\"", output_start);
            if(output_end != std::string::npos){
                output = out_json.substr(output_start, output_end - output_start);
            }
        }
        
        size_t error_start = out_json.find("\"error\":\"");
        if(error_start != std::string::npos){
            error_start += 9;
            size_t error_end = out_json.find("\"", error_start);
            if(error_end != std::string::npos){
                error = out_json.substr(error_start, error_end - error_start);
            }
        }
        
        // 生成结果页面
        std::string html;
        html = "<html>";
        html += "<head>";
        html += "<title>运行结果</title>";
        html += "<style>";
        html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }";
        html += ".container { max-width: 1000px; margin: 0 auto; background-color: white; border-radius: 8px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); padding: 30px; }";
        
        if(status == 0){
            html += "h1 { color: #28a745; margin-bottom: 30px; font-size: 24px; }";
            html += ".result { margin-top: 20px; padding: 20px; background-color: #d4edda; border: 1px solid #c3e6cb; border-radius: 6px; }";
            html += ".result h3 { margin-top: 0; color: #155724; }";
        } else {
            html += "h1 { color: #dc3545; margin-bottom: 30px; font-size: 24px; }";
            html += ".result { margin-top: 20px; padding: 20px; background-color: #f8d7da; border: 1px solid #f5c6cb; border-radius: 6px; }";
            html += ".result h3 { margin-top: 0; color: #721c24; }";
        }
        
        html += ".result pre { background-color: #f5f5f5; padding: 10px; border-radius: 4px; overflow-x: auto; font-family: 'Consolas', 'Monaco', 'Courier New', monospace; font-size: 14px; }";
        html += ".back { margin-top: 20px; }";
        html += "a { text-decoration: none; color: #007bff; }";
        html += "a:hover { text-decoration: underline; }";
        html += "</style>";
        html += "</head>";
        html += "<body>";
        html += "<div class='container'>";
        
        if(status == 0){
            html += "<h1>运行成功</h1>";
        } else {
            html += "<h1>运行失败</h1>";
        }
        
        html += "<div class='result'>";
        html += "<h3>原因：</h3>";
        html += "<pre>" + reason + "</pre>";
        
        if(!output.empty()){
            html += "<h3>输出：</h3>";
            html += "<pre>" + output + "</pre>";
        }
        
        if(!error.empty()){
            html += "<h3>错误：</h3>";
            html += "<pre>" + error + "</pre>";
        }
        
        html += "</div>";
        html += "<div class='back'>";
        html += "<a href='/judge/" + number + "'>重新提交</a> | <a href='/question/" + number + "'>返回题目详情</a>";
        html += "</div>";
        html += "</div>";
        html += "</body>";
        html += "</html>";
        
        res.set_content(html, "text/html;charset=utf-8");
    });
    
    // 启动服务器
    std::cout << "Server running on http://0.0.0.0:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}