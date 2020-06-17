#include <iostream>
#include <fstream>
#include <ostream>
#include <map>
#include <stack>

void printVecLines(vector<STransfer> &vecLines, int maxLine) {
	int i = 0;
	if (maxLine > static_cast<int>(vecLines.size()) || maxLine == -1) {
		maxLine = vecLines.size();
	}
	while (i < maxLine) {
		cout << "[" << i << "] " << vecLines[i].t1 << "  " << vecLines[i].t2 << endl;
		i += 1;
	}
}

#ifdef MY_DEBUG
void printVertex(int next) {
	for (auto v : _vertex) {
		cout << v << ", ";
	}
	cout << next << endl;
}
#endif
/** 在 vecLines 中的 STransfer 可能有多个 sender 是同一个id，用 map 将 id 和
* vecLines 中 STransfer 的序号映射起来
*
struct RangeMap {
	int id; // sender id
	int posStart; // 在 vecLines 中第一个出现的序号
	int posEnd; // 有记录的STransfer的最后一个之后一个的序号（不包含）
};
*/


template<size_t _Size>
struct SCircle {
	int step = _Size;
	vector<array<int, _Size>> data;
};

#if 0
	inline void next(int &value) {
		value = recvers[_currentRecvId];
		_currentRecvId += 1;
	}
	inline bool isLast() { return _currentRecvId == recvers.size(); }
#endif

void findConnBySenderId(int senderId, SConnectivity *& conn) {
	size_t end = vecConnSize;
	int id = -1;
	size_t start = 0;
	size_t mid = 0;
	while (end >= start) {
		mid = (start + end) >> 1;
		id = connIndex[mid];
		if (id < senderId) {
			start = mid + 1;
		}
		else if (id > senderId) {
			end = mid - 1;
		}
		else {
			conn = &vecConnectivities[mid];
			return;
		}
	}
}

		// 排序
		_rotate(tmpCircle);
		// 检测是否有重复的
		bool dump;
		for (int iter = 0; iter < _circleCount; iter += 1) {
			auto c = circle[iter];
			int i;
			dump = true;
			for (i = 0; i < _Size; i += 1) {
				if (c[i] != tmpCircle[i]) {
					dump = false;
				}
			}
			if (!dump) {
				continue;
			}
#ifdef MY_DEBUG
			cout << "[CS] Same Circle detected: ";
			_print(c, cout);
			_print(tmpCircle, cout);
#endif
			return;
		}
// ============== structure definitions ======
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

using namespace std;
//#define USE_THIRD_COL
/**
ת�˼�¼����ԭʼ txt �ļ�ת��Ϊ STransfer �ṹ��
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
	return s1.t1 < s2.t1; // || s1.t2 < s2.t2;
}

bool sortTransferByRecver(const STransfer &s1, const STransfer &s2) {
	return s1.t2 < s2.t2;
}

#define MaxRecverCount 12
#define MemoryBlockSize 4096
#define ConnUseArray 2  // 0 array 1 vector 2 None
/*
* ��ʾ�ڽӹ�ϵ
* �������Ϊ 0 �Ķ��㣬��ö���һ�����ڻ���
* sender id ��Ψһ�ģ�
* recvers ��ָ�����һ���ڵ�ļ���
*/
struct SConnectivity {
	int id; // sender �������е����
	int sender; // sender
	// ��һ���ڵ�
	size_t recverOffset = 0;
	size_t recverCount = 0;
	vector<SConnectivity *> last_1; 
	// distance 1 ��� next 1 Ϊ 0����û�л�·����� next_1 ����űȵ�ǰС�����⻷·ʱ��������������Ϊ nullptr
	vector<SConnectivity *> next_1; 
	vector<int> nextId_1; // recver �������е����
	SConnectivity() {
		last_1.reserve(MaxRecverCount);
		next_1.reserve(MaxRecverCount);
		nextId_1.reserve(MaxRecverCount);
	}
};

/* �����ָ����ݵĽڵ㣬 conn_offset ���� vecConnectivites �����е�ƫ�ƣ�recv_offset ���� Pub_Conn* �е�ƫ��
end ȫ���������������ͷ����ƫ��, offset �������ݶ�end ����
*/
struct Nodes {
	size_t id;
	size_t conn_offset;
	size_t conn_end;
};

