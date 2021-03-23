//#include "Library.h"
#include "..\include\DBConnection.h"
#include "..\include\Library.h"
#include "..\include\DataValidation.h"

Library::Library(DBConnection& dbc) : dbconnection(dbc), registry(dbc) {
    userLogged = loginOrSignIn();
}

bool Library::loginOrSignIn() {
    std::cout << "1. Log in\n";
    std::cout << "2. Sign in\n";
    char choice;
    checkInput(choice, '1', '2');
    if (choice == '1') {
       return  login();
    }
    return signIn();
}


bool Library::login() {

    std::string login = "sebanek";
    std::string password = "Qwerty!1";
    //login = *dbconnection.validateStringInput("Login");
    //password = *dbconnection.validateStringInput("Password");
    //std::string login = "nowylogin";
    //std::string password = "Nowehaslo!1";

    customerID = registry.checkLoginAndPassword(login, password);
    if (customerID == 0) {
        return false;
    }
    if (customerID == 1 || customerID == 2) {
        admin = true; // maybe new table for admins, seems not safe
    }
    return true;
}

bool Library::signIn() {
    std::string login = "sebanek3";
    std::string password = "Nowehaslo!1";
    std::cout << "Login: ";
    std::cin >> login;
    if (registry.validateLogin(login)) {
        std::cout << "Password: ";
        std::cin >> password;
        while (!registry.validatePassword(password)) {
            std::cout << "Password: ";
            std::cin >> password;
        }
        //registry.signIn(login, password);
        std::stringstream query;
        query << "INSERT INTO ";
        addToTable(table_customers, query);
        return saveNewUserToRegistry(login, password);
    }
    else {
        signIn();
    }
}

bool Library::saveNewUserToRegistry(const std::string& login, const std::string& password) {
    std::stringstream query;
    query << "SELECT Customer_ID FROM customers WHERE Customer_ID = (SELECT MAX(Customer_ID) FROM customers)";
    auto id = dbconnection.getOneField(query.str().c_str());
    customerID= stoi(*id);
    SHA256 sha256;
    std::string hahsPass = sha256(password);
    return registry.addNewUser(customerID, login, hahsPass, true);
}

void Library::welcomeUser(unsigned id) const {
    std::cout << "Logged as: " << *registry.getLogin((id)) << ", ";
    if (admin) {
        std::cout << "Administrator\n\n";
    }
    else {
        std::cout << "Customer\n\n";
    }
}

char Library::showMenu() {
    while (!userLogged) {
        std::cout << "Log in or sign in!\n";
        userLogged = loginOrSignIn();
    }
    if (admin) {
        return mainAdminMenuChoice();
    }
    else {
        return mainUserMenuChoice();
    }
}

void Library::showAdminMenu() const {

    std::cout << "\n\n1. Show books\n";
    std::cout << "2. Show customers\n";
    std::cout << "3. Show orders\n";
    std::cout << "4. Find \n";
    std::cout << "5. Add \n";
    std::cout << "6. Bestsellers \n";
    std::cout << "7. Incomes \n";
    std::cout << "0. Logout\n";
    welcomeUser(customerID);
}

void Library::showCustomerMenu() const {

    std::cout << "\n\n1. Show all books\n";
    std::cout << "2. Show bestsellers \n";
    std::cout << "3. Order book \n";
    std::cout << "4. Show your books\n";
    std::cout << "5. Find book\n";
    std::cout << "6. Find order\n";
    std::cout << "0. Logut\n";
    welcomeUser(customerID);
}

char Library::mainUserMenuChoice(){
    std::stringstream query;
    query << "SELECT * FROM ";
    showCustomerMenu();
    char choice{};
    checkInput(choice, '0', '6');
    switch(choice)
    {
        case '1': showTable(table_books);
            break;
        case '2': showBestSellingBooks();
            break;
        case '3':
            break;
        case '4': showCustomerBooks();
            break;
        case '5':  query << table_books;
                   findBook(query);
            break;
        case '6': query << table_orders;
                  findOrder(query);
            break;
        case '0': return '0';
    }

    return 'a';
}


