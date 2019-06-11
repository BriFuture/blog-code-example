"""
判断一个整数是否是回文数。回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。

示例 1:

输入: 121
输出: true
示例 2:

输入: -121
输出: false
解释: 从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。
示例 3:

输入: 10
输出: false
解释: 从右向左读, 为 01 。因此它不是一个回文数。

进阶:

你能不将整数转为字符串来解决这个问题吗？
"""

def getNumIn(x, bit):
    num1 = 10 ** bit
    num2 = 10 ** (bit+1)
    tmp = x
    if x > num2:
        tmp -= num2
    tmp = x - num1
    return 0

class Solution:
    def isPalindrome(self, x: int) -> bool:
        if x < 0:
            return False
        xlist = []
        while x > 0:
            x, tmp = divmod(x, 10)
            xlist.append(tmp)
        # print(xlist)
        xlen = len(xlist)
        for i in range(xlen // 2 ):
            if xlist[i] != xlist[xlen - 1 - i]:
                return False
        return True

data = [
    {
        "input1": 121,
        "res": True
    },
    {
        "input1": -121,
        "res": False
    },
    {
        "input1": 10,
        "res": False
    },
    {
        "input1": 1,
        "res": True
    },
    {
        "input1": 11,
        "res": True
    }
]

s = Solution()
for i in range(0, len(data)):
# for i in range(len(data) - 1, len(data)):
    d = data[i]
    expect = s.isPalindrome(d["input1"])
    if expect != d["res"]:
        raise ValueError(f"{i}, expect: {d['res']} != res: {expect}")
    print(expect, f"[Judge]==== {i} ======", expect == d["res"])