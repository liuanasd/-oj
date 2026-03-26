#define COMPILER_ONLINE
#include "4.header.cpp"
#include <iostream>
#include <string>

void Test1() {
    int result = Solution().romanToInt("III");
    std::cout << "Test 1: romanToInt(\"III\") = " << result << std::endl;
}

void Test2() {
    int result = Solution().romanToInt("IV");
    std::cout << "Test 2: romanToInt(\"IV\") = " << result << std::endl;
}

void Test3() {
    int result = Solution().romanToInt("IX");
    std::cout << "Test 3: romanToInt(\"IX\") = " << result << std::endl;
}

void Test4() {
    int result = Solution().romanToInt("LVIII");
    std::cout << "Test 4: romanToInt(\"LVIII\") = " << result << std::endl;
}

void Test5() {
    int result = Solution().romanToInt("MCMXCIV");
    std::cout << "Test 5: romanToInt(\"MCMXCIV\") = " << result << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}