#define COMPILER_ONLINE
#include "3.header.cpp"
#include <iostream>
#include <climits>

void Test1() {
    int result = Solution().reverse(123);
    std::cout << "Test 1: reverse(123) = " << result << std::endl;
}

void Test2() {
    int result = Solution().reverse(-123);
    std::cout << "Test 2: reverse(-123) = " << result << std::endl;
}

void Test3() {
    int result = Solution().reverse(120);
    std::cout << "Test 3: reverse(120) = " << result << std::endl;
}

void Test4() {
    int result = Solution().reverse(0);
    std::cout << "Test 4: reverse(0) = " << result << std::endl;
}

void Test5() {
    int result = Solution().reverse(1534236469); // 测试溢出
    std::cout << "Test 5: reverse(1534236469) = " << result << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}