#include <iostream>
//#include "DBConnection.h"
//#include "Library.h"
#include "..\include\DBConnection.h"
#include "..\include\Library.h"


int main()
{
    std::string host{"localhost"};
    std::string user{"root"};
    std::string passswd{""};
    std::string dbname{"library"};

    DBConnection dbconnection(host, user, passswd, dbname);
    Library library(dbconnection);
    while(library.showMenu() != '0') {
    };

    return 0;
}
