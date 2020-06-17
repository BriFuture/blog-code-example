#include <iostream>
#include <fstream>
#include <ostream>
#include <pthread.h>
// ============== structure definitions ======
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <iterator>

 //#undef BUILD_WITH_CMAKE

#define CACHE_LINE_SIZE 64
// 10w
#define CIRCLE_INITIAL_SIZE 20000 
#define CIRCLE_INCREMENT_SIZE 50000
#define USE_NODE_LAST_3

#ifdef BUILD_WITH_CMAKE
#include "definitions.h"
#else
#define MaxThreadNum 4
// WorkerNum 不能加快速度，但是可以均分任务
#define DefaultWorkerNum 12

#define DataInputFile "/data/test_data.txt"
#define DataOutputFile "/projects/student/result.txt"
#endif
#define MaxRecverCount 15
#if defined(MY_DEBUG) or defined(MY_INFO)
static const int VecLinesPreserveSize = 100000;
#else
static const int VecLinesPreserveSize = 300000;
#endif
static int WorkerNum = DefaultWorkerNum;
using namespace std;
//#define USE_THIRD_COL
/**
转账记录，从原始 txt 文件转换为 STransfer 结构体
*/
struct STransfer {
	// directive edge
	int t1; // sender
	int t2; // directive recver
#ifdef USE_THIRD_COL
	int money;  // unused now
#endif
};

bool sortTransferBySender(const STransfer &s1, const STransfer &s2) {
	if(s1.t1 != s2.t1) 
		return s1.t1 < s2.t1; 
	return s1.t2 < s2.t2;
}

bool sortTransferByRecver(const STransfer &s1, const STransfer &s2) {
	if(s1.t2 != s2.t2)
		return s1.t2 < s2.t2;
	return s1.t1 < s2.t1;
}


vector<STransfer> vecLines; // 按照 sender 排序
vector<STransfer> vecLines2; // 按照 recver 排序
set<int> Pub_graphNoId;
// ---------------- read from file ------------------
/** read from spec file with three columns,
	* first  column : sender
	* second column : recver
	* third  column : money
	* store data in structure STransfer, all STransfers are stored in vecLines with asc-order
	*/
#ifdef ON_WIN
#include <Windows.h>
#include <winnt.h>
#else 
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#endif
#define PAGE_SIZE 4096
void readfile()
{
	size_t firstPos = 0;
	size_t secondPos = 0;
	size_t iter = 0;
	vecLines.resize(VecLinesPreserveSize); // reserve size
	size_t fileSize;
	char *mappedFileAddr;
#ifdef ON_WIN
	HANDLE fileDesc = CreateFileA(DataInputFile, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(DataInputFile, GetFileExInfoStandard, &fad)) 
		return; // error condition, could cal
	fileSize = static_cast<size_t>(fad.nFileSizeLow);
	
	HANDLE fileMapping = CreateFileMapping(fileDesc, NULL, PAGE_READONLY, 0, 0, NULL);
	mappedFileAddr = (char *) MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
#else
	struct stat sb;
	int fd = open(DataInputFile, O_RDONLY);
	if(fd == -1) {
		cout << "[Read] OpenFile falied" << endl;
		close(fd);
		return;
	}
	fstat(fd, &sb);
	fileSize = static_cast<size_t>(sb.st_size);
	mappedFileAddr = (char *) mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
	if(mappedFileAddr == MAP_FAILED ) {
		cout << "MMap file failed" << endl;
		munmap(mappedFileAddr, 0);
		return;
	}
#endif // end of ON_WIN
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	cout << "MMap file Size: " << fileSize << endl;
#endif
	size_t readedBytes = 0;
	size_t newLineIdx = 0;
	size_t bufferStart = 0;
	size_t bufferEnd = 0;
	char buffer[40];
	
	while (true)
	{
		readedBytes = newLineIdx;
		while (newLineIdx < fileSize && mappedFileAddr[newLineIdx] != '\n') {
			newLineIdx += 1;
		}
		if(newLineIdx < readedBytes + 1) {
			break;
		}
#if defined(MY_DEBUG) || defined(MY_INFO)
		if (mappedFileAddr[readedBytes] == '#') continue;
#endif
		STransfer &trans = vecLines[iter];
		bufferStart = readedBytes;
		bufferEnd = readedBytes;
		while(mappedFileAddr[bufferEnd]!=',') {
			bufferEnd ++;
		}
		memcpy(buffer, mappedFileAddr+bufferStart, bufferEnd - bufferStart);
		buffer[bufferEnd - bufferStart] = '\0';
		trans.t1 = atoi(buffer);
		bufferEnd ++;
		bufferStart = bufferEnd;
		while(mappedFileAddr[bufferEnd]!=',') {
			bufferEnd ++;
		}
		memcpy(buffer, mappedFileAddr+bufferStart, bufferEnd - bufferStart);
		buffer[bufferEnd - bufferStart] = '\0';

		trans.t2 = atoi(buffer);
		Pub_graphNoId.insert(trans.t1);
		Pub_graphNoId.insert(trans.t2);
#ifdef USE_THIRD_COL
		// trans.money = stoi(line.substr(secondPos + 1));
#endif
		newLineIdx += 1;
		iter += 1;
	}

	vecLines.resize(iter);
	vecLines.shrink_to_fit();

	//std::copy(graphNodeId.begin(), graphNodeId.end(), Pub_graphNoId.begin());
	//std::sort(Pub_graphNoId.begin(), Pub_graphNoId.end());

	vecLines2.resize(iter);
	std::copy(vecLines.begin(), vecLines.end(), vecLines2.begin());
#ifdef ON_WIN
	UnmapViewOfFile(mappedFileAddr);
	CloseHandle(fileMapping);
	CloseHandle(fileDesc);
#else
	munmap(mappedFileAddr, fileSize);
	close(fd);
#endif
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	std::cout << "[CC] File Lines " << vecLines.size() 
		<< " Node size " << Pub_graphNoId.size() << " File: " << DataInputFile  << endl;
#endif
}

/* 包含 */
#define MaxCircleSize 7
#define MinCircleSize 3
#define CircleSizeCount (MaxCircleSize - MinCircleSize + 1)