// ============== end of structure definitions ======

#include <pthread.h>

#define ThreadNum 1

//#define LARGE_DATA 100

//#define FULL_DATA
#define FULL_DATA_INFO
#define FULL_DATA_DEBUG

#ifdef USE_TESTER
#if defined(LARGE_DATA)
#undef MY_INFO
#define DataInputFile "test_data_7342.txt"
//#define DataInputFile "test_data_38252.txt"
//#define DataInputFile "test_data_2861665.txt"
#else
//#define DataInputFile "test_data_1.txt"
#define DataInputFile "test_data.txt"
#endif
#define DataOutputFile "result.txt"
#else
#define DataInputFile "/data/test_data.txt"
#define DataOutputFile "/projects/student/result.txt"
#endif
/* ���� */
#define MaxCircleSize 7
#define MinCircleSize 3
#define CircleSizeCount (MaxCircleSize - MinCircleSize + 1)

//============================ CircleInterface ==================
class CircleInterface {
public:
	virtual int size() = 0;
	virtual void output(ostream &out) = 0;
	virtual bool isAdding() const = 0;
	virtual void addCircle(const vector<int>::iterator start, const vector<int>::iterator end) = 0;
};
/** �����������Ѿ������Ļ� ���Ĵ�С��Ԥ�ȶ���
*/
template<size_t _Size>
class _Circle : public CircleInterface {
public:
// 5w
#define INCREMENT_SIZE 50000
	_Circle() {
		circle.resize(INCREMENT_SIZE); 
	}
	bool isAdding() const { return false;  }
	void addCircle(const vector<int>::iterator start, const vector<int>::iterator end) {
		array<int, _Size> &tmpCircle = circle[_circleCount];
		// TODO �ҵ���С�ģ������� rotate
		for (auto it = start; it < end; it += 1) {
			tmpCircle[it - start] = *it;
		}
		if (!_isMinFirst(tmpCircle)) {
 			return;
		}
		_circleCount += 1;
#ifdef MY_DEBUG
		cout << "Got a circle, now circle: " << _circleCount << endl;
#endif
		if (_circleCount >= _maxSize) {
			_maxSize += INCREMENT_SIZE;
			circle.resize(_maxSize);
		}
	}
	/* �����õ��Ľ����ӡ����������ļ��� */
	inline void output(ostream &out) {
		for (int i = 0; i < _circleCount; i += 1) {
			_print(circle[i], out);
		}
	}

private:
	inline void _print(const array<int, _Size> &c, ostream &out) {
		int i;
		for (i = 0; i < _Size - 1; i += 1) {
			out << c[i] << ",";
		}
		out << c[i] << endl;
	}
	/* ��������С�����ƶ���������ǰ�� */
	void _rotate(array<int, _Size> &circle) {
#ifdef MY_DEBUG
		cout << "[CI] Before Rotate ";
		_print(circle, cout);
#endif
		int index = 0;
		int minIdx = 0;
		int min = circle[0]; // circle size >= 3
		// �ҳ���С����������
		while (index != _Size) {
			if (min > circle[index]) {
				min = circle[index];
				minIdx = index;
			}
			index += 1;
		}
		if (minIdx == 0) {
			return; // ����Ҫ�ƶ�
		}
		// ���õ�������
		index = 0;
		// �µĻ�
		array<int, _Size> tmpCircle;
		
		while (index < _Size) {
			tmpCircle[index] = circle[minIdx];
			minIdx += 1;
			if (minIdx >= _Size) {
				minIdx = 0;
			}
			index += 1;
		}
		memcpy(circle.data(), tmpCircle.data(), sizeof(int) * (_Size));
#ifdef MY_DEBUG
		cout << "[CI] After Rotate ";
		_print(circle, cout);
#endif
	} // end of sortCircle
	bool _isMinFirst(array<int, _Size> &circle) {
		int min = circle[0]; // circle size >= 3
		// �ҳ���С����������
		int index = 1;
		while (index != _Size) {
			if (min > circle[index]) {
				return false;
			}
			index += 1;
		}
		return true; // ����Ҫ�ƶ�
	}
	int size() { return _circleCount; }
private:
	/** �����Ļ������������ظ� */
	int _circleCount = 0;
	int _maxSize = INCREMENT_SIZE;
	vector<array<int, _Size>> circle;
};
// ============ end of CircleContainer ================

