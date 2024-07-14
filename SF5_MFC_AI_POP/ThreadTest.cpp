#include "pch.h"
#include "ThreadTest.h"

string CThreadTest::strCur;
string CThreadTest::strVib;
string CThreadTest::strV0;
string CThreadTest::strV1;
string CThreadTest::strC1;
mutex CThreadTest::critSect;

int CThreadTest::offsetCur = 1;
int CThreadTest::offsetVib = 1;

string CThreadTest::vectorToString(vector<double> vec) {
    string output;
    int i = 0;

    for (double num : vec)
    {
        i++;

        if (i >= vec.size())
        {
            output += std::to_string(num);
        }
        else
        {
            output += std::to_string(num) + ", ";
        }
    }
    return output;
}

void CThreadTest::Thread_DB_Get_Cur()
{
    MySQL_Connector* mysql = new MySQL_Connector();

    if (mysql->connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
    {
        vector<double> cur = mysql->fetchDataFromTable(ROBOT_CUR, offsetCur++);

        lock_guard<mutex> lock(critSect);

        strCur = vectorToString(cur);
    }

    delete mysql;
}

void CThreadTest::Thread_DB_Get_Vib()
{
    MySQL_Connector* mysql = new MySQL_Connector();

    if (mysql->connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
    {
        vector<double> vib = mysql->fetchDataFromTable(ROBOT_VIB, offsetVib++);

        lock_guard<mutex> lock(critSect);

        strVib = vectorToString(vib);

    }

    delete mysql;
}

void CThreadTest::Thread_DB_Get_Plastic()
{
    MySQL_Connector* mysql = new MySQL_Connector();

    if (mysql->connect("tcp://127.0.0.1:3306", "Nia", "0000", "pop"))
    {
        vector<double> v0 = mysql->fetchDataFromTable(PLASTIC_V0, offsetCur++);
        vector<double> v1 = mysql->fetchDataFromTable(PLASTIC_V1, offsetCur++);
        vector<double> c1 = mysql->fetchDataFromTable(PLASTIC_C1, offsetCur++);

        lock_guard<mutex> lock(critSect);

        strV0 = vectorToString(v0);
        strV1 = vectorToString(v1);
        strC1 = vectorToString(c1);
    }

    delete mysql;
}


int CThreadTest::Thread_DB_Wait_Plastic() {
    thread t1(Thread_DB_Get_Plastic);

    t1.join();

    return 0;
}

int CThreadTest::Thread_DB_Wait_Robot()
{
    thread t1(Thread_DB_Get_Cur);
    thread t2(Thread_DB_Get_Vib);

    t1.join();
    t2.join();

    return 0;
}