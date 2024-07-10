#include "pch.h"
#include "MySQL_Connector.h"

bool MySQL_Connector::connect(string server, string username, string password, string database)
{
    try {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema(database);

        stmt = con->createStatement();
        stmt->execute("set names euckr");
        if (stmt) { delete stmt; stmt = nullptr; }

        return true;
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

bool MySQL_Connector::login(string id, string pw)
{
    try {
        pstmt = con->prepareStatement("SELECT * FROM user WHERE id = ? AND pw = ?"); // 수정
        pstmt->setString(1, id);
        pstmt->setString(2, pw);
        result = pstmt->executeQuery();

        return result->next();  // 결과가 있는지 여부를 반환
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

bool MySQL_Connector::getData(int i)
{
    try {
        pstmt = con->prepareStatement("SELECT message FROM chat LIMIT ?, 1"); // 수정
        pstmt->setInt(1, i);
        result = pstmt->executeQuery();

        return result->next();  // 결과가 있는지 여부를 반환
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

CString MySQL_Connector::getMessage()
{
    try {
        string message = result->getString("message"); //수정
        return CString(message.c_str());
    }
    catch (sql::SQLException& e) {
        return _T("");
    }
}

CString MySQL_Connector::getID()
{
    try {
        pstmt = con->prepareStatement("SELECT id FROM chat LIMIT 1"); // 수정
        result = pstmt->executeQuery();
        string message = result->getString("id");

        return CString(message.c_str());
    }
    catch (sql::SQLException& e) {
        return _T("");
    }
}

CString MySQL_Connector::getFrom()
{
    try {
        pstmt = con->prepareStatement("SELECT id_from FROM chat LIMIT 1"); // 수정
        result = pstmt->executeQuery();
        string message = result->getString("id_from");

        return CString(message.c_str());
    }
    catch (sql::SQLException& e) {
        return _T("");
    }
}