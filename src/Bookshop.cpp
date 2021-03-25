//#include "Bookshop.h"
#include "..\include\DBConnection.h"
#include "..\include\Bookshop.h"
#include "..\include\DataValidation.h"

constexpr unsigned Bookshop::COLUMNS_BOOKS_SIZE;
constexpr unsigned Bookshop::COLUMNS_CUSTOMERS_SIZE;
constexpr unsigned Bookshop::COLUMNS_ORDERS_SIZE;

Bookshop::Bookshop(DBConnection& dbc) : dbconnection(dbc), registry(dbc) {
    userLogged = loginOrSignIn();
}

bool Bookshop::loginOrSignIn() {
    std::cout << "1. Log in\n";
    std::cout << "2. Sign in\n";
    char choice;
    checkInput(choice, '1', '2');
    if (choice == '1') {
       return  login();
    }
    return signIn();
}

bool Bookshop::login() {
    std::string login{"shopenhauer"}; //{"mithrandir"};
    std::string password{"Qwerty!1"}; //{"Mnbvcx!1"};
    //login = *dbconnection.validateStringInput("Login");
    //password = *dbconnection.validateStringInput("Password");

    customerID = registry.checkLoginAndPassword(login, password);
    if (customerID == 0) {
        return false;
    }
    if (customerID == 1 || customerID == 2) {
        admin = true;
    }
    return true;
}

bool Bookshop::signIn() {
    std::string login;
    std::string password;
    std::cout << "Login: ";
    std::cin >> login;
    if (registry.validateLogin(login)) {
        std::cout << "Password: ";
        std::cin >> password;
        while (!registry.validatePassword(password)) {
            std::cout << "Password: ";
            std::cin >> password;
        }
        std::stringstream query;
        query << "INSERT INTO ";
        addToTable(table_customers, query);
        return saveNewUserToRegistry(login, password);
    }
    else {
        signIn();
    }
}

bool Bookshop::saveNewUserToRegistry(const std::string& login, const std::string& password) {
    SHA256 sha256;
    std::string hahsPass = sha256(password);
    customerID = getMaxId(table_customers);
    return registry.addNewUser(customerID, login, hahsPass);
}

void Bookshop::welcomeUser(unsigned id) const {
    std::cout << "Logged as: " << *registry.getLogin((id)) << ", ";
    if (admin) {
        std::cout << "Administrator\n\n";
    }
    else {
        std::cout << "Customer\n\n";
    }
}

char Bookshop::showMenu() {
    while (!userLogged) {
        std::cout << "Log in or sign in!\n";
        userLogged = loginOrSignIn();
    }
    if (admin) {
        return adminMenuChoice();
    }
    return customerMenuChoice();
}

void Bookshop::showAdminMenu() const {
    std::cout << "\n\n1. Show all books\n";
    std::cout << "2. Show customers\n";
    std::cout << "3. Show orders\n";
    std::cout << "4. Find \n";
    std::cout << "5. Add \n";
    std::cout << "6. Bestsellers \n";
    std::cout << "7. Incomes \n";
    std::cout << "0. Logout\n";
    std::cout << *registry.getLogin((customerID))<< ", " ;
}

void Bookshop::showCustomerMenu() const {
    std::cout << "\n\n1. Show all books\n";
    std::cout << "2. Show bestsellers \n";
    std::cout << "3. Order book \n";
    std::cout << "4. Show your books\n";
    std::cout << "5. Find book\n";
    std::cout << "6. Find order\n";
    std::cout << "0. Logut\n";
    std::cout << *registry.getLogin((customerID)) << ", ";
}

