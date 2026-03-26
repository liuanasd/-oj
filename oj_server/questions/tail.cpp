#define COMPILER_ONLINE
#include "header.cpp"
#include "mudou.hpp"
void Test(){
    bool ret=Solution().isPalindrome(121);
    if(ret){
        std::cout<<"121 is palindrome"<<std::endl;
    }
    else{
        std::cout<<"121 is not palindrome"<<std::endl;
    }
} 
void Test2(){
    bool ret=Solution().isPalindrome(-121);
    if(ret){
        std::cout<<"-121 is palindrome"<<std::endl;
    }
    else{
        std::cout<<"-121 is not palindrome"<<std::endl;
    }
}
int main(){
    Test();
    Test2();
    return 0;
}