#if defined(MY_DEBUG) or defined(MY_INFO)
static const int VecLinesPreserveSize = 100000;
#else
static const int VecLinesPreserveSize = 1000000;
#endif
vector<STransfer> vecLines;
// ---------------- read from file ------------------
/** read from spec file with three columns,
	* first  column : sender
	* second column : recver
	* third  column : money
	* store data in structure STransfer, all STransfers are stored in vecLines with asc-order
	*/
void readfile()
{
	string line;
	ifstream myfile(DataInputFile);
	if (!myfile.is_open())
	{
#ifdef MY_INFO
		std::cout << "Unable to open file";
#endif
		exit(1);
		return;
	}
	size_t firstPos = 0;
	size_t secondPos = 0;
#ifdef USE_TESTER
	vecLines.clear();
#endif
	vecLines.reserve(VecLinesPreserveSize); // reserve size

	while (getline(myfile, line))
	{
		STransfer trans;
#if defined(MY_DEBUG) || defined(MY_INFO)
		if (line[0] == '#') continue;
#endif
		firstPos = line.find(',', 0U);
		secondPos = line.find(',', firstPos + 1);
		trans.t1 = stoi(line.substr(0, firstPos));
		trans.t2 = stoi(line.substr(firstPos + 1, secondPos - firstPos - 1));
#ifdef USE_THIRD_COL
		trans.money = stoi(line.substr(secondPos + 1));
#endif
		vecLines.push_back(trans);
	}
	myfile.close();
#ifdef MY_INFO
	std::cout << "[CC] File Lines " << vecLines.size() << endl;
#endif
}
//map<int, vector<int>> connectivities;
/* vector�д洢�� Connectivitiy
*/
vector<SConnectivity> Pub_vecConn;
vector<int> Pub_nextConnIndex; // ������洢 senderId��
size_t vecConnSize = 0;
/** idx Ӧ���� Pub_vecConn �п����ҵ�������Ҳ�����value��Ϊ-1 */
void findRidxBySid(int senderId, int &value) {
	size_t end = vecConnSize;
	int id = -1;
	size_t start = 0;
	value = 0;
	while (end >= start) {
		value = (start + end) >> 1;
		id = Pub_nextConnIndex[value];
		if (id < senderId) {
			start = value + 1;
		}
		else if (id > senderId) {
			end = value - 1;
		}
		else {
			return;
		}
	}
	value = -1;
}
vector<int> Pub_ConnNextId; // next �� id, �����С������ MaxRecverCount
vector<int> Pub_ConnLastId; // last �� id, �����С������ MaxRecverCount
map<int, int> idMap; // �� id ӳ��Ϊ idx
static size_t Pub_Jobs = 4;
vector<Nodes> Pub_nodes;

/* �� STransfer �ṹ��ת����ڽ�ʸ�� */
void preprocess() {
	sort(vecLines.begin(), vecLines.end(), sortTransferBySender);
	//connectivities.
	int lastSender = -1;
	//Pub_vecConn.reserve(vecLines.size()); // ��������û�л�·
	Pub_ConnNextId.reserve(vecLines.size() * MaxRecverCount);
	Pub_vecConn.resize(vecLines.size()); // ��������û�л�·
	// orderred vecLines Pub_vecConn sender Ҳ�������
	int iter = 0;
	SConnectivity *con;
	size_t lineCount = 0;
	for (auto line : vecLines) {
		Pub_ConnNextId.push_back(line.t2);
		if (lastSender != line.t1) {
			lastSender = line.t1;
			con = &Pub_vecConn[iter];
			con->id = iter;
			con->sender = lastSender;
			idMap[con->sender] = con->id;
			con->recverOffset = lineCount;
			lineCount += 1;
			con->recverCount += 1;
			iter += 1;
		}
		else {
			con->recverCount += 1;
			lineCount += 1;
		}
	}
	Pub_vecConn.resize(iter);
	Pub_vecConn.shrink_to_fit(); // �ͷŶ�����ڴ�

	// use cache index
	vecConnSize = Pub_vecConn.size();
	Pub_nextConnIndex.resize(vecConnSize);
	for (int i = 0; i < vecConnSize; i += 1) {
		Pub_nextConnIndex[i] = Pub_vecConn[i].sender;
	}

#if defined(MY_INFO) || defined(FULL_DATA_INFO)
	std::cout << "Connectivities: " << vecConnSize << " cache " << Pub_nextConnIndex.size() << endl;
#endif
} // end of preprocess

