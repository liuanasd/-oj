#include <iostream>
#include "../httplib.h"
#include "oj_control.hpp"

using namespace httplib;
using namespace ns_control;

int main() {
    Server svr;
    Control ctrl;

    svr.set_mount_point("/static", "../");

    svr.Get("/", [&ctrl](const Request &req, Response &res) {
        (void)req;
        std::string html;
        if (!ctrl.AllQuestions(&html)) {
            res.status = 500;
        }
        res.set_content(html, "text/html;charset=utf-8");
    });

    svr.Get("/question/([0-9]+)", [&ctrl](const Request &req, Response &res) {
        std::string number = req.matches[1];
        std::string html;
        if (!ctrl.GetQuestion(number, &html)) {
            res.status = 404;
        }
        res.set_content(html, "text/html;charset=utf-8");
    });

    svr.Get("/judge/([0-9]+)", [&ctrl](const Request &req, Response &res) {
        std::string number = req.matches[1];
        std::string html;
        if (!ctrl.GetJudgePage(number, &html)) {
            res.status = 404;
        }
        res.set_content(html, "text/html;charset=utf-8");
    });

    svr.Post("/judge/([0-9]+)", [&ctrl](const Request &req, Response &res) {
        std::string number = req.matches[1];
        std::string code;
        if (req.has_param("code")) {
            code = req.get_param_value("code");
        }

        ns_compile_run::JudgeResult result;
        std::string err_msg;
        if (!ctrl.Judge(number, code, &result, &err_msg)) {
            if (err_msg == "question not found") {
                res.status = 404;
            } else if (err_msg == "code is empty") {
                res.status = 400;
            } else {
                res.status = 500;
            }
        }

        std::string html;
        ctrl.GetJudgeResult(number, result, &html);
        res.set_content(html, "text/html;charset=utf-8");
    });

    std::cout << "oj server is running on 0.0.0.0:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}
