class Tester(object):
    def __init__(self):
        pass

    def test(self, func, args, exe_times = 100):
        import time
        print("[D] ", func, args)
        start = time.perf_counter()
        for _ in range(exe_times):
            func(**args)
        end = time.perf_counter()
        print("Exe func ", func)
        print("Total Time: ", end - start, "s", " Per time: ", (end - start) / exe_times)

class Main(object):
    def readfile(self, name):
        lines = []
        with open(name, "r", encoding="utf-8") as f:
            for line in f.readlines():
                cline = [int(l) for l in line.split(",") ]
                lines.append(cline)
            # for line in f:
            #     cline = [int(l) for l in line.split(",") ]
            #     lines.append(cline)
        # print(len(lines), lines[0:3])

if __name__ == "__main__":
    m = Main()
    # m.readfile("./test_data.txt")
    t = Tester()
    args = { "name": "./test_data.txt"}
    t.test(m.readfile, args = args, exe_times=3)
    

        