void preprocessPub() {
	// ���� vecConnSize ���� Pub_Jobs
	if (vecConnSize < 1000) {
		Pub_Jobs = 2;
	}
	else if (vecConnSize > 10000) {
		Pub_Jobs = 8;
	}

	Pub_nodes.resize(Pub_Jobs);
	size_t partialConnSize = vecConnSize / Pub_Jobs + 1; // vecConnSize ���� > Pub_Jobs
	size_t i;
	for (i = 0; i < Pub_Jobs; i += 1) {
		Pub_nodes[i].id = i;
		Pub_nodes[i].conn_offset = partialConnSize * i;
		Pub_nodes[i].conn_end = partialConnSize * (i + 1);
	}
	// last nodes modify
	i -= 1;
	if (Pub_nodes[i].conn_end > vecConnSize) {
		Pub_nodes[i].conn_end = vecConnSize;
	}
	// ------------ end nodes ------------------
	size_t totalRecvCap = Pub_ConnNextId.size();
	size_t it = 0;
	int ridx = 0;
	size_t it_conn = 0;
	size_t recv_count = 0;
	SConnectivity *conn = &Pub_vecConn[it_conn];
	SConnectivity *tmpConn;
	while (it < totalRecvCap) {
		findRidxBySid(Pub_ConnNextId[it], ridx);
		if (ridx > -1) {
			conn->next_1.push_back(&Pub_vecConn[ridx]);
		}
		else {
			conn->next_1.push_back(nullptr);
		}
		conn->nextId_1.push_back(Pub_ConnNextId[it]);

		recv_count += 1;
		if (recv_count == conn->recverCount) {
			it_conn += 1;
			if(it_conn < vecConnSize) 
				conn = &Pub_vecConn[it_conn];
			recv_count = 0;
		}
		it += 1;
	}
}

struct WalkCase{
	size_t idx; // �� Pub_vecConn �е��������
};

/* ���ڴ�����ܹ��ɻ��Ķ���� Connectivitiy ����
*/
size_t iter = 0;
class CircleChecker {
public:
	/* ��⻷������������䱣�浽������
	*/
	void checkCircle(Nodes &node) {
		size_t offset = node.conn_offset; size_t end = node.conn_end;
		while (offset < end) {
#if defined(LARGE_DATA) && defined(FULL_DATA_DEBUG)
			std::cout << "[CC]" << offset << "\t";
#endif
			// stack recursively max 7 times ջ�׵������
			push(&Pub_vecConn[offset]);
			int recverId;
			int recverTry; // recverTry ���� recver ��conn�����ڵ� offset
			SConnectivity *tmpConn;
			// ���� DFS ��������
			while (_vecSize != 0) { // ջ�ǿ�
				
				if (isTopLastRecver()) {
					// ��� connectivity �Ѿ���������ĩβ���Ƴ�
					pop();
					continue;
				}
				
				// ��ȡջ����Ԫ�� idx��Ȼ�����idx��ȡ SConnectivity ���������
				tmpConn = &Pub_vecConn[_stack[_stackTop]];

				recverId = tmpConn->nextId_1[_recvIdxTryCount[_stackTop]];
				tmpConn = tmpConn->next_1[_recvIdxTryCount[_stackTop]];
				_recvIdxTryCount[_stackTop] += 1;
				if (_vecSize < 2) {
					if (tmpConn != nullptr && tmpConn->sender > _vertex[0]) {
						push(tmpConn);
					}
				}
				else {
					CircleCase ccase = checkVertex(recverId);
					if (ccase == CirclePossible) {
						if (_vecSize < MaxCircleSize) { // ���ﲻ�ܰ��� MaxCircleSize����������� 8
							if (tmpConn != nullptr && tmpConn->sender > _vertex[0]) {
								push(tmpConn);
							}
						}
						else {
							pop(); // ��ջ�Ĵ�С���� 7����ô���Ĵ�С�ض����� 7������Ҫ��������
						}
					}
				}
			} // end of inner while

			offset += 1;

#if defined(LARGE_DATA) && !defined(FULL_DATA)
			iter += 1;
			if (iter >= LARGE_DATA) { break; }
#endif
		} // end of while 
	} // end of checkCircle

// ------------- stack -----------------
//public:
	inline bool isTopLastRecver() { return _recvIdxTryCount[_stackTop] >= _recvMaxIdx[_stackTop]; }
	inline void push(SConnectivity *c) {
		_stackTop += 1;
		_stack[_stackTop] = c->id;
		_recvMaxIdx[_stackTop] = c->recverCount;
		_vecSize += 1;
		_vertex.push_back(c->sender);
	}

