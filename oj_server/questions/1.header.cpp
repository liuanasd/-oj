#include <iostream>
class Solution {
public:
    bool isPalindrome(int x) {
        // 负数一定不是回文数
        if (x < 0) return false;
        
        int original = x;
        long long reversed = 0;
        
        while (x > 0) {
            int digit = x % 10;
            reversed = reversed * 10 + digit;
            x /= 10;
        }
        
        return original == reversed;
    }
};