char Bookshop::customerMenuChoice(){
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
        case '3': orderBook();
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

void Bookshop::orderBook() {
    showTable(table_books);
    std::cout << "Choose book id to order: ";
    unsigned bookId;
    unsigned maxId = getMaxId(table_books);
    checkInput(bookId, 1, maxId);

    std::stringstream query;
    query << "INSERT INTO " << table_orders << " VALUES (NULL, " << customerID << ", " << bookId << ", '2021-03-20', 'pending')";
    if (dbconnection.sendQuery(query.str().c_str())) {
        system("cls");
        std::cout << "Order placed!\n";
    }
}

char Bookshop::adminMenuChoice() {
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

void Bookshop::showTable(const std::string& table) const {
    welcomeUser(customerID);

    std::stringstream query;
    query << "SELECT * FROM " << table.c_str();
    sendQueryShowResult(query);
}

void Bookshop::customerMenu() {
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

void Bookshop::bookMenu(){
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

bool Bookshop::editBook() {
    std::cout << "Book id: ";
    unsigned bookId{};
    unsigned maxId = getMaxId(table_books);
    checkInput(bookId, 1, maxId);
    std::stringstream query;
    query << "SELECT * FROM books WHERE book_id=" << bookId;
    sendQueryShowResult(query);

    std::cout << "Edit: \n";
    for (int i = 1; i < COLUMNS_BOOKS_SIZE; i++) {
        std::cout << i << ". " << columnsBooks[i] << '\n';
    }
    unsigned choice;
    checkInput(choice, 1, 4);
    std::cout << "New " << columnsBooks[choice] << ": ";

    std::string field = *dbconnection.validateStringInput();
    std::stringstream newQuery;
    newQuery << "UPDATE books SET " << columnsBooks[choice] << "='" << field.c_str() << "'"
             << " WHERE book_id=" << bookId;
    if (dbconnection.sendQuery(newQuery.str().c_str())) {
        return true;
    }
    return false;
}

bool Bookshop::removeRecord(const std::string& table) {
    const char tableType = verifyTable(table);
    std::string columnIdAsText = getColumnIdName(table);

    int maxIdfromTable = getMaxId(table);
    if (maxIdfromTable == 0) {
        return false;
    }

    std::cout << "Type id to remove: ";
    unsigned idToDelete{};
    checkInput(idToDelete, 1, maxIdfromTable, "Wrong id! Try again\n");
    if (tableType == '2' && (idToDelete == 1 || idToDelete == 2)) {
        std::cout << "You can not remove Administrator account!\n";
        return false;
    }

    std::stringstream query;
    query << "DELETE FROM " << table << " WHERE " << columnIdAsText << "=" << idToDelete;

    if (dbconnection.sendQuery(query.str().c_str())) {
        std::cout << "Record id: " << idToDelete << " removed from table " << table << '\n';
        return true;
    }
    return false;
}

const std::string Bookshop::getColumnIdName(const std::string& table) const {
    const char tableType = verifyTable(table);
    std::string columnIdAsText;
    switch(tableType)
    {
        case '1': columnIdAsText = columnsBooks[COLUMN_ID]; break;
        case '2': columnIdAsText = columnsCustomers[COLUMN_ID]; break;
        case '3': columnIdAsText = columnsOrders[COLUMN_ID]; break;
    }
    return columnIdAsText;
}

unsigned Bookshop::getMaxId(const std::string& table) const {
    std::string columnIdAsText = getColumnIdName(table);
    std::stringstream query;
    query << "SELECT " << table << "." << columnIdAsText << " FROM " << table << " WHERE " << columnIdAsText
          << "=( SELECT MAX(" << columnIdAsText << ") FROM " << table << ")";

    auto result = dbconnection.getOneField(query.str().c_str());
    if (result->size() == 0) {
        std::cout << "No records at the table!\n";
        return 0;
    }
    return stoi(*result);
}

void Bookshop::orderMenu(){
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

void Bookshop::changeOrderStatus() {
    showTable(table_orders);
    unsigned orderId;
    std::cout << "Order ID: ";
    unsigned maxId = getMaxId(table_orders);
    checkInput(orderId, 1, maxId);

    std::cout << "1. Pending\n";
    std::cout << "2. Sent\n";
    std::cout << "0. Main menu\n";

    char choice{};
    checkInput(choice, '0', '2');
    std::stringstream query;
    query << "UPDATE orders SET status='";
    switch(choice)
    {
        case '1': query << "pending"; break;
        case '2': query << "sent"; break;
        case '0': return;
    }
    query << "' WHERE order_id=" << orderId;
    std::cout << query.str() << '\n';
    if (dbconnection.sendQuery(query.str().c_str())) {
        std::cout << "Status changed\n";
    }
}

const auto Bookshop::showColumnsTakeInput(const char* tableName[], const unsigned& tableSize) const {
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

unsigned Bookshop::showCustomerById() const {
    unsigned id;
    unsigned maxId = getMaxId(table_customers);
    std::cout << "Type customer's ID: ";
    checkInput(id, 1, maxId);

    std::stringstream query;
    query << "SELECT name, surname, email FROM customers WHERE customer_id =" << id;

    sendQueryShowResult(query);
    return id;
}

void Bookshop::editCustomer() {
    std::cout << "Edit:\n";
    unsigned customerID = showCustomerById();
    std::string* userInput = showColumnsTakeInput(columnsCustomers, COLUMNS_CUSTOMERS_SIZE);

    std::stringstream query;
    query << "UPDATE " << table_customers << " SET" <<
                " name='" << userInput[0] << "'," <<
                " surname='" << userInput[1] << "'," <<
                " email='" << userInput[2] << "'" <<
                " WHERE customer_id=" << customerID;

    delete[] userInput;
    dbconnection.sendQuery(query.str().c_str());
}

 void Bookshop::sendQueryShowResult(std::stringstream& query) const {
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


void Bookshop::findInDatabase() const{
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

void Bookshop::findBook(std::stringstream& query) const {
    std::cout << "1. Id\n";
    std::cout << "2. Author\n";
    std::cout << "3. Title\n";

    char choice{};
    checkInput(choice, '1', '3');
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE ";
    switch(choice)
    {
    case '1': query << "Book_ID=" << value.c_str();
        break;
    case '2': query << "Author_surname='" << value.c_str() << "'";
        break;
    case '3': query << "Title LIKE '%" << value.c_str() << "%'";
        break;
    }
    sendQueryShowResult(query);
}

void Bookshop::findCustomer(std::stringstream& query) const {

    std::cout << "1. Id\n";
    std::cout << "2. Surname\n";
    std::cout << "3. Email\n";

    char choice{};
    checkInput(choice, '1', '3');
    std::cout << ": ";
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE ";
    switch(choice)
    {
    case '1': query << "customer_ID=" << value.c_str();
        break;
    case '2': query << "surname LIKE '%" << value.c_str() << "%'";
        break;
    case '3': query << "email='" << value.c_str() << "'";
        break;
    }
    sendQueryShowResult(query);
}

void Bookshop::findOrder(std::stringstream& query) const {
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
    std::cout << ": ";
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE ";
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

void Bookshop::addToDatabase() const{
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

const char Bookshop::verifyTable(const std::string& table) const {
    if (table == table_books) return '1';
    else if (table == table_customers) return '2';
    else return '3';
}

void Bookshop::addToTable(const std::string& table, std::stringstream& query) const {
    const char tableType = verifyTable(table);

    query << table.c_str() << " (";

    Functor showFieldsFillRecord(query);

    std::cin.ignore(10, '\n');
    switch(tableType)
    {
    case '1': showFieldsFillRecord(COLUMNS_BOOKS_SIZE, columnsBooks);
        break;
    case '2': showFieldsFillRecord(COLUMNS_CUSTOMERS_SIZE, columnsCustomers);
        break;
    case '3': showFieldsFillRecord(COLUMNS_ORDERS_SIZE, columnsOrders);
        break;
    }

    if (dbconnection.sendQuery(query.str().c_str())) {
        system("cls");
        std::cout << "Record saved\n";
    }
}

void Bookshop::showCustomerBooks() const {
    std::stringstream query;
    unsigned id{};

    if (admin) {

        unsigned maxId = getMaxId(table_customers);
        std::cout << "\n1. Type customer's ID\n";
        checkInput(id, 1, maxId);
    }
    else {
        std::cout << "Your books:\n\n";
        id = customerID;
    }

    query << "SELECT books.author_name, books.author_surname, books.Title, orders.Status"
             " FROM customers, orders, books "
             " WHERE customers.Customer_ID=" << id <<
             " && customers.Customer_ID=orders.Customer_ID"
             " && books.Book_ID=orders.Book_ID"
             " ORDER BY orders.Date";

    sendQueryShowResult(query);
}

void Bookshop::showBestSellingBooks() const {
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

void Bookshop::showIncomes() const {
    std::stringstream query;
    query << "SELECT ROUND(SUM(books.Price),2) AS Incomes"
             " FROM books, orders"
             " WHERE books.Book_ID=orders.Book_ID";

    sendQueryShowResult(query);
}