	inline void pop() {
		_recvIdxTryCount[_stackTop] = 0; // ���ñ�־λ
		_stackTop -= 1;
		_vecSize -= 1;
		_vertex.pop_back();
	}
#if 0
	// not used
	inline void replaceTop(SConnectivity *c) {
		_recvIdxTryCount[_stackTop] = 0; // ���ñ�־λ
		_vertex[_stackTop] = c->sender;
	}
#endif
	inline bool empty() const { return _vecSize == 0; }
//public:
	CircleChecker() {
		// preserve stack size
		_stack.resize(MaxCircleSize + 3);
		_recvIdxTryCount.resize(MaxCircleSize + 3);
		_recvMaxIdx.resize(MaxCircleSize + 3);
	}
	~CircleChecker() {
		for (auto c : circles) {
			delete c;
		}
		circles.clear();
		_stack.clear();
		_recvIdxTryCount.clear();
		_recvMaxIdx.clear();
	}
//private:
	vector<int> _stack; // �洢 Connectivity �� idx �������е����
	// ��ʾ��ջ�� Connectivity ��ǰ���ҵ� recver �����
	vector<int> _recvIdxTryCount;
	vector<int> _recvMaxIdx;
	/* Top ������ */
	size_t _vecSize = 0;
	/* stackTop ָ��ջ����Ԫ�� */
	int _stackTop = -1;
	// ------------- stack -----------------
//public:
	inline void popVertex() { _vertex.pop_back(); }
	enum CircleCase {
		/*�޷����ɻ�*/
		CircleWrong,
		/*���ܹ��ɻ�*/
		CirclePossible,
		/*������һ������󣬾Ϳ��Թ��ɻ�*/
		CircleExits
	};
	/* ���� */
	/** @return true  �������ܹ��ɻ�
	* false �����޷����ɻ�
	* ����Ƿ��л�·�����洢��· 
	*/
	CircleCase checkVertex(int test) {
#if 0
		if (_vecSize > 2 && _vertex[0] == test) {
			error
				circles[_vecSize - MinCircleSize]->addCircle(_vertex.begin(), _vertex.begin() + _vecSize);
			return CircleExits;
		}
#else
		for (int i = 0; i < _vecSize - 2; i += 1) {
			if (_vertex[i] == test) {
				// ���� start������ end
				if (i == 0) {
					int idx = _vecSize - i - MinCircleSize;
					circles[idx]->addCircle(_vertex.begin() + i, _vertex.begin() + _vecSize);
				}
				return CircleExits;
			}
		}
#endif
		for (int i = _vecSize - 2; i < _vecSize; i += 1) {
			if (_vertex[i] == test) {
				return CircleWrong;
			}
		}
		return CirclePossible;
	}

	vector<int> _vertex;
// --------------- start of circles -----------------------

