#include "compiler.hpp"
#include "compile_run.hpp"
#include "httplib.h"
using namespace httplib;
using namespace ns_util;
using namespace json_util;

// 显示使用方法
void Usage(int argc, char *argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return;
    }
}

int main(int argc, char *argv[]){
    // 检查参数
    Usage(argc, argv);
    int port = atoi(argv[1]);
    
    // 创建HTTP服务器
    Server svr;
    // 处理POST请求
    svr.Post("/", [](const Request& req, Response& res){
        std::string in_json = req.body;
        std::string out_json;
        
        if(in_json.size() == 0){
            out_json = json_util::generate(1, "提交代码错误");
        } else {
            // 处理编译运行请求
            ns_compile_run::CompileRun::Stat(in_json, out_json);
        }
        
        res.set_content(out_json, "application/json");
    });
    
    // 启动服务器
    std::cout << "Server starting on 0.0.0.0:" << port << std::endl;
    svr.listen("0.0.0.0", port);
    
    return 0;
}
