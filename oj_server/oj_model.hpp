#pragma once

#include "../compiler.hpp"
#include "../modou.hpp"
#include <algorithm>

namespace ns_model {
using namespace ns_util;

struct Question {
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

class Model {
private:
    std::unordered_map<std::string, Question> questions;

public:
    Model() {
        LoadQuestionlist(question_list);
    }

    bool LoadQuestionlist(const std::string &list_path) {
        std::ifstream ifs(list_path.c_str());
        if (!ifs) {
            return false;
        }

        std::string line;
        while (getline(ifs, line)) {
            std::vector<std::string> tokens;
            StringUtil::SplitString(line, tokens, " ");
            if (tokens.size() != 5) {
                continue;
            }

            Question q;
            q.number = tokens[0];
            q.title = tokens[1];
            q.star = tokens[2];
            q.cpu_limit = std::atoi(tokens[3].c_str());
            q.mem_limit = std::atoi(tokens[4].c_str());

            q.desc = FileUtil::ReadFile(question_path + q.number + ".desc");
            q.header = FileUtil::ReadFile(question_path + q.number + ".header.cpp");
            q.tail = FileUtil::ReadFile(question_path + q.number + ".tail.cpp");

            questions[q.number] = q;
        }

        ifs.close();
        return true;
    }

    bool GetAllQuestions(std::vector<Question> &out) {
        out.clear();
        for (std::unordered_map<std::string, Question>::iterator it = questions.begin(); it != questions.end(); ++it) {
            out.push_back(it->second);
        }

        std::sort(out.begin(), out.end(), [](const Question &left, const Question &right) {
            return std::atoi(left.number.c_str()) < std::atoi(right.number.c_str());
        });

        return true;
    }

    bool GetOneQuestion(const std::string &number, Question &q) {
        std::unordered_map<std::string, Question>::iterator it = questions.find(number);
        if (it == questions.end()) {
            return false;
        }

        q = it->second;
        return true;
    }
};
}