//============================ CircleInterface ==================
class CircleInterface {
public:
	virtual int size() = 0;
	virtual void output(ostream &out) = 0;
	virtual size_t print(char *mmapAddr) = 0;
	virtual bool isAdding() const = 0;
	virtual void addCircle(int *start) = 0;
	virtual void sort() = 0;
};

#ifdef _MSC_VER
#pragma pack(push, 32)
#endif
/** 管理和排序已经检测出的环 环的大小会预先定义
*/
template<size_t _Size>
class _Circle : public CircleInterface {
public:

	_Circle(int initSize = CIRCLE_INITIAL_SIZE, int incSize = CIRCLE_INCREMENT_SIZE) {
		_maxSize = initSize;
		_incSize = incSize;
	}
	~_Circle() {
		circle.clear();
		//circle.shrink_to_fit();
	}
	bool isAdding() const { return false; }
	/* 为了适应 0-L ,1-3-5-P,2-4-6-N 的数据结构，修改数据存储方式 */
	void addCircle(int *start) {
		if (!_inited) {
			circle.resize(_maxSize);
			_inited = true;
		}
		array<int, _Size> &tmpCircle = circle[_circleCount];
		int it = 0;
		tmpCircle[0] = start[it]; // L 节点
		auto iter = 1;
		for (it = 1; it < _Size; it += 2) {
			tmpCircle[iter] = start[it]; // P 节点
			iter += 1;
		}
		iter = _Size - 1;
		for (it = 2; it < _Size; it += 2) {
			tmpCircle[iter] = start[it]; // N 节点
			iter -= 1;
		}
		_circleCount += 1;
		if (_circleCount >= _maxSize) {
			increaseSize();
		}
	}


	/* 将最后得到的结果打印或者输出到文件中 */
	inline void output(ostream &out) {
		int i, j;
		for (i = 0; i < _circleCount; i += 1) {
			//_print(circle[i], out);
			for (j = 0; j < _Size - 1; j += 1) {
				out << circle[i][j] << ",";
			}
			out << circle[i][j] << endl;
		}
	}
	size_t print(char *mmapAddr) {
		size_t i = 0;
		size_t j = 0;
		int n = 0;
		for (i = 0; i < _circleCount; i++) {
			for (j = 0; j < _Size - 1; j++) {
				n += sprintf(mmapAddr + n, "%d,", circle[i][j]);
			}
			n += sprintf(mmapAddr + n, "%d\n", circle[i][j]);
		}
		return n;
	}
	static bool compare(array<int, _Size> &a1, array<int, _Size> &a2) {
		for (int i = 1; i < _Size; i++) {
			if (a1[i] != a2[i])
				return a1[i] < a2[i];
		}
		return false;
	}
	void sort() {
		if (_circleCount == 0) {
			return;
		}
		auto i = circle.begin();
		/*auto upper = std::upper_bound(i, circle.end(), last);
		if (upper == circle.end()) {
		}*/
		auto j = i;
		auto start = i;
		i++;
		while (i != circle.end()) {
			if ((*j)[0] != (*i)[0]) {
				j = i;
			}
			if (j > start) {
				std::sort(start, j, compare);
				start = j;
			}
			i++;
		}
		//circle.resize(_circleCount);
		//circle.shrink_to_fit();
	}
	int size() { return _circleCount; }
protected:
	virtual void increaseSize() {
		_maxSize += _incSize;
		_incSize -= CIRCLE_INCREMENT_SIZE;
		if (_incSize < CIRCLE_INCREMENT_SIZE)
			_incSize = CIRCLE_INCREMENT_SIZE;
#if defined(FULL_DATA_INFO)
		cout << " Increase size [" << _Size << "]: " << _maxSize
			<< "  " << _incSize << endl;
#endif
		circle.resize(_maxSize);
	}
public:
	vector<array<int, _Size>> circle;
	/** 检测出的环数，不包含重复 */
	int _circleCount = 0;
	int _maxSize;
	// todo 尽可能给大环增加size
	int _incSize = 0;
	bool _inited = false;
}
#ifndef _MSC_VER
__attribute__((aligned(16)))
#endif
; // end of class definition
#ifdef _MSC_VER
#pragma pack(pop)
#endif

class Cycle7 : public _Circle<7> {
public:
	Cycle7(int initSize, int incSize): _Circle(initSize, incSize) {

	}
	inline void start() {
		//array<int, 7> &tmpCircle = circle[_circleCount];
		//tmpCircle[0] = L; // L 节点
		//tmpCircle[1] = P1; // P1
		//tmpCircle[2] = N1; // P2
	}
	inline void addCircle(int *start) {
		if (!_inited) {
			circle.resize(_maxSize);
			_inited = true;
		}
		
		array<int, 7> &tmpCircle = circle[_circleCount];
		int it = 0;
		tmpCircle[0] = start[0]; // L 节点
		tmpCircle[1] = start[1]; // P1
		tmpCircle[2] = start[2]; // P2
		tmpCircle[3] = start[3]; // P3
		tmpCircle[4] = start[6]; // N3
		tmpCircle[5] = start[5]; // N3
		tmpCircle[6] = start[4]; // N3
		
		_circleCount += 1;
		if (_circleCount >= _maxSize) {
			increaseSize();
		}
	}
};
// ============ end of CircleContainer ================

enum NodeGroup {
	GroupNone = 0x00,
	Group1 = 0x01,
	Group2 = 0x02,
	Group3 = 0x04,
	Group4 = 0x08,
	Group5 = 0x10,
	Group6 = 0x20,
	Group7 = 0x40,
	Group8 = 0x80,
	Group9 = 0x100,
	Group10 = 0x200,
	Group11 = 0x400,
	Group12 = 0x800,
	Group13 = 0x1000,
	Group14 = 0x2000,
	Group15 = 0x4000,
	Group16 = 0x8000,
	Group17 = 0x10000,
	Group18 = 0x20000,
	Group19 = 0x40000,
	Group20 = 0x80000,
	Group21 = 0x100000,
	Group22 = 0x200000,
	Group23 = 0x400000,
	Group24 = 0x800000,
	Group25 = 0x1000000,
	Group26 = 0x2000000,
	Group27 = 0x4000000,
	Group28 = 0x8000000,
	Group29 = 0x10000000,
	Group30 = 0x20000000,
	Group31 = 0x40000000,
	Group32 = 0x80000000,
	GroupAll = 0xffffffff,
};

