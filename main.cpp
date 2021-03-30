#include <iostream>
#include "include\DBConnection.h"
#include "include\Bookshop.h"

int main()
{
    std::string host{"localhost"};
    std::string user{"root"};
    std::string passswd{""};
    std::string dbname{"bookshop"};

    DBConnection dbconnection(host, user, passswd, dbname);
    Bookshop bookshop(dbconnection);
    while(bookshop.showMenu() != '0') {
    };

    return 0;
}
