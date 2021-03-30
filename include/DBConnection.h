#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <iostream>
#include <Windows.h>
#include <mysql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <functional>
#include <memory>

class DBConnection
{
    std::string dbHostName;
    std::string dbUser;
    std::string dbPassword;
    std::string dbName;
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

    // sends query without storing result, reutrns true when succeed, otherwise returns false
    bool sendQuery(const char* query);
    // checks if query result is ok, reutrns true when succeed, otherwise returns false
    bool sendQueryStoreResult(const char* query);
    // shows result of query in the way defined in the second argument
    bool fetchRowsWithLambda(const char* query, const std::function<void(MYSQL_ROW)> lambda);
    // returns number of columns for the query
    unsigned long long getNumOfColumns(const char* query);
    // returns one field from record
    std::shared_ptr<std::string>  getOneField(const char* query);
    // preventing sql injection
    const std::shared_ptr<std::string>  validateStringInput(const char* name = "") const;
};

#endif // DBCONNECT_H
