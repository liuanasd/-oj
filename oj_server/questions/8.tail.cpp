#define COMPILER_ONLINE
#include "8.header.cpp"
#include <iostream>
#include <vector>

// 打印数组
void printVector(std::vector<int>& nums, int length) {
    std::cout << "[";
    for (int i = 0; i < length; i++) {
        std::cout << nums[i];
        if (i < length - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void Test1() {
    std::vector<int> nums = {1, 1, 2};
    int length = Solution().removeDuplicates(nums);
    std::cout << "Test 1: " << std::endl;
    std::cout << "Input: [1, 1, 2]" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test2() {
    std::vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int length = Solution().removeDuplicates(nums);
    std::cout << "Test 2: " << std::endl;
    std::cout << "Input: [0, 0, 1, 1, 1, 2, 2, 3, 3, 4]" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test3() {
    std::vector<int> nums = {};
    int length = Solution().removeDuplicates(nums);
    std::cout << "Test 3: " << std::endl;
    std::cout << "Input: []" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test4() {
    std::vector<int> nums = {1};
    int length = Solution().removeDuplicates(nums);
    std::cout << "Test 4: " << std::endl;
    std::cout << "Input: [1]" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    return 0;
}