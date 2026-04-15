#pragma once

#include "compiler.hpp"
#include "modou.hpp"
#include "runner.hpp"
#include <fstream>
#include <sstream>

namespace json_util {
void parse(const std::string &json, std::string &code, std::string &input, int &cpu, int &mem);
std::string escape(const std::string &text);
std::string generate(int status, const std::string &reason, const std::string &output = "", const std::string &error = "");
}

namespace ns_compile_run {
enum {
    JUDGE_OK = 0,
    JUDGE_PARAM_ERROR = 1,
    JUDGE_SYSTEM_ERROR = 2,
    JUDGE_COMPILE_ERROR = 3,
    JUDGE_RUN_ERROR = 4
};

struct JudgeRequest {
    std::string code;
    std::string input;
    int cpu_limit;
    int mem_limit;

    JudgeRequest() : cpu_limit(1), mem_limit(128) {}
};

struct JudgeResult {
    int status;
    std::string reason;
    std::string output;
    std::string error;
    std::string file_name;

    JudgeResult() : status(JUDGE_OK) {}
};

class CompileRun {
public:
    static bool Judge(const JudgeRequest &request, JudgeResult *result);
    static void Stat(const std::string &in_json, std::string &out_json);
};
}
