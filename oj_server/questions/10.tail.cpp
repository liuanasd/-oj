#define COMPILER_ONLINE
#include "10.header.cpp"
#include <iostream>
#include <string>

void Test1() {
    std::string haystack = "hello";
    std::string needle = "ll";
    int result = Solution().strStr(haystack, needle);
    std::cout << "Test 1: " << std::endl;
    std::cout << "Input: haystack = \"hello\", needle = \"ll\"" << std::endl;
    std::cout << "Output: " << result << std::endl;
}

void Test2() {
    std::string haystack = "aaaaa";
    std::string needle = "bba";
    int result = Solution().strStr(haystack, needle);
    std::cout << "Test 2: " << std::endl;
    std::cout << "Input: haystack = \"aaaaa\", needle = \"bba\"" << std::endl;
    std::cout << "Output: " << result << std::endl;
}

void Test3() {
    std::string haystack = "";
    std::string needle = "";
    int result = Solution().strStr(haystack, needle);
    std::cout << "Test 3: " << std::endl;
    std::cout << "Input: haystack = \"\", needle = \"\"" << std::endl;
    std::cout << "Output: " << result << std::endl;
}

void Test4() {
    std::string haystack = "a";
    std::string needle = "a";
    int result = Solution().strStr(haystack, needle);
    std::cout << "Test 4: " << std::endl;
    std::cout << "Input: haystack = \"a\", needle = \"a\"" << std::endl;
    std::cout << "Output: " << result << std::endl;
}

void Test5() {
    std::string haystack = "mississippi";
    std::string needle = "issip";
    int result = Solution().strStr(haystack, needle);
    std::cout << "Test 5: " << std::endl;
    std::cout << "Input: haystack = \"mississippi\", needle = \"issip\"" << std::endl;
    std::cout << "Output: " << result << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}