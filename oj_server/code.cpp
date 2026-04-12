
#define COMPILER_ONLINE
#include "./questions/1.header.cpp"
#include <iostream>

void Test1() {
    bool ret = Solution().isPalindrome(121);
    std::cout << "Test 1: 121 is " << (ret ? "palindrome" : "not palindrome") << std::endl;
}

void Test2() {
    bool ret = Solution().isPalindrome(-121);
    std::cout << "Test 2: -121 is " << (ret ? "palindrome" : "not palindrome") << std::endl;
}

void Test3() {
    bool ret = Solution().isPalindrome(10);
    std::cout << "Test 3: 10 is " << (ret ? "palindrome" : "not palindrome") << std::endl;
}

void Test4() {
    bool ret = Solution().isPalindrome(0);
    std::cout << "Test 4: 0 is " << (ret ? "palindrome" : "not palindrome") << std::endl;
}

void Test5() {
    bool ret = Solution().isPalindrome(12321);
    std::cout << "Test 5: 12321 is " << (ret ? "palindrome" : "not palindrome") << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}