#ifdef _MSC_VER
#pragma pack(push, 16)
#endif
/*
* 表示邻接关系，SGraphNode 是双向的邻接向量
*/
struct SGraphNode {
	vector<SGraphNode *> v_last_1;
	// distance 1 如果 next 1 为 0，则没有回路，如果 v_next_1 的序号比当前小，则检测环路时可以跳过
	vector<SGraphNode *> v_next_1;
	int id; // id 表示文件中的转账人
	int idx; // 表示在容器中的序号，调试用
	int group;

	inline int degree() {
		return v_next_1.size() + v_last_1.size();
	}

	inline bool last1MaxLess() {
		if (v_last_1.size() == 0) { return true; }
		return (*v_last_1.rbegin())->id < id;
	}
	inline bool next1MaxLess() {
		if (v_next_1.size() == 0) { return true; }
		return (*v_next_1.rbegin())->id < id;
	}

	SGraphNode()
	{
		group = GroupNone;
		v_last_1.reserve(MaxRecverCount * 2);
		v_next_1.reserve(MaxRecverCount * 2);
	}
	~SGraphNode() {
		v_last_1.clear();
		v_next_1.clear();
	}
} 
#ifndef _MSC_VER
__attribute__((aligned(16)))
#endif
	;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

/* 用来分割数据的节点， offset 是在 Pub_ 数组中的偏移
end 全部都是相对于数组头部的偏移, offset 包含数据而end 不含
*/
struct WorkNode {
#ifdef MY_DEBUG
	size_t debugid;
#endif
	size_t offset;
	size_t end;
};

struct PathN3 {
	int paths1; // skip is 2 now
	int paths2; // skip is 2 now
	int toId;
	PathN3() {
		//toId = INT32_MAX;
	}
	PathN3(int to, int p1, int p2) {
		toId = to;
		paths1 = p1;
		paths2 = p2;
	}
}
#ifndef _MSC_VER
__attribute__((aligned(8)))
#endif
;
struct SPathCompare {

	bool operator()(const PathN3 &p1, const PathN3 &p2) const {
		return p1.toId < p2.toId;
	}
	bool operator()(const PathN3 &p1, int to) {
		return p1.toId < to;
	}
	bool operator()(int to, const PathN3 &p1) {
		return to < p1.toId;
	}
} pathCompare;
// ============== end of structure definitions ======

#define MaxNodeCount 200000
size_t Pub_graphSize = 0;

unordered_map<int, int> idxMap; // id -> idx
vector<WorkNode> Pub_workNodes;

struct PubVariables {
	vector<SGraphNode> nodes;
	// 一级的索引和 nodes 对应，二级表示到特定点的位置
	vector<vector<PathN3>> snodeUS3;
	vector<vector<PathN3>> lastUS2;
	vector<vector<PathN3>> nextUS2;
	vector<SGraphNode *> rootNodes;

	inline void resize() {
		nextUS2.resize(Pub_graphSize);
		lastUS2.resize(Pub_graphSize);
		snodeUS3.resize(Pub_graphSize);

		rootNodes.reserve(200);
	}
	/* 将 STransfer 结构体转变成邻接矢量 */
	void prepareGraphNode() {
		// construct GraphNodes

		int lastId = -1;
		int iter = 0;
		nodes.reserve(vecLines.size() << 2); // max 2 times of lines
		for (auto it = Pub_graphNoId.begin(); it != Pub_graphNoId.end(); it++) {
			if (lastId != *it) {
				lastId = *it;
				nodes.emplace_back();
				SGraphNode &gn = nodes[iter];
				gn.idx = iter;
				gn.id = lastId;
				idxMap[lastId] = iter;
				iter += 1;
			}
		}
		Pub_graphSize = iter;
		nodes.resize(Pub_graphSize);
	} // end of prepareGraphNode


} pv;
/* 准备 SGraphNode 图节点的下一级 next_1 */
void *prepareGraphNode_n1(void *tid) {
	int lastId = -1;
	int lastIdx;
	// pre set connectivities. 按照 sender 排序
	sort(vecLines.begin(), vecLines.end(), sortTransferBySender);
	SGraphNode *gn;
	for (STransfer & line : vecLines) {
		if (lastId != line.t1) {
			lastId = line.t1;
			lastIdx = idxMap[lastId];
			gn = &pv.nodes[lastIdx];
		}
		// idxMap 找到 idx，然后找到地址
		gn->v_next_1.push_back(&pv.nodes[idxMap[line.t2]]);
	}
	vecLines.clear();
	vecLines.shrink_to_fit();
	return NULL;
}
/* 准备 SGraphNode 图节点的下一级 last_1 */
void *prepareGraphNode_l1(void *tid) {
	// 按照 recver 排序
	sort(vecLines2.begin(), vecLines2.end(), sortTransferByRecver);
	int lastId = -1;
	int lastIdx;
	SGraphNode *gn;
	for (STransfer & line : vecLines2) {
		if (lastId != line.t2) {
			lastId = line.t2;
			lastIdx = idxMap[lastId];
			gn = &pv.nodes[lastIdx];
		}
		gn->v_last_1.push_back(&pv.nodes[idxMap[line.t1]]);
	}
	vecLines2.clear();
	vecLines2.shrink_to_fit();
	return NULL;
}


/* 准备工作数据节点
@param uniform true 均分数据 需要已知 graphNode 的个数
*/
void prepareWork() {
	Pub_workNodes.resize(WorkerNum);

	// 前面节点的数据多，后面的数据少
	size_t end = 0;
	size_t uniPartialSize;
	uniPartialSize = (Pub_graphSize / WorkerNum) + 2; // Pub_graphSize > ThreadNum

	unsigned char i;
	size_t start = 0;
	for (i = 0; i < WorkerNum; i++) {
		Pub_workNodes[i].offset = start;
		start = uniPartialSize + end;
		end = start;
		Pub_workNodes[i].end = start;
		// last nodes modify
		if (Pub_workNodes[i].end > Pub_graphSize) {
			Pub_workNodes[i].end = Pub_graphSize;
		}
	}
	i -= 1;
	if (Pub_workNodes[i].offset < Pub_graphSize && Pub_workNodes[i].end < Pub_graphSize) {
		Pub_workNodes[i].end = Pub_graphSize;
	}
#if defined(MY_INFO) || defined(FULL_DATA_DEBUG)
	for (i = 0; i < WorkerNum; i++) {
		cout << "Works " << i << " ==: " << Pub_workNodes[i].offset << " -> " << Pub_workNodes[i].end << endl;
	}
#endif
	// ------------ end work nodes ------------------
}

