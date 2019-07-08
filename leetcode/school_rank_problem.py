"""
有A、B、C、D、E5所学校，在一次检查评比中，已知E校肯定不是第2名或第3.他们相互进行推测。A校有人说，E校一定是第1名。
B校有人说，我校可能是第2名。
C校有人说，A校最差。
D校有人说，C校不是最好的。
E校有人说，D校会获得第1名。
结果只有第1名和第2名学校的人猜对了，编程指出这5所学校的名次
"""
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
schools = ('A', 'B', 'C', 'D', 'E')

def union(A, B) :
    """ A + B
    """
    return list(set(A).intersection(B))

def diff(A, B):
    """ A - B
    """
    return list(set(A).difference(set(B)))

RankFromRightPeople = [1, 2]
RankFromWrongPeople = [3, 4, 5]
predictSchool = [0] * 5


def predictRank(si, sj):
    global predictSchool
    predictSchool = [0] * 5
    for index, p in enumerate(pred):
        if si == index or sj == index:
            predictSchool[schools.index(p["Pos"][0])] = list(p["Pos"][1])
        else:
            predictSchool[schools.index(p["Neg"][0])] = list(p["Neg"][1])

    # print("Init", predictSchool)
    # 交集 union
    for i in range(len(schools)):
        if i == si or i == sj:
            predictSchool[i] = union(predictSchool[i], RankFromRightPeople)
        else:
            predictSchool[i] = union(predictSchool[i], RankFromWrongPeople)

    # print("Before", predictSchool)
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
            return []
                            
    return predictSchool

for i in range(5):
    for j in range(i+1, 5):
        pr = predictRank(i, j)
        if len(pr) > 0:
            result = {}
            for k, p in enumerate(pr):
                pr[k] = [schools[k], p[0]]
            print(dict(pr))
        # print(pr, i, j, '\n---------')

# pr = predictRank(1, 3)
# print(pr)   