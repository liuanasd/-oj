#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
class Solution {
public:
    bool isValid(std::string s) {
        std::stack<char> stack;
        std::unordered_map<char, char> bracketMap = {
            {')', '('},
            {'}', '{'},
            {']', '['}
        };
        
        for (char c : s) {
            // 如果是右括号
            if (bracketMap.find(c) != bracketMap.end()) {
                // 栈为空或栈顶元素不匹配
                if (stack.empty() || stack.top() != bracketMap[c]) {
                    return false;
                }
                stack.pop();
            } else {
                // 左括号入栈
                stack.push(c);
            }
        }
        
        // 栈为空则所有括号都匹配
        return stack.empty();
    }
};