#ifndef DBCONNECTION_H
#define DBCONNECTION_H
#include <iostream>
#include <Windows.h>
//#include "C:/Program Files (x86)/Dev-Cpp\MinGW64/x86_64-w64-mingw32/include/mysql.h"
#include <mysql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <functional>
#include <memory>

class DBConnection
{
    std::string dbHostName = "localhost";
    std::string dbUser = "root";
    std::string dbPassword = "";
    std::string dbName = "ksiegarnia";
    unsigned int dbPort = 3306;
    const char* unix_socket = nullptr;
    unsigned long dbClientFlag = 0;

    MYSQL *connection;
    MYSQL_ROW row;
    MYSQL_RES *query_result{};
    int qstate{};

public:

    explicit DBConnection(const std::string& host, const std::string&user,
                          const std::string& passwd, const std::string& name);
    ~DBConnection();

    bool isQueryCorrect(const char* query);
    bool isResultCorrect(const char* query);
    bool storeQeuryResult(const char* query);
    bool fetchRowsWithLambda(const char* query, const std::function<void(MYSQL_ROW)> lambda); // ! const;
    bool updateRecord(const char* query);
    unsigned long long getNumOfRows(const char* query);
    unsigned long long getNumOfColumns(const char* query);
    bool removeRecord(const char* query);
    std::shared_ptr<std::string>  getOneField(const char* query); // ! const;
    const std::shared_ptr<std::string>  validateStringInput(const char* name) const;
};

#endif // DBCONNECT_H