char Library::mainAdminMenuChoice() {
    showAdminMenu();
    char choice{};
    checkInput(choice, '0', '7');
    switch(choice)
    {
        case '1': bookMenu();
            break;
        case '2': customerMenu();
            break;
        case '3': orderMenu();
            break;
        case '4': findInDatabase();
            break;
        case '5': addToDatabase();
            break;
        case '6': showBestSellingBooks();
            break;
        case '7': showIncomes();
            break;
        case '0': return '0';
    }
    return 'a';
}

void Library::showTable(const std::string& table) const {
    welcomeUser(customerID);
    std::stringstream query;
    query << "SELECT * FROM " << table.c_str();

    sendQueryShowResult(query);
}

void Library::customerMenu() {
    showTable(table_customers);
    std::cout << "1. Show customer's books\n";
    std::cout << "2. Edit customer's data\n";
    std::cout << "3. Remove customer\n";
    std::cout << "0. Main menu\n";

    char choice{};
    checkInput(choice, '0', '3');
    switch(choice)
    {
        case '1': showCustomerBooks(); break;
        case '2': editCustomer(); break;
        case '3': removeRecord(table_customers); break;
        case '0': return;
    }
 }

void Library::bookMenu(){
    showTable(table_books);
    std::cout << "1. Edit book\n";
    std::cout << "2. Remove book\n";
    std::cout << "0. Main menu\n";

    char choice{};
    checkInput(choice, '0', '2');
    switch(choice)
    {
        case '1': editBook(); break;
        case '2': removeRecord(table_books); break;
        case '0': return;
    }
 }

bool Library::editBook() {
    std::cout << "\n";
    return true;
}

bool Library::removeRecord(const std::string& table) {
    const char tableType = verifyTable(table);
    std::string columnIdAsText;
    switch(tableType)
    {
        case '1': columnIdAsText = columnsBooks[COLUMN_ID]; break;
        case '2': columnIdAsText = columnsCustomers[COLUMN_ID]; break;
        case '3': columnIdAsText = columnsOrders[COLUMN_ID]; break;
    }

    int maxIdformTable = getMaxId(table);
    std::cout << "Type id to remove: ";
    unsigned idToDelete{};

    checkInput(idToDelete, 1, maxIdformTable, "Wrong id! Try again\n");


    if (tableType == '2' && (idToDelete == 1 || idToDelete == 2)) {
        std::cout << "You can not remove Administrator account!\n";
        return false;
    }

    std::stringstream query;
    query << "DELETE FROM " << table << " WHERE " << columnIdAsText << "=" << idToDelete;

    if (dbconnection.isQueryCorrect(query.str().c_str())) {
        std::cout << "Record id: " << idToDelete << " removed from table " << table << '\n';
        return true;
    }
    else {
        return false;
    }
}

int Library::getMaxId(const std::string& table) const {

    char tableType = verifyTable(table);
    std::string columnIdAsText;  // powtorka!
    switch(tableType)
    {
        case '1': columnIdAsText = columnsBooks[COLUMN_ID]; break;
        case '2': columnIdAsText = columnsCustomers[COLUMN_ID]; break;
        case '3': columnIdAsText = columnsOrders[COLUMN_ID]; break;
    } // powtorka

    std::stringstream query;
    query << "SELECT " << table << "." << columnIdAsText << " FROM " << table << " WHERE " << columnIdAsText
          << "=( SELECT MAX(" << columnIdAsText << ") FROM " << table << ")";
    auto res = dbconnection.getOneField(query.str().c_str());
    return stoi(*res);
}

void Library::orderMenu(){
    showTable(table_orders);
    std::cout << "1. Change status\n";
    std::cout << "0. Main menu\n";

    char choice{};
    checkInput(choice, '0', '1');
    switch(choice)
    {
        case '1': changeOrderStatus(); break;
        case '0': return;
    }
}

