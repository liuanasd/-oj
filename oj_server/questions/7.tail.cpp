#define COMPILER_ONLINE
#include "7.header.cpp"
#include <iostream>

// 打印链表
void printList(ListNode* head) {
    while (head) {
        std::cout << head->val;
        if (head->next) {
            std::cout << "->";
        }
        head = head->next;
    }
    std::cout << std::endl;
}

void Test1() {
    // 创建链表1: 1->2->4
    ListNode* l1 = new ListNode(1);
    l1->next = new ListNode(2);
    l1->next->next = new ListNode(4);
    
    // 创建链表2: 1->3->4
    ListNode* l2 = new ListNode(1);
    l2->next = new ListNode(3);
    l2->next->next = new ListNode(4);
    
    ListNode* result = Solution().mergeTwoLists(l1, l2);
    std::cout << "Test 1: " << std::endl;
    std::cout << "Input: 1->2->4, 1->3->4" << std::endl;
    std::cout << "Output: ";
    printList(result);
}

void Test2() {
    // 空链表测试
    ListNode* l1 = nullptr;
    ListNode* l2 = nullptr;
    
    ListNode* result = Solution().mergeTwoLists(l1, l2);
    std::cout << "Test 2: " << std::endl;
    std::cout << "Input: [], []" << std::endl;
    std::cout << "Output: ";
    printList(result);
}

void Test3() {
    // 一个空链表测试
    ListNode* l1 = nullptr;
    ListNode* l2 = new ListNode(0);
    
    ListNode* result = Solution().mergeTwoLists(l1, l2);
    std::cout << "Test 3: " << std::endl;
    std::cout << "Input: [], [0]" << std::endl;
    std::cout << "Output: ";
    printList(result);
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}