//#include "DBConnection.h"
#include "..\include\DBConnection.h"

DBConnection::DBConnection(const std::string& host, const std::string& user,
                           const std::string& passwd, const std::string& name)
    : dbHostName(host), dbUser(user), dbPassword(passwd), dbName(name)
{
    connection = mysql_init(0);
    connection = mysql_real_connect(connection, dbHostName.c_str(),
                     dbUser.c_str(), dbPassword.c_str(),
                     dbName.c_str(), dbPort, unix_socket, dbClientFlag);
    if (!connection) {
        std::cout << "Database not connected!\n";
        exit(9);
    }
    else {
        std::cout << "Database connected\n";
    }
}

DBConnection::~DBConnection() {
    mysql_close(connection);
}

const std::shared_ptr<std::string> DBConnection::validateStringInput(const char* name) const{
    std::cout << name << ": ";
    auto userInput = std::make_shared<std::string>();
    std::cin >> *userInput;

    if (userInput->find_first_of("'\";") != std::string::npos) {
        std::cout << "You have inserted forbidden characters";
        validateStringInput(name);
    }
    return userInput;
}

bool DBConnection::sendQuery(const char* query) {
    qstate = mysql_query(connection, query);
    if (qstate == 0) {
        return true;
    }
    std::cout << "Wrong query!\n";
    return false;
}

bool DBConnection::sendQueryStoreResult(const char* query) {
    sendQuery(query);
    query_result = mysql_store_result(connection);
    if (query_result) {
        return true;
    }
    std::cout << "Problems with database!\n";
    return false;
}

unsigned long long DBConnection::getNumOfRows(const char* query) {
    if (sendQueryStoreResult(query)) {
        return mysql_num_rows(query_result);
    }
    return 0;
}

unsigned long long DBConnection::getNumOfColumns(const char* query) {
    if (sendQueryStoreResult(query)) {
        return mysql_num_fields(query_result);
    }
    return 0;
}

bool DBConnection::fetchRowsWithLambda(const char* query, const std::function<void(MYSQL_ROW)> lambda) {
    if (!sendQueryStoreResult(query)) {
        return false;
    }
    while((row = mysql_fetch_row(query_result))) {
        lambda(row);
    }
    std::cout << "\n\n";
    return true;
}

bool DBConnection::updateRecord(const char* query) {
    if (sendQuery(query)) {
        std::cout << "Record changed\n";
        return true;
    }
    return false;
}

bool DBConnection::removeRecord(const char* query) {
    if (sendQuery(query)) {
        std::cout << "Record removed\n";
        return true;
    }
    return false;
}

std::shared_ptr<std::string> DBConnection::getOneField(const char* query) {

    auto result = std::make_shared<std::string>();
    auto lambda = [&result](MYSQL_ROW row) { *result = row[0]; };

    fetchRowsWithLambda(query, lambda);
    return result;
}
