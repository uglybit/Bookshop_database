#ifndef LIBRARY_H
#define LIBRARY_H
#include "DBConnection.h"
#include "Functor.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include "Registry.h"


class Library
{
    DBConnection& dbconnection;
    Registry registry;
    unsigned customerID{0};
    bool admin{false};
    bool userLogged{false};

    const unsigned COLUMN_ID = 0;
    std::string table_books = "books";
    /*static constexpr*/ unsigned COLUMNS_BOOKS = 5;
    const char* columnsBooks[5]{"Book_ID", "Author_name", "Author_surname", "Title", "Price"};

    std::string table_customers = "customers";

    /*static constexpr */unsigned COLUMNS_CUSTOMERS = 4;
    const char* columnsCustomers[4]{"Customer_ID", "Name", "Surname", "Town"};

    std::string table_orders = "orders";
    /*static constexpr*/ unsigned COLUMNS_ORDERS = 5;
    const char* columnsOrders[5]{"Order_ID", "Customer_ID", "Book_ID", "Date", "Status"};


public:

    explicit Library(DBConnection& dbc);
    ~Library() = default;

    bool loginOrSignIn();
    bool login();
    bool signIn();
    bool saveNewUserToRegistry(const std::string& login, const std::string& password);
    void welcomeUser(unsigned id) const;
    void showTable(const std::string& table) const;
    void showAdminMenu() const;
    void showCustomerMenu() const;
    char mainAdminMenuChoice();
    char mainUserMenuChoice();
    void findInDatabase() const;
    void findBook(std::stringstream& query) const;
    void findCustomer(std::stringstream& query) const;
    void findOrder(std::stringstream& query) const;
    void addToDatabase() const;
    void addToTable(const std::string& table, std::stringstream& query) const;
    const char verifyTable(const std::string& table) const;
    void updateRecord();
    void customerMenu();
    void bookMenu();
    bool editBook();
    bool removeRecord(const std::string& table);
    void orderMenu();
    void showCustomerBooks() const;
    void showBestSellingBooks() const;
    void showIncomes() const;
    void editCustomer();
    void sendQueryShowResult(std::stringstream& query) const;
    unsigned showCustomerById() const;
    const auto showColumnsTakeInput(const char* tableName[], const unsigned& tableSize) const;
    void changeOrderStatus();
    char showMenu();
    void showCustomerOrders() const;
    int getMaxId(const std::string& table) const;

};

#endif // LIBRARY_H
