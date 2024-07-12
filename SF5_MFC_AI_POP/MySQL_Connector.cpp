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

bool MySQL_Connector::getID()
{
    try {
        pstmt = con->prepareStatement("SELECT message FROM chat LIMIT 1, 1"); // 수정
        result = pstmt->executeQuery();

        return result->next();  // 결과가 있는지 여부를 반환
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

bool MySQL_Connector::getFrom()
{
    try {
        pstmt = con->prepareStatement("SELECT message FROM chat LIMIT 2, 1"); // 수정
        result = pstmt->executeQuery();

        return result->next();  // 결과가 있는지 여부를 반환
    }
    catch (sql::SQLException& e) {
        return false;
    }
}

vector<double> MySQL_Connector::fetchDataFromTable(string tableName, int offset)
{
    string col, data, column_name;
    vector<double> data_set;
    try {
        pstmt = con->prepareStatement("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '" + tableName + "' ORDER BY ORDINAL_POSITION LIMIT 1 OFFSET ?");
        pstmt->setString(1, to_string(offset));
        result = pstmt->executeQuery();

        if (result->next()) {
            col = result->getString(1).c_str();
        }
        pstmt = con->prepareStatement("select " + col + " from pop." + tableName);
        result = pstmt->executeQuery();

        while (result->next()) {
            data = result->getString(1).c_str();
            data_set.push_back(stod(data));
        }

        return data_set;
    }
    catch (sql::SQLException& e) {
        cerr << "SQLException: " << e.what() << endl;
    }
}

void MySQL_Connector::getTable(vector<double> dataset)
{
    string output;
    int i = 0;

    for (double num : dataset)
    {
        i++;

        if (i >= dataset.size())
        {
            output += to_string(num);
        }
        else
        {
            output += to_string(num) + ", ";
        }
    }

    this->data2 = output.c_str();
}

bool MySQL_Connector::newAdmin(string id, string pw)
{
    try {
        pstmt = con->prepareStatement("INSERT INTO user (id, pw) VALUES (?, ?)");
        pstmt->setString(1, id);
        pstmt->setString(2, pw);
        pstmt->executeUpdate();

        return true;
    }
    catch (sql::SQLException& e) {
        return false;
    }
}