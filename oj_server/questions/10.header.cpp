#include <iostream>
#include <string>
class Solution {
public:
    int strStr(std::string haystack, std::string needle) {
        int n = haystack.length();
        int m = needle.length();
        
        // 处理空needle的情况
        if (m == 0) return 0;
        
        // 遍历haystack，寻找needle的起始位置
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            // 比较当前位置开始的子串与needle
            while (j < m && haystack[i + j] == needle[j]) {
                j++;
            }
            // 如果完全匹配，返回起始位置
            if (j == m) {
                return i;
            }
        }
        
        // 没有找到匹配
        return -1;
    }
};