void Library::changeOrderStatus() {
    unsigned orderId;
    std::cout << "Order ID: ";
    std::cin >> orderId;
    std::cout << "1. oczekiwanie\n";
    std::cout << "2. wyslano\n";
    std::cout << "0. Main menu\n";

    char choice{};
    checkInput(choice, '0', '2');
    std::stringstream query;
    query << "UPDATE orders SET status='";
    switch(choice)
    {
        case '1': query << "oczekiwanie"; break;
        case '2': query << "wyslano"; break;
        case '0': return;
    }
    query << "' WHERE order_id=" << orderId;
    if (dbconnection.isQueryCorrect(query.str().c_str())) {
        std::cout << "Status changed\n";
    }
}

const auto Library::showColumnsTakeInput(const char* tableName[], const unsigned& tableSize) const {
    std::cin.clear();
    std::cin.ignore();
    auto tempTable = new std::string[tableSize];

    for (unsigned i = 1; i < tableSize; i++) {
    std::cout << tableName[i] << ": ";
    std::string input;
    std::getline(std::cin, input);
    tempTable[i-1] = input;
    }
    return tempTable;
}

unsigned Library::showCustomerById() const {
    unsigned customerID;
    std::cout << "Type customer's ID";
    std::cin >> customerID;

    std::stringstream query;
    query << "SELECT name, surname, town FROM customers WHERE customer_id =" << customerID;

    sendQueryShowResult(query);
    return customerID;
}

void Library::editCustomer() {
    std::cout << "Edit:\n";
    unsigned customerID = showCustomerById();
    std::string* userInput = showColumnsTakeInput(columnsCustomers, COLUMNS_CUSTOMERS);

    std::stringstream newQuery;
    newQuery << "UPDATE " << table_customers << " SET" <<
                " name='" << userInput[0] << "'," <<
                " surname='" << userInput[1] << "'," <<
                " town='" << userInput[2] << "'" <<
                " WHERE customer_id=" << customerID;

    delete[] userInput;
    dbconnection.isQueryCorrect(newQuery.str().c_str());
}

 void Library::sendQueryShowResult(std::stringstream& query) const {
     //system("cls");
     unsigned long long numOfColumns = dbconnection.getNumOfColumns(query.str().c_str());
     auto lambda = [&](MYSQL_ROW row){
        for (unsigned i = 0; i < numOfColumns; i++) {
            std::cout << row[i] << " | ";
        }
        std::cout << '\n';
    };
    if(!dbconnection.fetchRowsWithLambda(query.str().c_str(), lambda)) {
        std::cout << "Problems with database\n";
    }
 }


void Library::findInDatabase() const{
    system("cls");
    std::cout << "1. Find book\n";
    std::cout << "2. Find customer\n";
    std::cout << "3. Find order\n";
    std::cout << "0. Main menu\n";

    std::stringstream query;
    query << "SELECT * FROM ";

    char choice{};
    checkInput(choice, '0', '3');
    if(choice == '0') return;

    std::cout << "Find by\n";
    switch(choice)
    {
    case '1':
        query << table_books;
        findBook(query);
    break;
    case '2':
        query << table_customers;
        findCustomer(query);
    break;
    case '3':
        query << table_orders;
        findOrder(query);
    break;
    }


}

void Library::findBook(std::stringstream& query) const {

    query << " WHERE ";
    std::cout << "1. Id\n";
    std::cout << "2. Author\n";
    std::cout << "3. Title\n";

    char choice{};
    checkInput(choice, '1', '3');
    std::string value;
    std::cout << ": ";
    std::cin >> value;
    switch(choice)
    {
    case '1': query << "Book_ID=" << value.c_str();
        break;
    case '2': query << "Author_surname='" << value.c_str() << "'";
        break;
    case '3': query << "Title='%" << value.c_str() << "%'";
        break;
    }
    sendQueryShowResult(query);
}

