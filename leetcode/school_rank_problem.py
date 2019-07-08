"""
题目：
有A、B、C、D、E5所学校，在一次检查评比中，已知E校肯定不是第2名或第3.他们相互进行推测。A校有人说，E校一定是第1名。
B校有人说，我校可能是第2名。
C校有人说，A校最差。
D校有人说，C校不是最好的。
E校有人说，D校会获得第1名。
结果只有第1名和第2名学校的人猜对了，编程指出这5所学校的名次
"""

"""
Pos 表示该学校的人猜对的结果，Neg 表示猜错的结果。
从前往后依次是 A B C D E 学校的人的预测结果。

将问题转化到集合中，求出可能存在的确定的（无矛盾的）集合. By BriFuture

扩展性：修改 school 元组、pred 数组，RankFromRightPeople、RankFromWrongPeople 这几个参数，应该可以得出相应的结果（未测试）
"""

schools = ('A', 'B', 'C', 'D', 'E')

pred = [
    {
        "Pos": ('E', (1, )),
        "Neg": ('E', (4, 5))
    },
    {
        "Pos": ('B', (1, 2, 3, 4, 5)),
        "Neg": ('B', (1, 2, 3, 4, 5))
    },

    {
        "Pos": ('A', (5, )),
        "Neg": ('A', (1, 2, 3, 4))
    },

    {
        "Pos": ('C', (2, 3, 4, 5)),
        "Neg": ('C', (1, ))
    },

    {
        "Pos": ('D', (1, )),
        "Neg": ('D', (2, 3, 4, 5))
    }
]

# 猜对的人的学校排名是 1 或 2
RankFromRightPeople = [1, 2]
# 猜错的人的学校排名是 3 4 5
RankFromWrongPeople = [3, 4, 5]


def union(A, B) :
    """ A + B
    """
    return list(set(A).intersection(B))

def diff(A, B):
    """ A - B
    """
    return list(set(A).difference(set(B)))


def predictRank(si, sj):
    predictSchool = [0] * len(schools)

    # 收集结果集
    for index, p in enumerate(pred):
        if si == index or sj == index:
            predictSchool[schools.index(p["Pos"][0])] = list(p["Pos"][1])
        else:
            predictSchool[schools.index(p["Neg"][0])] = list(p["Neg"][1])

    # print("Init", predictSchool)
    # 取交集 union
    for i in range(len(schools)):
        if i == si or i == sj:
            predictSchool[i] = union(predictSchool[i], RankFromRightPeople)
        else:
            predictSchool[i] = union(predictSchool[i], RankFromWrongPeople)

    # print("Before", predictSchool)
    # 确定排名
    for _ in range(len(schools) * 2):
        # 只会执行一定次数循环
        s = set()
        # print("Before", predictSchool)
        for pr in predictSchool:
            lpr = len(pr)
            if lpr == 0:
                return []
            elif lpr == 1:
                if pr[0] in s:
                    # print("Already definite.")
                    return []
                s.add(pr[0])

        # print("Set 2 list", s)
        for i, pr in enumerate(predictSchool):
            lpr = len(pr)
            if lpr > 1:
                predictSchool[i] = diff(pr, s)

    for pr in predictSchool:
        if len(pr) != 1:
            #   有学校排名无法确定
            return []
                            
    return predictSchool

def startPredict():
    for i in range(5):
        for j in range(i+1, 5):
            pr = predictRank(i, j)
            if len(pr) > 0:
                for k, p in enumerate(pr):
                    pr[k] = [schools[k], p[0]]

                dpr = dict(pr)
                npr = sorted(dpr.items(), key = lambda item: item[1])
                print(npr)
                # 这里其实可以停止循环了
                # return
        # print(pr, i, j, '\n---------')

startPredict()