/* prepare 所有的Node全部准备完毕后就能检测环  */
void prepareLayer2(size_t off) {
	SGraphNode *nodeL = &pv.nodes[off];
	if (nodeL->v_last_1.size() == 0 || nodeL->v_next_1.size() == 0)
		return;
	size_t i = 0;
	size_t j = 0;
	int sizeP1;
	int nodeN2Id;
	vector<PathN3> &sNodeL2 = pv.lastUS2[off];
	SGraphNode *nodeN1; 
	bool reservedN3 = false;
	for (i = 0; i < nodeL->v_last_1.size(); i += 1) {
		// push P1
		nodeN1 = nodeL->v_last_1[i];
		sizeP1 = nodeN1->v_last_1.size();

		if (reservedN3 == false) {
			reservedN3 = true;
			sNodeL2.reserve(nodeL->v_last_1.size() * MaxRecverCount);
		}
		j = 0;
		while (j < sizeP1) {
			// push P2 (N1)
			nodeN2Id = nodeN1->v_last_1[j]->id;
			j++;
			if (nodeN2Id == nodeL->id)
			{
				continue;
			}
			sNodeL2.emplace_back(nodeN2Id, nodeN1->id, 0);
		} // end while N1(P2)
	} // end while P1
	std::sort(sNodeL2.begin(), sNodeL2.end(), pathCompare);

	reservedN3 = false;
	vector<PathN3> &sNodeN2 = pv.nextUS2[off];
	for (i = 0; i < nodeL->v_next_1.size(); i += 1) {
		// push P1
		nodeN1 = nodeL->v_next_1[i];
		sizeP1 = nodeN1->v_next_1.size();

		if (reservedN3 == false) {
			reservedN3 = true;
			sNodeN2.reserve(nodeL->v_next_1.size() * MaxRecverCount);
		}
		j = 0;
		while (j < sizeP1) {
			// push P2 (N1)
			nodeN2Id = nodeN1->v_next_1[j]->id;
			j++;
			if (nodeN2Id == nodeL->id)
			{
				continue;
			}
			sNodeN2.emplace_back(nodeN2Id, nodeN1->id, 0);
		} // end while N1(P2)
	} // end while P1
	std::sort(sNodeN2.begin(), sNodeN2.end(), pathCompare);
}

void prepareLayer3(size_t off) {
	SGraphNode *nodeL = &pv.nodes[off];
	if (nodeL->v_last_1.size() == 0 || nodeL->v_next_1.size() == 0)
		return;
	size_t i = 0;
	size_t j = 0;
	int sizeP1;
	vector<PathN3> &sNodeN3 = pv.snodeUS3[off];
	vector<SGraphNode *>::iterator n3start;
	vector<SGraphNode *>::iterator n3end;
	SGraphNode *nodeP1;
	SGraphNode *nodeN1;
	bool reservedN3 = false;
	int nodeLN1Size = nodeL->v_next_1.size();
	int n3Count = 0;
	//int CacheItem = CACHE_LINE_SIZE / sizeof(int);
	for (i = 0; i < nodeLN1Size; i += 1) {
		// push P1
		nodeP1 = nodeL->v_next_1[i];
		sizeP1 = nodeP1->v_next_1.size();
		j = 0;
		while (j < sizeP1) {
			// push P2 (N1)
			nodeN1 = nodeP1->v_next_1[j];
			j++;
			if (nodeN1 == nodeL)
			{
				continue;
			}
			if (reservedN3 == false) {
				reservedN3 = true;
				sNodeN3.resize(nodeLN1Size * MaxRecverCount * MaxRecverCount);
			}
			
			n3end = nodeN1->v_next_1.end();
			int np1id = nodeP1->id;
			int nn1id = nodeN1->id;
			for (n3start = nodeN1->v_next_1.begin(); n3start != n3end; n3start ++) { // vn3 可能包含 L
				sNodeN3[n3Count].toId = (*n3start)->id;
				sNodeN3[n3Count].paths1 = np1id;
				sNodeN3[n3Count].paths2 = nn1id;
				n3Count++;
			} // end P2(P3)

		} // end while N1(P2)
	} // end while P1
	sNodeN3.resize(n3Count);
	std::sort(sNodeN3.begin(), sNodeN3.end(), pathCompare); // cost 1s on 100w data
}