void Library::findCustomer(std::stringstream& query) const {
    query << " WHERE ";
    std::cout << "1. Id\n";
    std::cout << "2. Surname\n";
    std::cout << "3. Town\n";

    char choice{};
    checkInput(choice, '1', '3');
    std::string value;
    std::cout << ": ";
    std::cin >> value;

    switch(choice)
    {
    case '1': query << "Customer_ID=" << value.c_str();
        break;
    case '2': query << "Surname LIKE '%" << value.c_str() << "%'";
        break;
    case '3': query << "Town LIKE '%" << value.c_str() << "%'";
        break;
    }
    sendQueryShowResult(query);
}

void Library::findOrder(std::stringstream& query) const {
    query << " WHERE ";
    char choice{};

    std::cout << "1. Order Id\n";
    if (admin) {
        std::cout << "2. Client Id\n";
        std::cout << "3. Book Id\n";
        std::cout << "4. Date\n";
        std::cout << "5. Status\n";
        checkInput(choice, '1', '5');
    }
    else {
        choice = '1';
    }
    std::string value;
    std::cout << ": ";
    std::cin >> value;

    switch(choice)
    {
    case '1': query << "Order_ID=" << value.c_str();
        break;
    case '2': query << "Customer_ID=" << value.c_str();
        break;
    case '3': query << "Book_ID=" << value.c_str();
        break;
    case '4': query << "Date='" << value.c_str() << "'";
        break;
    case '5': query << "Status='" << value.c_str() << "'";
        break;
    }
    sendQueryShowResult(query);
}

void Library::addToDatabase() const{
    std::cout << "1. Add book\n";
    std::cout << "2. Add customer\n";
    std::cout << "3. Add order\n";
    std::cout << "0. Main menu\n";

    std::stringstream query;
    query << "INSERT INTO ";

    char choice{};
    checkInput(choice, '0', '3');

    switch(choice)
    {
    case '1': addToTable(table_books, query);
        break;
    case '2':  addToTable(table_customers, query);
        break;
    case '3':  addToTable(table_orders, query);
        break;
    case '0': return;
    }
}

const char Library::verifyTable(const std::string& table) const {
    if (table == table_books) return '1';
    else if (table == table_customers) return '2';
    else return '3';
}

void Library::addToTable(const std::string& table, std::stringstream& query) const {

    const char option = verifyTable(table);

    query << table.c_str() << " ("; //zapis nazwy tablicy

    Functor takeInputFinishQuery(query);

    std::cin.ignore(10, '\n');
    switch(option)
    {
    case '1': takeInputFinishQuery(COLUMNS_BOOKS, columnsBooks);
        break;
    case '2': takeInputFinishQuery(COLUMNS_CUSTOMERS, columnsCustomers);
        break;
    case '3': takeInputFinishQuery(COLUMNS_ORDERS, columnsOrders);
        break;
    }

    if (dbconnection.isQueryCorrect(query.str().c_str())) {
        std::cout << "Record saved\n";
    }
}

void Library::showCustomerBooks() const {
    std::stringstream query;

    if (admin) {
        unsigned customerID;
        std::cout << "\n1. Type customer's ID\n";
        std::cin >> customerID;
    }

    query << "SELECT customers.Name, customers.Surname, books.Title, orders.Status"
             " FROM customers, orders, books "
             " WHERE customers.Customer_ID=" << customerID <<
             " && customers.Customer_ID=orders.Customer_ID"
             " && books.Book_ID=orders.Book_ID"
             " ORDER BY orders.Date";

    sendQueryShowResult(query);
}

void Library::showBestSellingBooks() const {
    std::stringstream query;
    query << "SELECT books.Title,"
             " COUNT(orders.Book_ID) AS Sold"
             " FROM books, orders"
             " WHERE books.Book_ID=orders.Book_ID"
             " GROUP BY books.Title"
             " ORDER BY Sold DESC"
             " LIMIT 3";

    sendQueryShowResult(query);
}

void Library::showIncomes() const {
    std::stringstream query;
    query << "SELECT ROUND(SUM(books.Price),2) AS Incomes"
             " FROM books, orders"
             " WHERE books.Book_ID=orders.Book_ID";

    sendQueryShowResult(query);
}
