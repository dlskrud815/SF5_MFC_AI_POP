#pragma once

#include "MySQL_Connector.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

using namespace std;

class CThreadTest {
public:
    static std::string strCur;
    static std::string strVib;
    static std::mutex critSect;
    static int offsetCur;
    static int offsetVib;

    static void Thread_DB_Get_Cur();
    static void Thread_DB_Get_Vib();
    static int Thread_DB_Wait();
};