array<int, 7> cycle7[3000000];
void prepareTreeParts() {
	SGraphNode *nodeL;  int nodeLP1Size;
	SGraphNode *nodeP1; int nodeP1NSize;
	SGraphNode *nodeP2; int nodeP2NSize;
	SGraphNode *nodeP3; int nodeLN1Size;
	SGraphNode *nodeN1; int nodeN1NSize;
	SGraphNode *nodeN2; int nodeN2NSize;
	SGraphNode *nodeN3;
	int currentGroup = 0x01;
	int ni;
	int cycle7Count = 0;
	for (ni = 0; ni < Pub_graphSize; ni++) {
		nodeL = &pv.nodes[ni];
		nodeLP1Size = nodeL->v_next_1.size();
		nodeLN1Size = nodeL->v_last_1.size();
		if (nodeLP1Size == 0 || nodeLN1Size == 0) {
			continue;
		}
		currentGroup++;
		pv.rootNodes.push_back(nodeL);
		nodeL->group = currentGroup;
		for (int j = 0; j < nodeLN1Size; j++) {
			nodeN1 = nodeL->v_last_1[j];
			nodeN1->group = currentGroup;
			nodeN1NSize = nodeN1->v_last_1.size();
			for (int k = 0; k < nodeN1NSize; k++) {
				nodeN2 = nodeN1->v_last_1[k];
				if (nodeN2->id <= nodeL->id) { continue; }
				nodeN2->group = currentGroup;
				nodeN2NSize = nodeN2->v_last_1.size();
				for (int l = 0; l < nodeN2NSize; l++) {
					nodeN3 = nodeN2->v_last_1[l];
					if (nodeN3->id <= nodeL->id || nodeN3->id == nodeN1->id) 
					{ continue; }
					nodeN3->group = currentGroup;
				}
			}
		}
		
		for (int j = 0; j < nodeLP1Size; j++) {
			nodeP1 = nodeL->v_next_1[j];
			if (nodeP1->id == nodeL->id) {
				continue;
			}
			nodeP1->group = currentGroup;
			nodeP1NSize = nodeP1->v_next_1.size();
			for (int k = 0; k < nodeP1NSize; k++) {
				nodeP2 = nodeP1->v_next_1[k];
				if (nodeP2->id <= nodeL->id) { continue; }
				nodeP2->group = currentGroup;
				nodeP2NSize = nodeP2->v_next_1.size();
				for (int l = 0; l < nodeP2NSize; l++) {
					nodeP3 = nodeP2->v_next_1[l];
					if (nodeP3->id <= nodeL->id || nodeP3->id == nodeP1->id)
					{ continue; }
					nodeP3->group = currentGroup;
					nodeN2NSize = nodeP3->v_next_1.size();
					for (int m = 0; m < nodeN2NSize; m++) {
						nodeN3 = nodeP3->v_next_1[m];
						if (nodeN3->group != currentGroup || nodeN3->id <= nodeL->id
							|| nodeN3->id == nodeP1->id || nodeN3->id == nodeP2->id) {
							continue;
						}
						for (int n = 0; n < nodeLN1Size; n++) {
							nodeN1 = nodeL->v_last_1[n];
							if (nodeN1->id <= nodeL->id || nodeN1->id == nodeP1->id
								|| nodeN1->id == nodeP2->id || nodeN1->id == nodeP3->id) {
								continue;
							}
							
							for (int p = 0; p < nodeN1->v_last_1.size(); p++) {
							}
							//vector<PathN3> &pathN1 = pv.lastUS2[nodeN1->idx];
							//for (int p = 0; p < pathN1.size(); p++) {
							//	if (pathN1[p].toId == nodeN3->id) {
							//		/*cycle7[cycle7Count][0] = nodeL->id;
							//		cycle7[cycle7Count][1] = nodeP1->id;
							//		cycle7[cycle7Count][2] = nodeP2->id;
							//		cycle7[cycle7Count][3] = nodeP3->id;
							//		cycle7[cycle7Count][4] = pathN1[p].toId;
							//		cycle7[cycle7Count][5] = pathN1[p].paths1;
							//		cycle7[cycle7Count][6] = nodeN1->id;*/
							//		cycle7Count++;
							//	}
							//}
						}
					}
				}

			}
		}
	}
	cout << "[D] cycle7: " << cycle7Count << endl;
}


class Intersection {
public:
	Intersection() {
		v_intersectionL1.resize(MaxRecverCount * 5);
	}
	~Intersection() {
		v_intersectionL1.clear();
	}
	void dis2Node(SGraphNode *n1, SGraphNode *n2) {
		end = std::set_intersection(n1->v_next_1.begin(),
			n1->v_next_1.end(),
			n2->v_last_1.begin(),
			n2->v_last_1.end(),
			v_intersectionL1.begin());
		start = v_intersectionL1.begin();
	}

public:
	std::vector<SGraphNode *>::iterator start;
	std::vector<SGraphNode *>::iterator end;
	// max 50 times compare
	std::vector<SGraphNode *> v_intersectionL1;
};

/* 用于储存可能构成环的顶点和 Connectivitiy 对象
* 包含一个双向栈，NodeL表示当前测试的节点，P表示下一级节点，N表示上一级节点
*/
#ifdef _MSC_VER
#pragma pack(push, 16)
#endif
class CircleChecker {
#define NodeLPos 0
#define NodeL Node[NodeLPos]
/* id */
#define NodeLId	   NodeIds[NodeLPos]
/* 1 <= i <= 3 */
#define NodeNPos(i) (i * 2)
/* 1 <= i <= 3 */
#define NodePPos(i) (i * 2 - 1)
	/*NodeId 不能用临时变量替换，但Node可以*/
#define NodePId(i) NodeIds[NodePPos(i)]
#define NodeNId(i) NodeIds[NodeNPos(i)]
#define NodeP(i) Node[NodePPos(i)]
#define NodeN(i) Node[NodeNPos(i)]
public:
	
	/* start > 0, 0 表示 NodeL 节点，(包含start）
	end > 0，大于 0 表示 P 节点 （包含end）
	 -- start 和 end 都会加上响应的偏移 --
	2，3，4 就是检测大小为 3 的环是否存在 
	环存在的必要条件是 end 的 next 节点中 存在 start
	*/
	/* 1 <= i <= 6 */
	/* 检测环 4 5 6 7的情况，并将其保存到数组中 DFS */ 
	void run(size_t off) {
		//return;
		NodeL = &pv.nodes[off];
		if (NodeL->last1MaxLess() || NodeL->next1MaxLess()) 
			return;
		NodeLId = NodeL->id;
		size_t i = 0, j = 0;
		size_t iend = NodeL->v_next_1.size();
		size_t jend = NodeL->v_last_1.size();
		while (i < iend) {
			// p layer 1
			NodeP(1) = NodeL->v_next_1[i];
			i++;
			NodePId(1) = NodeP(1)->id;
			if (NodePId(1) < NodeLId)  // P1 不会和 L 相同
			{ continue; }
			_run5();
			_run6();
			j = 0;
			while (j < jend) {
				// push n layer -1
				NodeN(1) = NodeL->v_last_1[j];
				j++;
				NodeNId(1) = NodeN(1)->id;
				if (NodeNId(1) < NodeLId || NodeNId(1) == NodePId(1))
				{ continue; }
				if (binary_search(NodeN(1)->v_last_1.begin(), NodeN(1)->v_last_1.end(), NodeP(1))) {
					circles[0]->addCircle(NodeIds);
				}
				_inter1.dis2Node(NodeP(1), NodeN(1));
				while (_inter1.start != _inter1.end) {
					NodePId(2) = (*_inter1.start)->id;
					if (NodePId(2) > NodeLId) {
						circles[1]->addCircle(NodeIds); // ringsize == 4
					}
					_inter1.start++;
				} // end of RingCheck 4
			} // end of while Node N Idx(1) n layer 1
		} // end of Node P Idx(1) p layer 1
		i = 0;
		while (i < iend) {
			// p layer 1
			NodeP(1) = NodeL->v_next_1[i];
			i++;
			NodePId(1) = NodeP(1)->id;
			if (NodePId(1) < NodeLId)  // P1 不会和 L 相同
			{ continue; }
			j = 0;
			while (j < jend) {
				// push n layer -1
				NodeN(1) = NodeL->v_last_1[j];
				j++;
				NodeNId(1) = NodeN(1)->id;
				if (NodeNId(1) < NodeLId || NodeNId(1) == NodePId(1))
				{ continue; }
				_run7(); // cost 1.6s seconds/100w data 1core
			} // end of while Node N Idx(1) n layer 1
		} // end of Node P Idx(1) p layer 1
	} // end method run

