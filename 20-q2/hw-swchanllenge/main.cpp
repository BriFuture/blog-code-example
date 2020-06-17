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
#define DefaultWorkerNum 24

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
	/** 检测出的环数，不包含重复 */
	int _circleCount = 0;
	int _maxSize;
	bool _inited = false;
	vector<array<int, _Size>> circle;
	// todo 尽可能给大环增加size
	int _incSize = 0;
}
#ifndef _MSC_VER
__attribute__((aligned(16)))
#endif
; // end of class definition
#ifdef _MSC_VER
#pragma pack(pop)
#endif

// ============ end of CircleContainer ================

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
	
	inline int degree() {
		return v_next_1.size() + v_last_1.size();
	}

	inline bool last1MaxLess() {
		if (v_last_1.size() == 0) { return true; }
		return (*v_last_1.rbegin())->id < id;
	}
	inline int last1MaxId() {
		// last is reversed
		if (v_last_1.size() == 0) { return -1; }
		return (*v_last_1.rbegin())->id;
	}
	inline bool next1MaxLess() {
		if (v_next_1.size() == 0) { return true; }
		return (*v_next_1.rbegin())->id < id;
	}
	inline int next1MaxId() {
		if (v_next_1.size() == 0) { return -1; }
		return (*v_next_1.rbegin())->id;
	}

	SGraphNode()
	{
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

// ============== end of structure definitions ======

#define MaxNodeCount 100000
size_t Pub_graphSize = 0;
vector<SGraphNode> Pub_nodes;

struct SGraphNodeConn2{
	unordered_map<int, vector<int>> umv2;
	//vector<int> next2;
};
#ifdef USE_NODE_LAYER_2
vector<SGraphNodeConn2> Pub_snodeUMV2; // 表示第2级的 next 集合,不需要重复，序号和 Pub_nodes 序号相同
#endif

struct PathN3 {
	int paths1; // skip is 2 now
	int paths2; // skip is 2 now
	int toId = -1;
	PathN3() {}
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

template<class ForwardIt>
inline void path_binary_find(ForwardIt &first, ForwardIt last, int value)
{
	first = std::lower_bound(first, last, value, pathCompare);
	//if(pathCompare(value, *first)) // value < *first, not found
	//	first = last;
	//return first;
}

void path_binary_find(vector<PathN3> &path, vector<SGraphNode*> &nodes,
	vector<PathN3>::iterator &first, vector<PathN3>::iterator &last)
{
	first = std::lower_bound(
		path.begin(), path.end(), nodes[0]->id, pathCompare);
	last = std::upper_bound(
		path.begin(), path.end(), (*nodes.rbegin())->id, pathCompare);
	if (std::distance(first, last) <= 0) {
		first = path.end();
	}
	//return last;
}

// 一级的索引和 Pub_nodes 对应，二级表示到特定点的位置
vector<vector<PathN3>> Pub_snodeUS3;
#ifdef USE_NODE_LAST_3
vector<vector<PathN3>> Pub_lastUS3;
#endif
unordered_map<int, int> idxMap; // id -> idx
vector<WorkNode> Pub_workNodes;


/* 将 STransfer 结构体转变成邻接矢量 */
void prepareGraphNode() {
	// construct GraphNodes

	int lastId = -1;
	int iter = 0;
	Pub_nodes.reserve(vecLines.size() << 2); // max 2 times of lines
	for (auto it = Pub_graphNoId.begin(); it != Pub_graphNoId.end(); it ++) {
		if (lastId != *it) {
			lastId = *it;
			Pub_nodes.emplace_back();
			SGraphNode &gn = Pub_nodes[iter];
			gn.idx = iter;
			gn.id = lastId;
			idxMap[lastId] = iter;
			iter += 1;
		}
	}
	Pub_graphSize = iter;
	Pub_nodes.resize(Pub_graphSize);
} // end of prepareGraphNode

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
			gn = &Pub_nodes[lastIdx];
		}
		// idxMap 找到 idx，然后找到地址
		gn->v_next_1.push_back(&Pub_nodes[idxMap[line.t2]]);
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
	SGraphNode *gn;
	for (STransfer & line : vecLines2) {
		if (lastId != line.t2) {
			lastId = line.t2;
			gn = &Pub_nodes[idxMap[lastId]];
		}
		gn->v_last_1.push_back(&Pub_nodes[idxMap[line.t1]]);
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
#ifdef MY_DEBUG
		Pub_workNodes[i].debugid = i;
#endif
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


#if defined(USE_NODE_LAST_2) && 0
	//std::vector<SGraphNode *> s_intersection;
	std::vector<SGraphNode *> v_intersectionL2;
	std::vector<SGraphNode *>::iterator intersectionL2Start;
	std::vector<SGraphNode *>::iterator intersectionL2End;
	//  检测 n1->last_2 - 与 n2->next_1 交集
	inline void dis3NodeItersection(SGraphNode *n1, SGraphNode *n2) {
		intersectionL2End = std::set_intersection(sN1->s_last_2.begin(),
			sN1->s_last_2.end(),
			n2->v_next_1.begin(),
			n2->v_next_1.end(),
			v_intersectionL2.begin());
	}
#endif
public:
#ifdef USE_NODE_LAYER_2
	std::vector<int>::iterator start;
	std::vector<int>::iterator end;
#else
	std::vector<SGraphNode *>::iterator start;
	std::vector<SGraphNode *>::iterator end;
	// max 50 times compare
	std::vector<SGraphNode *> v_intersectionL1;
#endif
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
#define NodePId(i) NodeIds[NodePPos(i)]
#define NodeNId(i) NodeIds[NodeNPos(i)]
#define NodeP(i) Node[NodePPos(i)]
#define NodeN(i) Node[NodeNPos(i)]

#define Circles(s) circles[s - MinCircleSize]
#define RingCheck(s) shouldRingCheck[s - 5] // 只有 5 和 6 需要进行 RingCheck
#define BFind binary_find
public:
	enum RingCheckStatus {
		CheckedBypass = 0x0, //  预检查过，不可能构成环
		NotChecked = 0x1, // 尚未检查过
		CheckedPass = 0x2, // 预检查过，可能构成环
	};
	/* start > 0, 0 表示 NodeL 节点，(包含start）
	end > 0，大于 0 表示 P 节点 （包含end）
	 -- start 和 end 都会加上响应的偏移 --
	2，3，4 就是检测大小为 3 的环是否存在 
	环存在的必要条件是 end 的 next 节点中 存在 start
	*/
	void _check3() {
		// L- P1 -N1
		auto np = Node[NodeNPos(1)]->v_next_1;
		if (binary_search(np.begin(),
			np.end(), NodeL)) {
			Circles(3)->addCircle(NodeIds);
		}
	}

	/* prepare 所有的Node全部准备完毕后就能检测环，但是 3环比较特殊，在本阶段就可以检测 */
	void prepareLayer_2(size_t off) {
		NodeL = &Pub_nodes[off];
		if (NodeL->v_last_1.size() == 0) return; 
		if (NodeL->v_next_1.size() == 0) return;
		NodeLId = NodeL->id;

		size_t i = 0;
		size_t j = 0;
		vector<SGraphNode *>::iterator gn2;
		vector<SGraphNode *>::iterator vecEnd;
		bool reservedN3 = false;
		vector<SGraphNode*>::iterator n3start;
		vector<SGraphNode*>::iterator n3end;
		int sizeP1;
#ifdef USE_NODE_LAST_3
		vector<PathN3> &sNodeL2 = Pub_lastUS3[off];
		for (i = 0; i < NodeL->v_last_1.size(); i += 1) {
			// push P1
			NodeN(1) = NodeL->v_last_1[i];
			NodeNId(1) = NodeN(1)->id;
			sizeP1 = NodeN(1)->v_last_1.size();

			if (reservedN3 == false) {
				reservedN3 = true;
				sNodeL2.reserve(NodeL->v_last_1.size() *
					MaxRecverCount);
			}
			//if (sizeP1 == 0) continue;
			NodeIdx[NodeNPos(2)] = 0;
			while (NodeIdx[NodeNPos(2)] < sizeP1) {
				// push P2 (N1)
				NodeN(2) = NodeN(1)->v_last_1[NodeIdx[NodeNPos(2)]];
				NodeIdx[NodeNPos(2)] ++;
				if (NodeN(2) == NodeL)
				{ continue; }
				NodeNId(2) = NodeN(2)->id;
				sNodeL2.emplace_back(NodeNId(2), NodeNId(1), 0);
			} // end while N1(P2)
			
		} // end while P1
		std::sort(sNodeL2.begin(), sNodeL2.end(), pathCompare);
#endif
		reservedN3 = false;
		vector<PathN3> &sNodeN3 = Pub_snodeUS3[off];
		for (i = 0; i < NodeL->v_next_1.size(); i += 1) {
			// push P1
			NodeP(1) = NodeL->v_next_1[i];
			NodePId(1) = NodeP(1)->id;
			sizeP1 = NodeP(1)->v_next_1.size();
		
			//if (sizeP1 == 0) continue;
			NodeIdx[NodeNPos(1)] = 0;
			while (NodeIdx[NodeNPos(1)] < sizeP1) {
				// push P2 (N1)
				NodeN(1) = NodeP(1)->v_next_1[NodeIdx[NodeNPos(1)]];
				NodeIdx[NodeNPos(1)] ++;
				if (Node[NodeNPos(1)] == NodeL) 
				{ continue; }
				NodeIds[NodeNPos(1)] = Node[NodeNPos(1)]->id;
				if (NodeIds[NodeNPos(1)] > NodeLId &&
					NodeIds[NodePPos(1)] > NodeLId )
				{
					_check3(); // ringsize == 3
				}
				if (reservedN3 == false) {
					reservedN3 = true;
					sNodeN3.reserve(NodeL->v_next_1.size() *
						MaxRecverCount * MaxRecverCount);
				}
				n3start = Node[NodeNPos(1)]->v_next_1.begin();
				n3end   = Node[NodeNPos(1)]->v_next_1.end();
				while(n3start != n3end) { // vn3 可能包含 L 
					sNodeN3.emplace_back((*n3start)->id, NodePId(1), NodeNId(1));
					n3start++;
				} // end P2(P3)
			} // end while N1(P2)
			
		} // end while P1
		std::sort(sNodeN3.begin(), sNodeN3.end(), pathCompare);
		//sNodeN3.erase(std::unique(sNodeN3.begin(), sNodeN3.end()), sNodeN3.end());
	}
	/* 1 <= i <= 6 */
/* i 表示数组序号，第1层将会修改 Node[1] 的值
@param node
@param i 为 1-6
@return True 表示 Pid 比 Lid 小，或者其它 需要过滤的情况
*/
	/* 检测环 4 5 的情况，并将其保存到数组中 DFS */ 
	void run(size_t off) {
		NodeL = &Pub_nodes[off];
		if (NodeL->last1MaxLess() || NodeL->next1MaxLess()) 
			return;
		NodeLId = NodeL->id;
		for (auto nodeP1 : NodeL->v_next_1) {
			// p layer 1
			NodeP(1) = nodeP1;
			NodeIds[NodePPos(1)] = NodeP(1)->id;
			if (NodeIds[NodePPos(1)] < NodeLId)  // P1 不会和 L 相同
			{ continue; }
			for (auto nodeN1 : NodeL->v_last_1) {
				// push n layer -1
				NodeN(1) = nodeN1;
				NodeIds[NodeNPos(1)] = NodeN(1)->id;
				if (NodeIds[NodeNPos(1)] < NodeLId ||
					NodeIds[NodeNPos(1)] == NodeIds[NodePPos(1)])
				{ continue; }
				_inter1.dis2Node(NodeP(1), NodeN(1));
				while (_inter1.start != _inter1.end) {
#ifdef USE_NODE_LAYER_2
					NodeIds[NodePPos(2)] = *_inter1.start;
#else
					NodeIds[NodePPos(2)] = (*_inter1.start)->id;
#endif
					if (NodeIds[NodePPos(2)] > NodeLId) {
						Circles(4)->addCircle(NodeIds); // ringsize == 4
					}
					_inter1.start++;
				} // end of RingCheck 4
				NodeIdx[NodePPos(2)] = 0;
				_run7(); // cost 3.7s seconds/100w data 1core
				//_run5();
			} // end of while Node N Idx(1) n layer 1
			_run5();
			_run6();
		} // end of Node P Idx(1) p layer 1
	} // end method run

	// process layer >= 2, for example P2-N1 check ring size 4
	void _run5() {
		vector<PathN3> &vn3 = Pub_snodeUS3[NodeP(1)->idx];
		vector<SGraphNode *> &vl2 = NodeL->v_last_1;
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<SGraphNode *>::iterator vl2start = vl2.begin();
		vector<SGraphNode *>::iterator vl2end = vl2.end();
		vector<SGraphNode *>::iterator te2;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start)->id < (*vn3start).toId) {
				vl2start++;
			}
			else if ((*vl2start)->id > (*vn3start).toId) {
				vn3start++;
			}
			else {
				auto tf2 = vn3start;
				auto tf1 = vl2start;
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
						Circles(5)->addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			}
		}
	}
	void _run6() { 
		vector<PathN3> &vn3 = Pub_snodeUS3[NodeP(1)->idx];
		vector<PathN3> &vl2 = Pub_lastUS3[NodeL->idx];
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<PathN3>::iterator vl2start = vl2.begin();
		vector<PathN3>::iterator vl2end = vl2.end();
		vector<PathN3>::iterator te2;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start).toId < (*vn3start).toId) {
				vl2start++;
			}
			else if ((*vl2start).toId > (*vn3start).toId) {
				vn3start++;
			}
			else {
				auto tf2 = vn3start;
				auto tf1 = vl2start;
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
						Circles(6)->addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			}
		}
	} // end of function _run6
	void _run7() { 
		//  当前 N1 P1 都已经确定，通过 P1 和 N1 确定 N3，然后确定整个环
		///*
		vector<PathN3> &vn3 = Pub_snodeUS3[NodeP(1)->idx];
		vector<PathN3> &vl2 = Pub_lastUS3[NodeN(1)->idx];
		vector<PathN3>::iterator vn3start = vn3.begin();
		vector<PathN3>::iterator vn3end = vn3.end();
		vector<PathN3>::iterator vl2start = vl2.begin();
		vector<PathN3>::iterator vl2end = vl2.end();
		vector<PathN3>::iterator te2;
		while (vl2start != vl2end && vn3start != vn3end) {
			if ((*vl2start).toId < (*vn3start).toId) {
				vl2start++;
			}
			else if ((*vl2start).toId > (*vn3start).toId) {
				vn3start++;
			}
			else {
				auto tf2 = vn3start;
				auto tf1 = vl2start;
				while (tf2 != vn3end && (*tf2).toId == (*vn3start).toId) {
					tf2++;
				}
				while (tf1 != vl2end && (*tf1).toId == (*vl2start).toId) {
					tf1++;
				}
				while(vn3start < tf2) {
					te2 = vl2start;
					while(te2 < tf1) {
						NodeNId(2) = (*te2).paths1;
						NodeNId(3) = (*te2).toId;
						NodePId(2) = (*vn3start).paths1;
						NodePId(3) = (*vn3start).paths2;
						te2++;
						if (NodeNId(2) <= NodeLId || NodeNId(3) <= NodeLId
							|| NodePId(2) <= NodeLId || NodePId(3) <= NodeLId) {
							continue;
						}
						if (NodeNId(1) == NodePId(2) || NodeNId(1) == NodePId(3)
							|| NodeNId(3) == NodeNId(1)
							|| NodeNId(2) == NodePId(1) || NodeNId(2) == NodePId(2)
							|| NodeNId(2) == NodePId(3) || NodeNId(3) == NodePId(1)
							|| NodeNId(3) == NodePId(2)
							) {
							continue;
						}
						circle7.addCircle(NodeIds);
					}
					vn3start++;
				}
				vl2start = tf1;
			}
		}
	} // end of function _run7

	/* @param ci  表示当前P节点的层数  pi 表示 上一级节点
	使用 NodePPos 进行包裹 不过滤的条件是 Id 比 Lid 大，且栈中不存在相同的id
	@param end 表示检查栈中是否存在相同元素的索引，默认为 ci
	@return filter */
	inline bool tryP(int ci) {
		if (NodeIds[ci] > NodeLId) {
			for (int s = 1; s < ci; s++) {
				if (NodeIds[s] == NodeIds[ci]) {
					return true;
				}
			}
			return false;
		}
		return true;
	}

	Intersection _inter1; // 用于求交集
	CircleChecker(): circle6(200000), circle7(300000, 350000) 
	{
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
		id = 0;
		groupId = 0;
#endif
		// preserve stack size
		//NodeIds.resize(MaxCircleSize);
		for (int i = 0; i < MaxCircleSize; i += 1) {
			NodeIdx[i] = 0;
		}
		for (int i = 0; i < 2; i += 1) {
			shouldRingCheck[i] = NotChecked;
		}
		
	}
	~CircleChecker() {}
