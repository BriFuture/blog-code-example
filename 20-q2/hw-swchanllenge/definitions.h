#ifndef  DEFINITIONS_H
#define  DEFINITIONS_H

#define USE_TESTER

#ifdef _MSC_VER
#define ON_WIN
#endif

#define LARGE_DATA 
#define TEST_RUNLARGE
//#define TEST_LAYER2

#if defined(LARGE_DATA) && !defined(TEST_RUNLARGE)
#define MaxThreadNum 4
#else 
#define MaxThreadNum 1
#endif
#ifdef TEST_RUNLARGE
#define DefaultWorkerNum 1
#else
// WorkerNum 不能加快速度，但是可以均分任务
#define DefaultWorkerNum (MaxThreadNum * 1)
#endif

#if defined(ON_WIN) && defined(LARGE_DATA)
  #define NotOutputToFile
#endif

#define FULL_DATA_INFO
// #define FULL_DATA_DEBUG

#ifdef USE_TESTER
#if defined(LARGE_DATA)
#undef MY_INFO
//#define DataInputFile "test_data_2861665.txt" // 11s(26s delete)/1c 5s(20s delete)/4c
//#define DataInputFile "test_data_2896262.txt"  // best 14s828ms
#define DataInputFile "test_data_3512444.txt" // best 16s55/1c  6s2/4c, kp 2c 18s total
#define DataInputFile "test_data_1004812.txt" // best 7s4/1c 2s7/4c kp 8s/2c 13s/1c total
#else
//#define DataInputFile "test_data_77409.txt" // best 50ms(460ms wf)
#define DataInputFile "test_data_3738.txt"   // best 162ms (171ms wf) // 287ms on preserved path/1core
#define DataInputFile "test_data_56.txt" // best 31 ms
#define DataInputFile "test_data_38252.txt"  // best 99ms (306ms wf)
#define DataInputFile "test_data_1.txt"
#define DataInputFile "test_data_58284.txt" // best 485 ms(808ms wf)
#endif // end LARGE_DATA

#define DataOutputFile "result.txt"
#endif // end USE_TESTER


//#define USE_NODE_LAYER_2

#endif // DEFINITIONS_H