	// process layer >= 2, for example P2-N1 check ring size 4
	void _run5() {
		vector<PathN3> &vn3 = pv.snodeUS3[NodeP(1)->idx];
		vector<SGraphNode *> &vl2 = NodeL->v_last_1;
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<PathN3>::iterator tf2;
		vector<SGraphNode *>::iterator vl2start = vl2.begin();
		vector<SGraphNode *>::iterator vl2end = vl2.end();
		vector<SGraphNode *>::iterator te2;
		vector<SGraphNode *>::iterator tf1;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start)->id > (*vn3start).toId) {
				vn3start++;
			}
			else if ((*vl2start)->id < (*vn3start).toId) {
				vl2start++;
			}
			else {
				tf2 = vn3start;
				tf1 = vl2start;
				while (tf2 != vn3end && (*tf2).toId == (*vn3start).toId) {
					tf2++;
				}
				while (tf1 != vl2end && (*tf1)->id == (*vl2start)->id) {
					tf1++;
				}
				while (vn3start < tf2) {
					te2 = vl2start;
					while (te2 < tf1) {
						NodeNId(1) = (*te2)->id;
						NodePId(2) = (*vn3start).paths1;
						NodeNId(2) = (*vn3start).paths2;
						te2++;
						if (NodeNId(1) < NodeLId || NodeNId(2) <= NodeLId
							|| NodePId(2) <= NodeLId) {
							continue;
						}
						if (NodeNId(1) == NodePId(1) || NodeNId(1) == NodePId(2)
							|| NodeNId(2) == NodePId(1)
							) {
							continue;
						}
						circles[2]->addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			}
		}
	}
	void _run6() { 
		vector<PathN3> &vn3 = pv.snodeUS3[NodeP(1)->idx];
		vector<PathN3> &vl2 = pv.lastUS2[NodeL->idx];
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<PathN3>::iterator vl2start = vl2.begin();
		vector<PathN3>::iterator vl2end = vl2.end();
		vector<PathN3>::iterator te2;
		vector<PathN3>::iterator tf2;
		vector<PathN3>::iterator tf1;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start).toId > (*vn3start).toId) {
				vn3start++;
			}
			else if ((*vl2start).toId < (*vn3start).toId) {
				vl2start++;
			}
			else {
				tf2 = vn3start;
				tf1 = vl2start;
				while (tf2 != vn3end && (*tf2).toId == (*vn3start).toId) {
					tf2++;
				}
				while (tf1 != vl2end && (*tf1).toId == (*vl2start).toId) {
					tf1++;
				}
				while (vn3start < tf2) {
					te2 = vl2start;
					while (te2 < tf1) {
						NodeNId(1) = (*te2).paths1;
						NodeNId(2) = (*te2).toId;
						NodePId(2) = (*vn3start).paths1;
						NodePId(3) = (*vn3start).paths2;
						te2++;
						if (NodeNId(1) < NodeLId || NodeNId(2) <= NodeLId
							|| NodePId(2) <= NodeLId || NodePId(3) <= NodeLId) {
							continue;
						}
						if (NodeNId(1) == NodePId(1) || NodeNId(1) == NodePId(2) || NodeNId(1) == NodePId(3)
							|| NodeNId(2) == NodePId(1) || NodeNId(2) == NodePId(2)
							) {
							continue;
						}
						circle6.addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			}
		}
	} // end of function _run6
	
	void _run7() { 
		//  当前 N1 P1 都已经确定，通过 P1 和 N1 确定 N3，然后确定整个环
		vector<PathN3> &vn3 = pv.snodeUS3[NodeP(1)->idx];
		vector<PathN3> &vl2 = pv.lastUS2[NodeN(1)->idx];
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<PathN3>::iterator vl2start = vl2.begin();
		vector<PathN3>::iterator vl2end = vl2.end();
		vector<PathN3>::iterator te2;
		vector<PathN3>::iterator tf2;
		vector<PathN3>::iterator tf1;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start).toId > (*vn3start).toId) {
				vn3start++; // vl2begin > vn3begin
			}
			else if ((*vl2start).toId < (*vn3start).toId) {
				vl2start++;
			}
			else {
				tf2 = vn3start;
				tf1 = vl2start;
				while (tf2 != vn3end && (*tf2).toId == (*vn3start).toId) {
					tf2++;
				}
				while (tf1 != vl2end && (*tf1).toId == (*vl2start).toId) {
					tf1++;
				}
				while(vn3start < tf2) {
					te2 = vl2start;
					NodePId(2) = (*vn3start).paths1;
					NodePId(3) = (*vn3start).paths2;
					NodeNId(3) = (*vn3start).toId;
					if (NodeNId(3) <= NodeLId || NodeNId(3) == NodePId(1)
						|| NodeNId(3) == NodePId(2) || NodeNId(3) == NodeNId(1)
						|| NodePId(3) <= NodeLId || NodeNId(1) == NodePId(3)
						|| NodeNId(1) == NodePId(2)) {
						vn3start++;
						continue;
					}
					while(te2 < tf1) {
						NodeNId(2) = (*te2).paths1;
						te2++;
						if (NodeNId(2) <= NodeLId
							|| NodePId(2) <= NodeLId ) {
							continue;
						}
						if (NodeNId(2) == NodePId(1) || NodeNId(2) == NodePId(2)
							|| NodeNId(2) == NodePId(3)
							) {
							continue;
						}
						circle7.addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			} // end of else
		}
	} // end of function _run7

	Intersection _inter1; // 用于求交集
	CircleChecker(): circle6(200000),
		circle7(300000, 350000) 
	{
		// preserve stack size
	}
	~CircleChecker() {}
