#include "compile_run.hpp"
#include <cctype>

namespace {
bool FindStringValue(const std::string &json, const std::string &key, std::string *value) {
    if (value == nullptr) {
        return false;
    }

    std::string pattern = "\"" + key + "\":\"";
    size_t start = json.find(pattern);
    if (start == std::string::npos) {
        return false;
    }

    start += pattern.size();
    std::string raw;
    bool escape = false;
    for (size_t i = start; i < json.size(); ++i) {
        char ch = json[i];
        if (escape) {
            raw += ch;
            escape = false;
            continue;
        }

        if (ch == '\\') {
            raw += ch;
            escape = true;
            continue;
        }

        if (ch == '"') {
            *value = raw;
            return true;
        }

        raw += ch;
    }

    return false;
}

bool FindIntValue(const std::string &json, const std::string &key, int *value) {
    if (value == nullptr) {
        return false;
    }

    std::string pattern = "\"" + key + "\":";
    size_t start = json.find(pattern);
    if (start == std::string::npos) {
        return false;
    }

    start += pattern.size();
    while (start < json.size() && json[start] == ' ') {
        ++start;
    }

    size_t end = start;
    while (end < json.size() && ((json[end] >= '0' && json[end] <= '9') || json[end] == '-')) {
        ++end;
    }

    if (end == start) {
        return false;
    }

    *value = std::atoi(json.substr(start, end - start).c_str());
    return true;
}
}

namespace json_util {
std::string Unescape(const std::string &text) {
    std::string result;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '\\' || i + 1 >= text.size()) {
            result += text[i];
            continue;
        }

        char next = text[i + 1];
        switch (next) {
        case 'n':
            result += '\n';
            break;
        case 'r':
            result += '\r';
            break;
        case 't':
            result += '\t';
            break;
        case 'b':
            result += '\b';
            break;
        case 'f':
            result += '\f';
            break;
        case '"':
            result += '"';
            break;
        case '\\':
            result += '\\';
            break;
        default:
            result += next;
            break;
        }
        ++i;
    }
    return result;
}

void parse(const std::string &json, std::string &code, std::string &input, int &cpu, int &mem) {
    code.clear();
    input.clear();
    cpu = 1;
    mem = 128;

    std::string raw_code;
    if (FindStringValue(json, "code", &raw_code)) {
        code = Unescape(raw_code);
    }

    std::string raw_input;
    if (FindStringValue(json, "input", &raw_input)) {
        input = Unescape(raw_input);
    }

    FindIntValue(json, "cpu", &cpu);
    FindIntValue(json, "mem", &mem);
}

std::string escape(const std::string &text) {
    std::string result;
    for (size_t i = 0; i < text.size(); ++i) {
        switch (text[i]) {
        case '"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        case '\b':
            result += "\\b";
            break;
        case '\f':
            result += "\\f";
            break;
        default:
            result += text[i];
            break;
        }
    }
    return result;
}

std::string generate(int status, const std::string &reason, const std::string &output, const std::string &error) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"status\":" << status << ",";
    oss << "\"reason\":\"" << escape(reason) << "\",";
    oss << "\"output\":\"" << escape(output) << "\",";
    oss << "\"error\":\"" << escape(error) << "\"";
    oss << "}";
    return oss.str();
}
}

namespace ns_compile_run {
bool CompileRun::Judge(const JudgeRequest &request, JudgeResult *result) {
    if (result == nullptr) {
        return false;
    }

    result->status = JUDGE_OK;
    result->reason.clear();
    result->output.clear();
    result->error.clear();
    result->file_name.clear();

    if (request.code.empty()) {
        result->status = JUDGE_PARAM_ERROR;
        result->reason = "empty code";
        return false;
    }

    std::string file_name = ns_util::Fileutil::UniqFileName();
    if (file_name.empty()) {
        result->status = JUDGE_SYSTEM_ERROR;
        result->reason = "create temp file failed";
        return false;
    }
    result->file_name = file_name;

    if (!ns_util::Fileutil::WriteFile(ns_util::File_name::Src(file_name), request.code)) {
        result->status = JUDGE_SYSTEM_ERROR;
        result->reason = "write source file failed";
        return false;
    }

    if (!ns_util::Fileutil::WriteFile(ns_util::File_name::Stdin(file_name), request.input)) {
        result->status = JUDGE_SYSTEM_ERROR;
        result->reason = "write input file failed";
        return false;
    }

    if (!ns_util::compilefile::CompileFile(file_name)) {
        ns_util::Fileutil::Readfile(ns_util::File_name::CompilerError(file_name), result->error, false);
        result->status = JUDGE_COMPILE_ERROR;
        result->reason = "compile error";
        return true;
    }

    int run_ret = ns_runner::Runner::run(file_name, request.cpu_limit, request.mem_limit);
    if (run_ret < 0) {
        result->status = JUDGE_SYSTEM_ERROR;
        result->reason = "run system error";
        return false;
    }

    ns_util::Fileutil::Readfile(ns_util::File_name::Stdout(file_name), result->output, false);
    ns_util::Fileutil::Readfile(ns_util::File_name::Stderr(file_name), result->error, false);

    if (run_ret == 0) {
        result->status = JUDGE_OK;
        result->reason = "run ok";
        return true;
    }

    result->status = JUDGE_RUN_ERROR;
    std::ostringstream oss;
    oss << "run error, exit code: " << run_ret;
    result->reason = oss.str();
    return true;
}

void CompileRun::Stat(const std::string &in_json, std::string &out_json) {
    JudgeRequest request;
    json_util::parse(in_json, request.code, request.input, request.cpu_limit, request.mem_limit);

    JudgeResult result;
    CompileRun::Judge(request, &result);
    out_json = json_util::generate(result.status, result.reason, result.output, result.error);
}
}
