from typing import List
from datetime import datetime
swap = [0, 0]

itemsize = 2
def _quickSort(indices, start, end):
    # start, end are both even
    global swap, itemsize
    base = indices[start + 1]
    i = start + itemsize
    j = end
    while i != j:
        while indices[j+1] >= base and j > i:
            j -= itemsize
        while indices[i+1] <= base and i < j:
            i += itemsize

        swap = indices[j:j+itemsize]
        indices[j:j+itemsize] = indices[i:i+itemsize]
        indices[i:i+itemsize] = swap
        # j -= itemsize
        # i += itemsize
        # else:
    # the real value of i is always less than base
    if base > indices[i+1]:
        swap = indices[start:start + itemsize]
        indices[start:start + itemsize] = indices[i:i+itemsize]
        indices[i:i+itemsize] = swap
        return i
    else:
        return start

def quickSort(indices, start, end):
    if start >= end:
        return
    i = _quickSort(indices, start, end)
    quickSort(indices, start, i-1)
    quickSort(indices, i+1, end)

def quickSortIterative(indices, start, end):
    global itemsize
    stack = [0] * (end - start + 1)
    top = -1
    top += 1
    stack[top] = start
    top += 1
    stack[top] = end

    while top >= 0:
        end = stack[top]
        top -= 1
        start = stack[top]
        top -= 1

        mid = _quickSort(indices, start, end)
        if mid - itemsize > start:
            top += 1
            stack[top] = start
            top += 1
            stack[top] = mid - itemsize

        if mid + itemsize < end:
            top += 1
            stack[top] = mid + itemsize
            top += 1
            stack[top] = end

class Solution(object):
    def checkQuickSort(self):
        m = 0
        minimum = self.indices[1]
        j = 0
        for value in self.indices[1::2]:
            if value < minimum:
                raise ValueError(f"Quicksort Failed [{j}] {value} >= [{m}: {self.indices[m]}] {minimum}")
            else:
                minimum = value
                m = j
            j += itemsize
        
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        indices = [0] * len(nums) * 2
        for i, n in zip(range(len(nums)), nums):
            indices[i*2] = i
            indices[i*2+1] = n

        l_indices = len(indices)
        self.indices = indices
        quickSortIterative(indices, 0, l_indices - itemsize)
        self.checkQuickSort()
        # bubbleSort(indices)
        # print(indices)
        # sort done

        minValue = indices[1]
        for i in range(l_indices//2):
            indices[i*2+1] = indices[i*2+1] - minValue
        # target -= minValue
        adder = 2
        self.target = target - adder * minValue
        self.targetIndices = []
        adder1Idx, adder2Idx = self._twoSum(self.target)

        if indices[adder1Idx] < indices[adder2Idx]:
            self.targetIndices.append(indices[adder1Idx])
            self.targetIndices.append(indices[adder2Idx])
        else:
            self.targetIndices.append(indices[adder2Idx])
            self.targetIndices.append(indices[adder1Idx])
        # self.certainAdder(adder, target)
        return self.targetIndices

    def _twoSum(self, target):
        arrs = self.indices
        indices = len(arrs)
        adder2Idx = indices - itemsize # idx must be even
        adder1Start = 0
        # print("_twoSum:", arrs, target)
        while True:
            ssum = arrs[adder1Start+1] + arrs[adder2Idx+1]
            # minor range 找到符合条件的较大的数的最大取值
            if ssum == target:
                return adder1Start, adder2Idx
            elif ssum < target:
                break
            adder2Idx -= itemsize
        adder1End = adder2Idx - itemsize
        # esum = arrs[adder1End+1] + arrs[adder2Idx+1]
        adder1Idx = (adder1Start // 4 + adder1End //4) << 1

        while adder2Idx > adder1Idx:
            isum = arrs[adder1Idx+1] + arrs[adder2Idx+1]
            if isum < target:  # move adder1 Start
                if adder1Idx > adder1Start:
                    adder1Start = adder1Idx
                    adder1Idx = (adder1Start // 4 + adder1End //4) << 1
                else:
                    adder1Start += itemsize
                    adder1Idx = adder1End
            elif isum > target: # move adder1 end
                if adder1Idx < adder1End:
                    adder1End = adder1Idx
                    adder1Idx = (adder1Start // 4 + adder1End //4) << 1
                else:
                    adder1End -= itemsize
                    adder1Idx = adder1Start
            else:
                return adder1Idx, adder2Idx

            if adder1Start > adder1End:
                adder2Idx -= itemsize
                adder1Start = 0
                adder1End = adder2Idx - itemsize
                adder1Idx = (adder1Start // 4 + adder1End //4) << 1
 
        return adder1Idx, adder2Idx

s = Solution()
from data1 import data
# data["nums"] = [230,863,916,585,981,404,316,785,88,12,70,435,384,778,887,755,740,337,86,92,325,422,815,650,920,125,277,336,221,847,168,23,677,61,400,136,874,363,394,199,863,997,794,587,124,321,212,957,764,173,314,422,927,783,930,282,306,506,44,926,691,568,68,730,933,737,531,180,414,751,28,546,60,371,493,370,527,387,43,541,13,457,328,227,652,365,430,803,59,858,538,427,583,368,375,173,809,896,370,789]
# data["target"] = 542
# data["nums"] = [8, 7, 12, 4, 2]
# data["target"] = 11

# data["nums"] = [6, 1, 2, 7, 9, 3, 4, 5, 10, 8, 5, 6, 6, 5, 10, 23]
# data["target"] = 12
# data["nums"] = data["nums"][90:100]
print(s.twoSum(data["nums"], data["target"]))