// ------------- 双向 stack -----------------
	SGraphNode *Node[4]; // 0 存储 L 节点，1-3-5存储P节点，2-4-6存储N节点
	//SShadowGraphNode *SNode[MaxCircleSize];
	/* 尝试的索引,并非全局索引 */
	/* 保存查找的索引，便于回溯 */
	int NodeIds[8]; // 0 存储 L 节点id，1-3-5存储P节点Id，2-4-6存储N节点Id
// ------------- end of stack -----------------


// --------------- start of circles -----------------------
	//_Circle<3> circle3;
	//_Circle<4> circle4;
	//_Circle<5> circle5;
	_Circle<6> circle6;
	Cycle7 circle7;
	CircleInterface *circles[CircleSizeCount];
	int _circleCount = 0;
	//public:
	void initCircles() {
		circles[0] = new _Circle<3>();
		circles[1] = new _Circle<4>();
		circles[2] = new _Circle<5>();
		circles[3] = &circle6;
		circles[4] = &circle7;
	}

	void cacheCircleCount() {
		if(_circleCount != 0) { return; }
		for (auto c : circles) {
#ifdef TEST_RUNLARGE
			cout << " Size " << c->size() << endl;
#endif
			_circleCount += c->size();
		}
	}
	
// --------------- end of circles -----------------------
}
#ifndef _MSC_VER
__attribute__((aligned(16)))
#endif
; // end of class CircleChecker
#ifdef _MSC_VER
#pragma pack(pop)
#endif

#if defined(FULL_DATA_INFO)
#include <chrono> 
#include <ctime>
#include <iomanip>
using namespace std::chrono;
static void printNow(const string &hint) {
	auto stop = high_resolution_clock::now();
	auto now = system_clock::to_time_t(system_clock::now());
	std::cout << hint << std::put_time(std::localtime(&now), " %F %T") << endl;
}
#endif

CircleChecker cc[DefaultWorkerNum];

/* 为每个节点构建 layer2 and layer3 属性 */
static void *constructLayer2(void *id) {
	size_t gid = (size_t)(id); // group id
	size_t wid = gid; // worker id
	size_t start;
	size_t end;
#if defined(FULL_DATA_INFO)
	cout << "[Info] Worker Group Construct Layer2: " << wid << endl;
#endif
	while (wid < WorkerNum) {
		start = Pub_workNodes[wid].offset;
		end = Pub_workNodes[wid].end;
		while (start < end) {
			prepareLayer2(start);
			//prepareLayer3(start);
			start += 1;
		} // end of loop (off < end)
		wid += MaxThreadNum;
	}
	return NULL;
}

/*p 为 worker id */
void *doJobs(void *id) {
	size_t gid = (size_t)(id); // group id
	size_t wid = gid; // worker id start
	size_t start;
	size_t end;
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
		cout << "Worker Group start at: " << wid << endl;
#endif
	while (wid < WorkerNum) {
		cc[wid].initCircles();
		start = Pub_workNodes[wid].offset;
		end = Pub_workNodes[wid].end;
		while (start < end) {
			cc[wid].run(start);
			start += 1;
		} // end of loop (off < end)
		wid += MaxThreadNum;
	}
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	cout << "Worker Group [" << gid << "] ** Jobs all done " << end << endl;
#endif
	// sort circles
	wid = gid;
	while (wid < WorkerNum) {
#if defined(MY_DEBUG) || defined(FULL_DATA_DEBUG)
		cout << "Worker sort " << wid << " start " << start << " " << end << "\t";
#endif
		for(int index = 1; index < CircleSizeCount; index ++)
			cc[wid].circles[index]->sort();
		cc[wid].cacheCircleCount();
		wid += MaxThreadNum;
	}

	return NULL;
}
void *deinitNodes(void *id) {
	idxMap.clear(); // release idxMap for not used
	// release memory for nodes
	Pub_workNodes.clear();
	Pub_workNodes.shrink_to_fit();
	pv.nodes.clear();
	pv.nodes.shrink_to_fit();
	pv.snodeUS3.clear();
	pv.snodeUS3.shrink_to_fit();
	pv.lastUS2.clear();
	//pv.lastUS2.shrink_to_fit();
#ifdef FULL_DATA_INFO
	cout << "[Info] deinitNodes All. " << endl;
#endif
	return NULL;
}

void *deinitCheckers(void *id) {
	size_t gid = (size_t)(id); // group id
	size_t wid = gid;
	wid = gid;
	while (wid < WorkerNum) {
		wid += MaxThreadNum;
	}
#ifdef FULL_DATA_INFO
	cout << "[Info] deinit workers. " << gid << endl;
#endif
	return NULL;
}

#define SIZE_KB 1024
#define SIZE_MB (1024 * 1024)

pthread_t pId[MaxThreadNum + 1]; // +1 便于在后台执行清理工作

int main(int argc, char *argv[]) {
#ifdef USE_TESTER
	auto now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIS] " << std::put_time(std::localtime(&now), "%F %T") << endl;
	auto time_start = high_resolution_clock::now();
#endif
#ifdef FULL_DATA_INFO
	cout << "Structure Size: "
		<< " Transfer " << sizeof(STransfer) << endl
		<< " GraphNode " << sizeof(SGraphNode) << " " << sizeof(unordered_set<SGraphNode *>)
		<< endl
		<< " CircleCheker " << sizeof(CircleChecker)
		<< " Circles " << sizeof(_Circle<3>) << " Circles " << sizeof(_Circle<7>) << endl
		<< "Thread Nums " << MaxThreadNum << " Worker nums " << WorkerNum << endl
		<< endl;
#endif
	size_t i;
	int ret;
	// start of main
	readfile();
	// ----------- start prepare ---------------
	pv.prepareGraphNode();
	// prepare layer2 and layer3 ids
