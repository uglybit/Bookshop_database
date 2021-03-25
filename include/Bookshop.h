#ifndef LIBRARY_H
#define LIBRARY_H
#include "DBConnection.h"
#include "Functor.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include "Registry.h"
#include "Table.h"

class Bookshop
{
    DBConnection& dbconnection;
    Registry registry;
    unsigned customerID{0};
    bool admin{false};
    bool userLogged{false};

    Table* books;
    Table* customers;
    Table* orders;

public:
    explicit Bookshop(DBConnection& dbc);
    ~Bookshop();

    char showMenu();

private:
    // admin only
    void showAdminMenu() const;
    char adminMenuChoice();
    void addToDatabase() const;
    void findInDatabase() const;
    void findCustomer(std::stringstream& query) const;
    void bookMenu();
    bool editBook();
    void orderMenu();
    void changeOrderStatus();
    void showIncomes() const;

    //customer & admin
    bool signIn();
    bool saveNewUserToRegistry(const std::string& login, const std::string& password);
    bool loginOrSignIn();
    bool login();
    void welcomeUser(unsigned id) const;
    void customerMenu();
    void showCustomerMenu() const;
    char customerMenuChoice();
    void findBook(std::stringstream& query) const;
    void findOrder(std::stringstream& query) const;
    void showCustomerBooks() const;
    unsigned showCustomerById() const;
    void orderBook();

    // internal functions
    void showTable(Table* table) const;
    const TableType verifyTable(Table* table) const;
    void updateRecord();
    void showBestSellingBooks() const;
    void sendQueryShowResult(std::stringstream& query) const;
    const std::string* showColumnsTakeInput(Table* table) const;
    bool removeRecord(Table* table);
    unsigned getMaxId(Table* table) const;
    const std::string getColumnIdName(Table* table) const;
    void addToTable(Table* table, std::stringstream& query) const;
    void editRecord(Table* table);
    unsigned showRecordById(Table * table) const;
    void addColumsToQuery(Table* table, std::stringstream& query) const;

};

#endif // LIBRARY_H
