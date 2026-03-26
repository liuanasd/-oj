#define COMPILER_ONLINE
#include "6.header.cpp"
#include <iostream>
#include <string>

void Test1() {
    bool result = Solution().isValid("()");
    std::cout << "Test 1: isValid(\"()\") = " << (result ? "true" : "false") << std::endl;
}

void Test2() {
    bool result = Solution().isValid("()[]{}");
    std::cout << "Test 2: isValid(\"()[]{}\") = " << (result ? "true" : "false") << std::endl;
}

void Test3() {
    bool result = Solution().isValid("(]");
    std::cout << "Test 3: isValid(\"(]\") = " << (result ? "true" : "false") << std::endl;
}

void Test4() {
    bool result = Solution().isValid("([)]");
    std::cout << "Test 4: isValid(\"([)]\") = " << (result ? "true" : "false") << std::endl;
}

void Test5() {
    bool result = Solution().isValid("{[]}");
    std::cout << "Test 5: isValid(\"{[]}\") = " << (result ? "true" : "false") << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}