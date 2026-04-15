#pragma once

#include "modou.hpp"
#include "log.hpp"

namespace ns_util {
extern const std::string temp_path;

class File_name {
public:
    static std::string add(const std::string &file_name, const std::string &suffix);
    static std::string Src(const std::string &file_name);
    static std::string Exe(const std::string &file_name);
    static std::string Stderr(const std::string &file_name);
    static std::string Stdin(const std::string &file_name);
    static std::string Stdout(const std::string &file_name);
    static std::string CompilerError(const std::string &file_name);
};

class Fileutil {
public:
    static bool iffileexe(const std::string &path_name);
    static bool EnsureDir(const std::string &path_name);
    static std::string UniqFileName();
    static bool WriteFile(const std::string &file_name, const std::string &code);
    static bool Readfile(const std::string &target, std::string &code, bool keep = false);
};

class timeutil {
public:
    static std::string gettime();
};

class compilefile {
public:
    static bool CompileFile(const std::string &file_name);
};

class File {
public:
    bool FFile(const std::string &file_name);
};
}
