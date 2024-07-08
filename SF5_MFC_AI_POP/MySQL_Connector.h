#pragma once

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <string>

using namespace std;

class MySQL_Connector
{
public:
    MySQL_Connector() : driver(nullptr), con(nullptr), pstmt(nullptr), result(nullptr) {}

    ~MySQL_Connector() {
        if (result) { delete result; result = nullptr; }
        if (pstmt) { delete pstmt; pstmt = nullptr; }
        if (con) { delete con; con = nullptr; }
    }

    bool connect(const string& server, const string& username, const string& password);
    bool login(const string& id, const string& pw);

private:
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;
};

