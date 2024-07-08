#include "pch.h"
#include "MySQL_Connector.h"

bool MySQL_Connector::connect(const string& server, const string& username, const string& password)
{
    try {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema("chatting_project"); // 수정

        stmt = con->createStatement();
        stmt->execute("set names euckr");
        if (stmt) { delete stmt; stmt = nullptr; }

        return true;
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

bool MySQL_Connector::login(const string& id, const string& pw)
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