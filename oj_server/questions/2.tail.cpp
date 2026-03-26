#define COMPILER_ONLINE
#include "2.header.cpp"
#include <iostream>
#include <vector>

void Test1() {
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::vector<int> result = Solution().twoSum(nums, target);
    std::cout << "Test 1: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

void Test2() {
    std::vector<int> nums = {3, 2, 4};
    int target = 6;
    std::vector<int> result = Solution().twoSum(nums, target);
    std::cout << "Test 2: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

void Test3() {
    std::vector<int> nums = {3, 3};
    int target = 6;
    std::vector<int> result = Solution().twoSum(nums, target);
    std::cout << "Test 3: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

void Test4() {
    std::vector<int> nums = {-1, -2, -3, -4, -5};
    int target = -8;
    std::vector<int> result = Solution().twoSum(nums, target);
    std::cout << "Test 4: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

void Test5() {
    std::vector<int> nums = {0, 4, 3, 0};
    int target = 0;
    std::vector<int> result = Solution().twoSum(nums, target);
    std::cout << "Test 5: [" << result[0] << ", " << result[1] << "]" << std::endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}