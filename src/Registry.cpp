#include "..\include\Registry.h"
//#include "Registry.h"
//#include "sha256.h"


// for existing customer
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

    // check if login exist and take customer id
    query << "SELECT regitstry.customer_id, regitstry.login FROM regitstry WHERE login='" << login << "'";
    if (!dbconnection.fetchRowsWithLambda(query.str().c_str(), lambda)) {
        return 0;
    }

    if (!checkPassword(customerId, password)) {
        return 0;
    }
    return customerId;
}

// for existing customer
bool Registry::checkPassword(unsigned customerId, const std::string& password) const {
    SHA256 sha256;
    std::string passHash = sha256(password.c_str());

    std::stringstream query;
    query << "SELECT password FROM regitstry WHERE customer_id=" << customerId;
    auto validPassword = dbconnection.getOneField(query.str().c_str());
    system("cls");
    if (*validPassword == passHash) {
        std::cout << "You are logged in!\n";
        std::cout << "Id klienta:" << customerId << '\n';
        return true;
    }
    else {
        std::cout << "Incorrect login or password\n";
        return false;
    }
}

std::shared_ptr<std::string> Registry::getLogin(unsigned id) const {
    std::stringstream query;
    query << "SELECT login FROM regitstry WHERE customer_id=" << id;
    auto customerLogin = dbconnection.getOneField(query.str().c_str());
    return customerLogin;
}



// for new customer
bool Registry::validateLogin(const std::string& login) {

    if (login.size() < 5 || login.size() > 20) {
        std::cout << "Login must contain 5-20 characters\n";
        return false;
    }
    std::stringstream query;
    query << "SELECT regitstry.customer_id FROM regitstry WHERE regitstry.login='" << login << "'";
    auto isLoginAvailable = dbconnection.getOneField(query.str().c_str());
    if (isLoginAvailable->size() == 0) {
         return true;
    }
    return false;
}

// for new customer
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

bool Registry::addNewUser(const unsigned& id, const std::string& login, const std::string& password, bool premium) {
    std::stringstream query;
    query << "INSERT INTO regitstry VALUES (" << id << ", '" << login << "', '" << password << "', " << premium << ")";

    return dbconnection.isQueryCorrect(query.str().c_str());
}