// ------------- 双向 stack -----------------
	SGraphNode *Node[MaxCircleSize]; // 0 存储 L 节点，1-3-5存储P节点，2-4-6存储N节点
	//SShadowGraphNode *SNode[MaxCircleSize];
	/* 尝试的索引,并非全局索引 */
	int NodeIdx[MaxCircleSize];  // 这里的 idx 不是 Pub 数组中的序号，而是尝试的次数
	RingCheckStatus shouldRingCheck[2]; // 3 - 4 - 7 不需要保存
	/* 保存查找的索引，便于回溯 */
	int NodeIds[MaxCircleSize]; // 0 存储 L 节点id，1-3-5存储P节点Id，2-4-6存储N节点Id
// ------------- end of stack -----------------
//public:
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	int id;
	int groupId;
#endif

// --------------- start of circles -----------------------
	CircleInterface *circles[CircleSizeCount];
	_Circle<3> circle3;
	_Circle<4> circle4;
	_Circle<5> circle5;
	_Circle<6> circle6;
	_Circle<7> circle7;
	void initCircles() {
#if defined(FULL_DATA_INFO) && 0
		cout << "Worker [" << id << " - " << groupId << "] Init Circles. " << endl;
#endif
		circles[0] = &circle3;
		circles[1] = &circle4;
		circles[2] = &circle5;
		circles[3] = &circle6;
		circles[4] = &circle7;
	}
	size_t sumCircles() {
		size_t count = 0;
		for (auto c : circles) {
			count += c->size();
		}
		return count;
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
	size_t _circleCount = 0;
// --------------- end of circles -----------------------
}
#ifndef _MSC_VER
__attribute__((aligned(16)))
#endif
; // end of class CircleChecker
#ifdef _MSC_VER
#pragma pack(pop)
#endif

