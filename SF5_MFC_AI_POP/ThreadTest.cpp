#include "pch.h"
#include "ThreadTest.h"

string CThreadTest::strCur;
string CThreadTest::strVib;

mutex CThreadTest::critSect;

int CThreadTest::offsetCur = 1;
int CThreadTest::offsetVib = 1;


void CThreadTest::Thread_DB_Get_Cur()
{
    MySQL_Connector* mysql = new MySQL_Connector();
    int i = 0;
    string output;

    if (mysql->connect("tcp://192.168.1.245:3306", "Nia", "0000", "pop"))
    {
        vector<double> cur = mysql->fetchDataFromTable("current", offsetCur++);

        lock_guard<mutex> lock(critSect);

        for (double num : cur)
        {
            i++;
            if (i >= cur.size())
            {
                output += std::to_string(num);
            }
            else
            {
                output += std::to_string(num) + ", ";
            }
        }

        strCur = output;
    }

    delete mysql;
}

void CThreadTest::Thread_DB_Get_Vib()
{
    MySQL_Connector* mysql = new MySQL_Connector();
    int i = 0;
    string output;

    if (mysql->connect("tcp://192.168.1.245:3306", "Nia", "0000", "pop"))
    {
        vector<double> vib = mysql->fetchDataFromTable("vibration", offsetVib++);

        lock_guard<mutex> lock(critSect);

        for (double num : vib)
        {
            i++;
            if (i >= vib.size())
            {
                output += std::to_string(num);
            }
            else
            {
                output += std::to_string(num) + ", ";
            }
        }

        strVib = output;
    }

    delete mysql;
}

int CThreadTest::Thread_DB_Wait()
{
    thread t1(Thread_DB_Get_Cur);
    thread t2(Thread_DB_Get_Vib);

    t1.join();
    t2.join();

    return 0;
}