#if 0
	ret = pthread_create(&pId[0], NULL, prepareGraphNode_n1, (void *)0);
	ret = pthread_create(&pId[1], NULL, prepareGraphNode_l1, (void *)1);
#else
	prepareGraphNode_n1(NULL);
	prepareGraphNode_l1(NULL);
#endif
	prepareWork();
	Pub_graphNoId.clear();

	pv.resize();

#if 0
	pthread_join(pId[0], NULL);
	pthread_join(pId[1], NULL);
#endif
	// -------- end of prepare  ---------------
	// -------- start construct layer2 or layer3 ---------
	for (i = 1; i < MaxThreadNum; i += 1) {
		ret = pthread_create(&pId[i], NULL, constructLayer2, (void *)i);
		if (ret != 0)
		{
			cout << "[Error] create init pthread error on " << i << endl;
			exit(1);
		}
	}
	constructLayer2((void *)0);

	for (i = 1; i < MaxThreadNum; i += 1) {
		pthread_join(pId[i], NULL);
	}
	// -------- end of construct layer2 or layer3 ---------
	prepareTreeParts();

	// -------- start do jobs ---------
	//创建子线程，线程id为pId
	for (i = 1; i < MaxThreadNum; i ++ ) {
		ret = pthread_create(&pId[i], NULL, doJobs, (void *)i);
		if (ret != 0)
		{
			cout << "[Error] create init pthread error on " << i << endl;
			exit(1);
		}
	}
	//doJobs((void *)0);
	for (i = 1; i < MaxThreadNum; i++) {
		pthread_join(pId[i], NULL);
	}

	// -------- end of jobs ---------
	// 无论 MaxThreadNum 为多少，清除工作都在后台执行
	ret = pthread_create(&pId[0], NULL, deinitNodes, (void *)0);
	int circleCount = 0;
	for (size_t i = 0; i < WorkerNum; i += 1) {
		circleCount += cc[i]._circleCount;
	}

#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	std::cout << "[Info] Total circles " << circleCount << endl;
	printNow("[TI]");
#endif

#if !defined(ON_WIN) && !defined(NotOutputToFile)
	int fd = open(DataOutputFile, O_RDWR | O_CREAT, (mode_t)0666);
	if(fd == -1) {
		cout << "[E] Output to file  " << endl;
		close(fd);
		return 1;
	}
	
	const int iMaxFileSize = 300 * SIZE_MB;
	ret = fallocate(fd, 0, 0, iMaxFileSize);
	if(ret == -1) {
		cout << "Fallocate file failed" << endl;
		close(fd);
		return 1;
	}
	char *mappedFileAddr = (char *) mmap(NULL, iMaxFileSize, PROT_WRITE, MAP_SHARED, fd, 0);
	if(!mappedFileAddr) {
		cout << "[Output] error mapping" << endl;
		munmap(mappedFileAddr, iMaxFileSize);
		close(fd);
		return 1;
	}
	size_t bytesStart = sprintf(mappedFileAddr, "%d\n", circleCount);
	size_t bytesWrite = 0;
	
	for (size_t index = 0; index < CircleSizeCount - 1; index ++ ) {
		for (i = 0; i < WorkerNum; i ++ ) {
			bytesWrite = cc[i].circles[index]->print(mappedFileAddr + bytesStart);
			bytesStart += bytesWrite;
		}
	}
	for (i = 0; i < WorkerNum; i ++ ) {
		// bytesWrite = cc[i].circle7.print(mappedFileAddr + bytesStart);
		size_t ci = 0;
		size_t cj = 0;
		vector<array<int, 7>> &cycle7 = cc[i].circle7.circle;
		bytesWrite = bytesStart;
		for (ci = 0; ci < cc[i].circle7._circleCount; ci++) {
			bytesWrite = sprintf(mappedFileAddr + bytesStart, 
				"%d,%d,%d,%d,%d,%d,%d\n", cycle7[ci][0], cycle7[ci][1], cycle7[ci][2],
				cycle7[ci][3],cycle7[ci][4],cycle7[ci][5], cycle7[ci][6]);
			bytesStart += bytesWrite;
		}
	}
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	cout << "[Info] bytes write: " << bytesStart 
		<< " Bytes,  " << (bytesStart / SIZE_KB) 
		<< " Kb " << (bytesStart / SIZE_MB) << " MB"  << endl;
#endif
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	printNow("before msync");
#endif
	msync(mappedFileAddr, bytesStart, MS_SYNC);
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	printNow("before munmap");
#endif
	munmap(mappedFileAddr, bytesStart);
	ftruncate(fd, bytesStart);
	close(fd);
#endif // end of ON_WIN

#if defined(ON_WIN) && !defined(NotOutputToFile)
	cout << "[Info] Output to file: result_" << DataInputFile << endl;
	ofstream output("result_" DataInputFile, std::ios::out);
	output << circleCount << endl;
	for (i = 0; i < WorkerNum; i++) {
		cc[i].circles[0]->output(output);
	}
	for (size_t index = 1; index < CircleSizeCount; index++) {
		for (i = 0; i < WorkerNum; i++) {
			cc[i].circles[index]->output(output);
		}
	}
	output.close();
#endif // end of ON_WIN
	// clean checkers

	// for (i = 0; i < MaxThreadNum; i += 1) {
	// 	// ret = pthread_create(&pId[i], NULL, deinitCheckers, (void *)i);
	// 	deinitCheckers((void *) i);
	// }

	for (i = 0; i < 1; i += 1) { // 0 确保 nodes 被 deinit 掉
		pthread_join(pId[i], NULL);
	}

#ifdef USE_TESTER
	auto stop = high_resolution_clock::now();
	now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIE] " << std::put_time(std::localtime(&now), "%F %T") << endl;
	auto duration = duration_cast<seconds>(stop - time_start);
	double passed = duration.count();
	if (passed < 1.0) {
		auto mduration = duration_cast<milliseconds>(stop - time_start);
		passed = mduration.count();
		std::cout << "Elapsed: " << passed << " milliseconds " << endl << endl;
	}
	else {
		auto mduration = duration_cast<milliseconds>(stop - duration - time_start);
		std::cout << "Elapsed: " << passed << " seconds "
			<< mduration.count() << " ms " << endl << endl;

	}
#endif
	return 0;
}