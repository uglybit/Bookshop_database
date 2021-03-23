#ifndef REGISTRY_H
#define REGISTRY_H
#include "DBConnection.h"
#include <sstream>
#include "sha256.h"

class Registry
{
    DBConnection& dbconnection;
    std::string table_registry = "regitstry";
    /*static constexpr*/ unsigned COLUMNS_REGISTRY = 4;
    const char* columnsRegistry[5]{"Customer_ID", "Login", "Password", "Premium"};
public:
    explicit Registry(DBConnection& dbc) : dbconnection(dbc) {}
    ~Registry() = default;

    unsigned checkLoginAndPassword(const std::string& login, const std::string& password) const;
    bool checkPassword(unsigned customerId, const std::string& password) const ;
    bool validateLogin(const std::string& login);
    bool validatePassword(const std::string& password);
    bool addNewUser(const unsigned& id, const std::string& login, const std::string& password, bool premium);
    std::shared_ptr<std::string> getLogin(unsigned id) const;

};

#endif // REGISTRY_H
