"""
给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。

如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。

您可以假设除了数字 0 之外，这两个数都不会以 0 开头。

示例：

输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 0 -> 8
原因：342 + 465 = 807
"""

# Definition for singly-linked list.
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

def printListNode(ln):
    """辅助函数，打印链表
    """
    sum = 0
    level = 0
    while ln:
        i = ln.val
        sum += i * pow(10, level)
        ln = ln.next
        level += 1
    print(sum)

def buildListNode(arr):
    """辅助函数，从数组中构建链表
    """
    nodestart = ListNode(arr[0])
    node = nodestart
    for i in range(1, len(arr)):
        node.next = ListNode(arr[i])
        node = node.next
    return nodestart
    

class Solution:
    def addTwoNumbers(self, l1: ListNode, l2: ListNode) -> ListNode:
        nodestart = ListNode(0)
        result = nodestart

        l1start = ListNode(0)
        l1start.next = l1
        l2start = ListNode(0)
        l2start.next = l2
        
        nextLevelup = False  #  carry flag
        while True:          
            if l1start.next is None and l2start.next is None:
                if nextLevelup:
                    result.next = ListNode(1)
                    # result = result.next
                break
            l1start = l1start.next if l1start.next is not None else ListNode(0)
            l2start = l2start.next if l2start.next is not None else ListNode(0)

            num1 = l1start.val if l1start is not None else 0
            num2 = l2start.val if l2start is not None else 0
            if nextLevelup:
                num1 += 1
                nextLevelup = False
            
            num1 += num2
            if num1 > 9:
                nextLevelup = True
                num1 -= 10
            result.next = ListNode(num1)
            result = result.next

        return nodestart.next

s = Solution()
d1 = [2, 4, 3]
d2 = [5, 6, 4]
d1 = [1,8]
d2 = [0]
d1 = [5]
d2 = [5]

datal1 = buildListNode(d1)
datal2 = buildListNode(d2)
# printListNode(datal2)

res = s.addTwoNumbers(datal1, datal2)

printListNode(res)