#if defined(MY_INFO) || defined(FULL_DATA_INFO)
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
#ifdef USE_TESTER

typedef int(*TestFunc)(void);

static void TimeIt(TestFunc tf, int exe = 10) {
	auto now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIS] " << std::put_time(std::localtime(&now), "%F %T") << endl;
	auto start = high_resolution_clock::now();
	tf();
	auto stop = high_resolution_clock::now();
	now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIE] " << std::put_time(std::localtime(&now), "%F %T") << endl;
	auto duration = duration_cast<seconds>(stop - start);
	double passed = duration.count();
	if (passed < 1.0) {
		auto mduration = duration_cast<milliseconds>(stop - start);
		passed = mduration.count();
		std::cout << "Elapsed: " << passed << " milliseconds " << endl << endl;
	}
	else {
		auto mduration = duration_cast<milliseconds>(stop - duration - start);
		std::cout << "Elapsed: " << passed << " seconds " 
			<< mduration.count() << " ms " << endl << endl;

	}
}
#endif // end USE_TESTER

CircleChecker cc[DefaultWorkerNum];
//vector<CircleChecker> cc;

/* 为每个节点构建 layer2 and layer3 属性 */
void *constructLayer2(void *id) {
	size_t gid = (size_t)(id); // group id
	size_t wid = gid; // worker id
	size_t start;
	size_t end;
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	cout << "[Info] Worker Group Construct Layer2: " << wid << endl;
#endif
	while (wid < WorkerNum) {
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
		cc[wid].id = wid;
		cc[wid].groupId = gid;
#endif
		cc[wid].initCircles();
		start = Pub_workNodes[wid].offset;
		end = Pub_workNodes[wid].end;
		while (start < end) {
			cc[wid].prepareLayer_2(start);
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
		start = Pub_workNodes[wid].offset;
		end = Pub_workNodes[wid].end;
		while (start < end) {
			cc[wid].run(start);
			start += 1;
#if defined(MY_DEBUG) || defined(FULL_DATA_DEBUG)
			if(start % 100 == 0)
				cout << "Worker[" << wid << "] has done job " << start << endl;
#endif
		} // end of loop (off < end)
		wid += MaxThreadNum;
#if defined(MY_DEBUG) || defined(FULL_DATA_DEBUG)
		cout << "Worker [" << wid << " - " << gid << "] ** Jobs all done " << end << endl;
#endif
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
	// release memory for nodes
	Pub_workNodes.clear();
	Pub_workNodes.shrink_to_fit();
	Pub_nodes.clear();
	Pub_nodes.shrink_to_fit();
	Pub_snodeUS3.clear();
	Pub_snodeUS3.shrink_to_fit();
#ifdef USE_NODE_LAST_3
	Pub_lastUS3.clear();
	//Pub_lastUS3.shrink_to_fit();
#endif
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

#ifdef USE_TESTER
int test_readfile() {
#else
int main(int argc, char *argv[]) {
#endif
#ifdef FULL_DATA_INFO
	cout << "Structure Size: "
		<< " Transfer " << sizeof(STransfer) << endl
		<< " GraphNode " << sizeof(SGraphNode) << " " << sizeof(unordered_set<SGraphNode *>)
		<< " GraphNodeConn " << sizeof(SGraphNodeConn2) << endl
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
	prepareGraphNode();
	// prepare layer2 and layer3 ids
#if 1
	ret = pthread_create(&pId[0], NULL, prepareGraphNode_n1, (void *)0);
	ret = pthread_create(&pId[1], NULL, prepareGraphNode_l1, (void *)1);
#else
	prepareGraphNode_n1(NULL);
	prepareGraphNode_l1(NULL);
#endif
	prepareWork();
	Pub_graphNoId.clear();

#ifdef USE_NODE_LAST_3
	Pub_lastUS3.resize(Pub_graphSize);
#endif
	Pub_snodeUS3.resize(Pub_graphSize);
#if 1
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
	idxMap.clear(); // release idxMap for not used
#if !defined(TEST_LAYER2)
	constructLayer2((void *)0);
#else
	cc[0].initCircles();
	auto start = Pub_workNodes[0].offset;
	auto end = Pub_workNodes[0].end;
	while (start < end) {
		cc[0].prepareLayer_2(start);
		start += 1;
	} // end of loop (off < end)
#endif
	for (i = 1; i < MaxThreadNum; i += 1) {
		pthread_join(pId[i], NULL);
	}
	// -------- end of construct layer2 or layer3 ---------
	// -------- start do jobs ---------
	//创建子线程，线程id为pId
#ifndef TEST_RUNLARGE
	for (i = 1; i < MaxThreadNum; i ++ ) {
		ret = pthread_create(&pId[i], NULL, doJobs, (void *)i);
		if (ret != 0)
		{
			cout << "[Error] create init pthread error on " << i << endl;
			exit(1);
		}
	}
	doJobs((void *)0);
	for (i = 1; i < MaxThreadNum; i++) {
		pthread_join(pId[i], NULL);
	}
#else
	size_t gid = 0; // group id
	size_t wid = gid; // worker id start
	size_t start;
	size_t end;
	start = Pub_workNodes[wid].offset;
	end = Pub_workNodes[wid].end;
	while (start < end) {
		cc[wid].run(start);
		start += 1;
	} // end of loop (off < end)
	for (int index = 1; index < CircleSizeCount; index++)
		cc[wid].circles[index]->sort();
	cc[wid].cacheCircleCount();
#endif
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
		cout << "[Output ]Error " << endl;
		close(fd);
		return 1;
	}
	
	const int iMaxFileSize = 300 * SIZE_MB;
	const int iFileSizeInc = 20 * SIZE_MB;
	const int iFileMargin = 10 * SIZE_MB;
	int iSizeInc = 60 * SIZE_MB;
	int currentFileSize = 50 * SIZE_MB;
	ret = fallocate(fd, 0, 0, currentFileSize);
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
			if(bytesStart >= currentFileSize - iFileMargin) {
				fallocate(fd, 0, currentFileSize, iSizeInc);
				currentFileSize += iSizeInc;
				if(iSizeInc > iFileSizeInc) {
					iSizeInc -= (10 * SIZE_MB);
				}
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
				cout << "[Debug] on " << (iSizeInc  / SIZE_MB) << "MB now " << (currentFileSize / SIZE_MB) << endl;
#endif
			}
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
			if(bytesStart >= currentFileSize - iFileMargin) {
				fallocate(fd, 0, currentFileSize, iSizeInc);
				currentFileSize += iSizeInc;
				if(iSizeInc > iFileSizeInc) {
					iSizeInc -= (10 * SIZE_MB);
				}
#if defined(MY_INFO) || defined(FULL_DATA_INFO)
				cout << "[Debug 7] " << (iSizeInc  / SIZE_MB) << "MB now " << (currentFileSize / SIZE_MB)<< endl;
#endif
			}
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
#if defined(MY_DEBUG) || defined(FULL_DATA_DEBUG)
	cout << "[Debug ] w0" << endl;
#endif
	for (i = 0; i < 1; i += 1) { // 0 确保 nodes 被 deinit 掉
		pthread_join(pId[i], NULL);
	}
	return 0;
}

#ifdef USE_TESTER
int main(int argc, char *argv[])
{
	TimeIt(test_readfile, 1);
	return 0;
}
#endif
