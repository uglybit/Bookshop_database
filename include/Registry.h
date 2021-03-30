#ifndef REGISTRY_H
#define REGISTRY_H

#include <sstream>
#include <memory>
#include "DBConnection.h"
#include "Table.h"
#include "sha256.h"

/*
    Registry stores sensitive data so it is separated
    from library users data, the relation of tables is
    only user's id number
*/

class Registry
{
    DBConnection& dbconnection;
    Access access{"access", {"Customer_ID", "Login", "Password"}};

public:
    explicit Registry(DBConnection& dbc);
    ~Registry();

    //for existing users
    // if login is in database checks if password match then return user id or zero
    unsigned checkLoginAndPassword(const std::string& login, const std::string& password) const;
    std::shared_ptr<std::string> getLogin(unsigned id) const;
    //for new users
    bool validateLogin(const std::string& login);
    bool validatePassword(const std::string& password);
    bool addNewUser(const unsigned& id, const std::string& login, const std::string& password);

private:
    //for existing users
    bool checkPassword(unsigned customerId, const std::string& password) const ;

};

#endif // REGISTRY_H
