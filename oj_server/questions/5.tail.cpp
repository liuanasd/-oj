#define COMPILER_ONLINE
#include "5.header.cpp"
#include <iostream>
#include <vector>
#include <string>

void Test1() {
    std::vector<std::string> strs = {"flower", "flow", "flight"};
    std::string result = Solution().longestCommonPrefix(strs);
    std::cout << "Test 1: longestCommonPrefix([\"flower\", \"flow\", \"flight\"]) = \"" << result << "\"" << std::endl;
}

void Test2() {
    std::vector<std::string> strs = {"dog", "racecar", "car"};
    std::string result = Solution().longestCommonPrefix(strs);
    std::cout << "Test 2: longestCommonPrefix([\"dog\", \"racecar\", \"car\"]) = \"" << result << "\"" << std::endl;
}

void Test3() {
    std::vector<std::string> strs = {"apple", "app", "application"};
    std::string result = Solution().longestCommonPrefix(strs);
    std::cout << "Test 3: longestCommonPrefix([\"apple\", \"app\", \"application\"]) = \"" << result << "\"" << std::endl;
}

void Test4() {
    std::vector<std::string> strs = {""};
    std::string result = Solution().longestCommonPrefix(strs);
    std::cout << "Test 4: longestCommonPrefix([\"\"]) = \"" << result << "\"" << std::endl;
}

void Test5() {
    std::vector<std::string> strs = {"a", "a", "a"};
    std::string result = Solution().longestCommonPrefix(strs);
    std::cout << "Test 5: longestCommonPrefix([\"a\", \"a\", \"a\"]) = \"" << result << "\"" << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}