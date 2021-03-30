#include <iostream>
#include "include\DBConnection.h"
#include "include\Bookshop.h"

int main()
{
    DBConnection dbconnection("localhost", "root", "", "bookshop");
    Bookshop bookshop(dbconnection);
    while(bookshop.showMenu() != '0');

    return 0;
}
