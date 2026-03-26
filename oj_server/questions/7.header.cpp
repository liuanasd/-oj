#include <iostream>
// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // 创建哑节点作为新链表的头节点
        ListNode dummy(0);
        ListNode* current = &dummy;
        
        // 遍历两个链表，比较节点值
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                current->next = l1;
                l1 = l1->next;
            } else {
                current->next = l2;
                l2 = l2->next;
            }
            current = current->next;
        }
        
        // 处理剩余节点
        if (l1) {
            current->next = l1;
        } else {
            current->next = l2;
        }
        
        return dummy.next;
    }
};