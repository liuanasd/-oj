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
            
            if (bracketMap.find(c) != bracketMap.end()) {
                
                if (stack.empty() || stack.top() != bracketMap[c]) {
                    return false;
                }
                stack.pop();
            } else {
                
                stack.push(c);
            }
        }
        
        
        return stack.empty();
    }
};