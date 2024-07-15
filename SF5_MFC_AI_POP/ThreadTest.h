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
    static std::string strV0;
    static std::string strV1;
    static std::string strC1;
    static vector<string> strHeat;
    static std::mutex critSect;

    static string vectorToString(vector<double> vec);

    static void Thread_DB_Get_Cur();
    static void Thread_DB_Get_Vib();
    static void Thread_DB_Get_Plastic(MySQL_Connector* mysql);
    static void Thread_DB_Get_Heat();

    static int Thread_DB_Wait_Robot();
    static int Thread_DB_Wait_Plastic();
    static int Thread_DB_Wait_Heat();
};
