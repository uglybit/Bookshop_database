#ifndef LIBRARY_H
#define LIBRARY_H
#include "DBConnection.h"
#include "Functor.h"
#include <sstream>
#include <iomanip>
#include <memory>
#include "Registry.h"

class Bookshop
{
    DBConnection& dbconnection;
    Registry registry;
    unsigned customerID{0};
    bool admin{false};
    bool userLogged{false};

    const unsigned COLUMN_ID = 0;
    std::string table_books = "books";
    static constexpr unsigned COLUMNS_BOOKS_SIZE{5};
    const char* columnsBooks[COLUMNS_BOOKS_SIZE]{"Book_ID", "Author_name", "Author_surname", "Title", "Price"};

    std::string table_customers = "customers";

    static constexpr unsigned COLUMNS_CUSTOMERS_SIZE = 4;
    const char* columnsCustomers[COLUMNS_CUSTOMERS_SIZE]{"Customer_ID", "Name", "Surname", "Email"};

    std::string table_orders = "orders";
    static constexpr unsigned COLUMNS_ORDERS_SIZE = 5;
    const char* columnsOrders[COLUMNS_ORDERS_SIZE]{"Order_ID", "Customer_ID", "Book_ID", "Date", "Status"};

public:
    explicit Bookshop(DBConnection& dbc);
    ~Bookshop() = default;

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
    void editCustomer();
    unsigned showCustomerById() const;
    void orderBook();

    // internal functions
    void addToTable(const std::string& table, std::stringstream& query) const;
    void showTable(const std::string& table) const;
    const char verifyTable(const std::string& table) const;
    void updateRecord();
    void showBestSellingBooks() const;
    void sendQueryShowResult(std::stringstream& query) const;
    const auto showColumnsTakeInput(const char* tableName[], const unsigned& tableSize) const;
    bool removeRecord(const std::string& table);
    unsigned getMaxId(const std::string& table) const;
    const std::string getColumnIdName(const std::string& table) const;

};

#endif // LIBRARY_H
