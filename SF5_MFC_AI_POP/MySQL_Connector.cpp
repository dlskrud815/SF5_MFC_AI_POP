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

void MySQL_Connector::truncateDB(string database) {
    pstmt = con->prepareStatement("TRUNCATE ?");
    pstmt->setString(1, database);
    pstmt->execute();
}

vector<double> MySQL_Connector::fetchDataFromTable(tNum process, int offset)
{
    switch (process) {

    case ROBOT_CUR:
        try {
            string col, data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'current' ORDER BY ORDINAL_POSITION LIMIT 1 OFFSET ?");
            pstmt->setString(1, to_string(offset));
            result = pstmt->executeQuery();

            if (result->next()) {
                col = result->getString(1).c_str();
            }
            pstmt = con->prepareStatement("select " + col + " from pop.current");
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
    case ROBOT_VIB:
        try {
            string col, data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'vibration' ORDER BY ORDINAL_POSITION LIMIT 1 OFFSET ?");
            pstmt->setString(1, to_string(offset));
            result = pstmt->executeQuery();

            if (result->next()) {
                col = result->getString(1).c_str();
            }
            pstmt = con->prepareStatement("select " + col + " from pop.vibration");
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
    case PLASTIC_V0:
        try {
            string data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT AI0_Vibration FROM pop.plastic LIMIT 20 OFFSET ?");
            pstmt->setString(1, to_string(offset - 1));
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
    case PLASTIC_V1:
        try {
            string data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT AI1_Vibration FROM pop.plastic LIMIT 20 OFFSET ?");
            pstmt->setString(1, to_string(offset - 1));
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
    case PLASTIC_C1:
        try {
            string data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT AI2_Current FROM pop.plastic LIMIT 20 OFFSET ?");
            pstmt->setString(1, to_string(offset - 1));
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
    case HEAT_NUM:
        try {
            string data;
            vector<double> data_set;

            data_set.clear();

            pstmt = con->prepareStatement("SELECT * FROM pop.heat LIMIT 1 OFFSET ?");
            pstmt->setString(1, to_string(offset - 1));
            result = pstmt->executeQuery();

            if (result->next()) {
                for (int i = 1; i < 21; ++i) {
                    data = result->getString(i).c_str();
                    data_set.push_back(stod(data));
                }
            }

            return data_set;
        }
        catch (sql::SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
        }
    }

}

void MySQL_Connector::sendErrorToDB(CString idx, CString time, CString status, CString facility, CString message)
{
    try {
        pstmt = con->prepareStatement("INSERT INTO pop_result.errorlist VALUE (?,?,?,?,?)");
        pstmt->setString(1, string(CT2CA(idx)));
        pstmt->setString(2, string(CT2CA(time)));
        pstmt->setString(3, string(CT2CA(status)));
        pstmt->setString(4, string(CT2CA(facility)));
        pstmt->setString(5, string(CT2CA(message)));
        pstmt->execute();
        if (facility == "소성가공") {
            stmt = con->createStatement();
            result = stmt->executeQuery("SELECT count(*) FROM pop_result.plastic");
            if (result->next()) idx = result->getString(1).c_str();
            int idx_plastic = stoi(string(CT2CA(idx)));
            pstmt = con->prepareStatement("INSERT INTO pop_result.plastic VALUE (?,?,?)");
            pstmt->setString(1, to_string(idx_plastic + 1));
            pstmt->setString(2, string(CT2CA(time)));
            pstmt->setString(3, string(CT2CA(message)));
            pstmt->execute();
        }
        else if (facility == "로봇 용접") {
            stmt = con->createStatement();
            result = stmt->executeQuery("SELECT count(*) FROM pop_result.robot");
            if (result->next()) idx = result->getString(1).c_str();
            int idx_robot = stoi(string(CT2CA(idx)));
            pstmt = con->prepareStatement("INSERT INTO pop_result.robot VALUE (?,?,?)");
            pstmt->setString(1, to_string(idx_robot + 1));
            pstmt->setString(2, string(CT2CA(time)));
            pstmt->setString(3, string(CT2CA(message)));
            pstmt->execute();
        }
        else if (facility == "열처리") {
            stmt = con->createStatement();
            result = stmt->executeQuery("SELECT count(*) FROM pop_result.heat");
            if (result->next()) idx = result->getString(1).c_str();
            int idx_heat = stoi(string(CT2CA(idx)));
            pstmt = con->prepareStatement("INSERT INTO pop_result.heat VALUE (?,?,?)");
            pstmt->setString(1, to_string(idx_heat + 1));
            pstmt->setString(2, string(CT2CA(time)));
            pstmt->setString(3, string(CT2CA(message)));
            pstmt->execute();
        }
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