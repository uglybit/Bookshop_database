#ifndef REGISTRY_H
#define REGISTRY_H
#include "DBConnection.h"
#include "Table.h"
#include <sstream>
#include "sha256.h"
#include <memory>

class Registry
{
    DBConnection& dbconnection;
    Access access{"access", {"Customer_ID", "Login", "Password"}};

public:
    explicit Registry(DBConnection& dbc);
    ~Registry();

    std::shared_ptr<std::string> getLogin(unsigned id) const;

    //for existing users
    unsigned checkLoginAndPassword(const std::string& login, const std::string& password) const;
    bool checkPassword(unsigned customerId, const std::string& password) const ;

    //for new users
    bool validateLogin(const std::string& login);
    bool validatePassword(const std::string& password);
    bool addNewUser(const unsigned& id, const std::string& login, const std::string& password);
};

#endif // REGISTRY_H