	vector<CircleInterface *> circles;
	void initCircles() {
#ifdef MY_INFO
		cout << "Init Circles"<< endl;
#endif
		circles.resize(CircleSizeCount);
		circles[0] = new _Circle<3>();
		circles[1] = new _Circle<4>();
		circles[2] = new _Circle<5>();
		circles[3] = new _Circle<6>();
		circles[4] = new _Circle<7>();
	}
	size_t sumCircles() {
		size_t count = 0;
		for (auto c : circles) {
			count += c->size();
		}
		return count;
	}
	void outputCircles(size_t i) {
#if defined(USE_TESTER) && defined(MY_INFO)
		circles[i]->output(std::cout);
#endif
	}
	/* ��ӡ������� */
	void outputAllCircles() {
#if defined(USE_TESTER) and defined(MY_INFO)
		for (auto c : circles) {
			c->output(std::cout);
		}
#endif
		return; // disable file output
#ifndef MY_DEBUG
		ofstream output("result.txt", std::ios::out);
		/*output << count << endl;*/
		for (auto c : circles) {
			c->output(output);
		}
		output.close();
#endif
	}
// end of circles
}; // end of class CircleChecker

#ifdef USE_TESTER
#include <chrono> 
#include <ctime>
#include <iomanip>
using namespace std::chrono;
typedef void(*TestFunc)(void);

static void TimeIt(TestFunc tf, int exe = 10) {
	double passed = 0;
	auto now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIS] " << std::put_time(std::localtime(&now), "%F %T") << endl;
	auto start = high_resolution_clock::now();
	tf();
	auto stop = high_resolution_clock::now();
	now = system_clock::to_time_t(system_clock::now());
	std::cout << "[TIE] " << std::put_time(std::localtime(&now), "%F %T") << endl;
#ifdef LARGE_DATA
	auto duration = duration_cast<seconds>(stop - start);
	passed += duration.count();
#else
	auto duration = duration_cast<milliseconds>(stop - start);
	passed += duration.count() / 10e3;
#endif
	std::cout << "Elapsed: " << passed << "s " << endl << endl;
}


CircleChecker cc[ThreadNum];

void *doJobs(void *p) {
	size_t wid = (size_t)(p); // worker id
	size_t i = (Pub_Jobs / ThreadNum) * wid;
	size_t last = (Pub_Jobs / ThreadNum) * (wid + 1);
	cc[wid].initCircles();
	while (i < last) {
		cc[wid].checkCircle(Pub_nodes[i]);
		i += 1;
	}
	return NULL;
}


#ifdef USE_TESTER
static void test_readfile() {
#else
void main(int argc, char *argv[]) {
#endif
	// start of main
	readfile();
	preprocess();
	preprocessPub();
	vecLines.clear();
	vecLines.shrink_to_fit();

#if ThreadNum > 1
	pthread_t pId[ThreadNum];
	size_t i, ret;
	//�������̣߳��߳�idΪpId
	for (size_t i = 1; i < ThreadNum; i += 1) {
		ret = pthread_create(&pId[i], NULL, doJobs, (void *)i);
		if (ret != 0)
		{
			printf("create init pthread error!\n");
			exit(1);
		}
	}
	doJobs((void *)0);
	for (size_t i = 1; i < ThreadNum; i += 1) {
		pthread_join(pId[i], NULL);
	}
#else 
	for (size_t i = 0; i < ThreadNum; i += 1) {
		doJobs((void *)i);
	}
#endif
	size_t count = 0;
	for (size_t i = 0; i < ThreadNum; i += 1) {
		count += cc[i].sumCircles();
	}
#if defined(USE_TESTER) and defined(MY_INFO)
	std::cout << "Total circles " << count << endl;
#endif
	for (size_t index = 0; index < CircleSizeCount; index += 1) {
		for (size_t i = 0; i < ThreadNum; i += 1) {
			cc[i].outputCircles(index);
		}
	}

	// release memory
	Pub_vecConn.clear();
	Pub_ConnNextId.clear();
	Pub_nodes.clear();
}

#endif // end USE_TESTER


#ifdef USE_TESTER
void main(int argc, char *argv[])
{
	cout << "Test " << sizeof(SConnectivity) << " cc "  << sizeof(CircleChecker) << endl;
	TimeIt(test_readfile, 1);
}
#endif