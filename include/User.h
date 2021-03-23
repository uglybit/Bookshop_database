#ifndef USER_H
#define USER_H
#include <iostream>

class User
{
public:
    User();
    virtual ~User();

};


class Customer : public User {
    unsigned customer_id;
    std::string customer_login;
    std::string customer_name;
    std::string customer_surname;
    
};

class Admin : public User {
    unsigned admin_id;
    std::string admin_login;
};


#endif // USER_H
