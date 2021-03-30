#include "..\include\Registry.h"

Registry::Registry(DBConnection& dbc) : dbconnection(dbc) { }

Registry::~Registry() { }


// for existing user

unsigned  Registry::checkLoginAndPassword(const std::string& login, const std::string& password) const {
    std::stringstream query;
    unsigned customerId{};

     auto lambda = [&customerId, &login](MYSQL_ROW row){
            if (row[1] == login) {
                std::stringstream value;
                value << row[0];
                value >> customerId;
             }
    };

    // if login exist save customer id
    query << "SELECT access.customer_id, access.login FROM access WHERE login='" << login << "'";
    if (!dbconnection.fetchRowsWithLambda(query.str().c_str(), lambda)) {
        return 0;
    }

    if (!checkPassword(customerId, password)) {
        return 0;
    }
    return customerId;
}

bool Registry::checkPassword(unsigned customerId, const std::string& password) const {
    SHA256 sha256;
    std::string passHash = sha256(password.c_str());

    std::stringstream query;
    query << "SELECT password FROM access WHERE customer_id=" << customerId;
    auto validPassword = *dbconnection.getOneField(query.str().c_str());
    system("cls");
    if (validPassword == passHash) {
        std::cout << "You are logged in!\n";
        std::cout << "Id klienta:" << customerId << '\n';
        return true;
    }
    std::cout << "Incorrect login or password\n";
    return false;
}

std::shared_ptr<std::string> Registry::getLogin(unsigned id) const {
    std::stringstream query;
    query << "SELECT login FROM access WHERE customer_id=" << id;
    auto customerLogin = dbconnection.getOneField(query.str().c_str());
    return customerLogin;
}

bool Registry::validateLogin(const std::string& login) {
    if (login.size() < 5 || login.size() > 20) {
        std::cout << "Login must contain 5-20 characters\n";
        return false;
    }

    std::stringstream query;
    query << "SELECT access.customer_id FROM access WHERE access.login='" << login << "'";
    auto isLoginAvailable = dbconnection.getOneField(query.str().c_str());

    if (isLoginAvailable->size() == 0) {
         return true;
    }
    return false;
}

bool Registry::validatePassword(const std::string& password) {
    if (password.size() < 8 || password.size() > 25) {
        std::cout << "Password must contain 8-25 characters\n";
        return false;
    }
    if (password.find_first_of("!@#$%^&*()_\\/;'<>?") == std::string::npos) {
        std::cout << "Password must contain at least one special character: !@#$%^&*()_\\/;'<>?\n";
        return false;
    }
    if (password.find_first_of("0123456789") == std::string::npos) {
        std::cout << "Password must contain at least one digit\n";
        return false;
    }
    if (password.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos) {
        std::cout << "Password must contain at least one lowercase\n";
        return false;
    }
    if (password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos) {
        std::cout << "Password must contain at least one capital letter\n";
        return false;
    }
    return true;
}

bool Registry::addNewUser(const unsigned& id, const std::string& login, const std::string& password) {
    std::stringstream query;
    query << "INSERT INTO access VALUES (" << id << ", '" << login << "', '" << password << "')";

    return dbconnection.sendQuery(query.str().c_str());
}

