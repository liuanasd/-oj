#define COMPILER_ONLINE
#include "9.header.cpp"
#include <iostream>
#include <vector>


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
    std::vector<int> nums = {3, 2, 2, 3};
    int val = 3;
    int length = Solution().removeElement(nums, val);
    std::cout << "Test 1: " << std::endl;
    std::cout << "Input: [3, 2, 2, 3], val = 3" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test2() {
    std::vector<int> nums = {0, 1, 2, 2, 3, 0, 4, 2};
    int val = 2;
    int length = Solution().removeElement(nums, val);
    std::cout << "Test 2: " << std::endl;
    std::cout << "Input: [0, 1, 2, 2, 3, 0, 4, 2], val = 2" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test3() {
    std::vector<int> nums = {};
    int val = 0;
    int length = Solution().removeElement(nums, val);
    std::cout << "Test 3: " << std::endl;
    std::cout << "Input: [], val = 0" << std::endl;
    std::cout << "Output length: " << length << std::endl;
    std::cout << "Output array: ";
    printVector(nums, length);
}

void Test4() {
    std::vector<int> nums = {2};
    int val = 3;
    int length = Solution().removeElement(nums, val);
    std::cout << "Test 4: " << std::endl;
    std::cout << "Input: [2], val = 3" << std::endl;
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