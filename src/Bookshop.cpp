#include "..\include\Bookshop.h"
#include "..\include\DataValidation.h"


Bookshop::Bookshop(DBConnection& dbc) : dbconnection(dbc), registry(dbc) {

    books = new Books{"books", {"Book_ID", "Author_name", "Author_surname", "Title", "Price"}};
    customers = new Customers{"customers", {"Customer_ID", "Name", "Surname", "Email"}};
    orders = new Orders{"orders", {"Order_ID", "Customer_ID", "Book_ID", "Date", "Status"}};
    userLogged = loginOrSignIn();

}

Bookshop::~Bookshop() {
   if (books) {
        delete books;
        books = nullptr;
    }
    if (customers) {
        delete customers;
        customers = nullptr;
    }
    if (orders) {
        delete orders;
        orders = nullptr;
    }

}

bool Bookshop::loginOrSignIn() {
    std::cout << "1. Log in\n";
    std::cout << "2. Sign in\n";
    unsigned choice;
    checkInput(choice, 1, 2);
    if (choice == 1) {
       return  login();
    }
    return signIn();
}

bool Bookshop::login() {
    std::string login {"schopenhauer"};/* {"mithrandir"}; {"ibish"};*/
    std::string password {"Qwerty!1"}; /*{"Mnbvcx!1"};  {"Zxcvbn!1"};*/
    login = *dbconnection.validateStringInput("Login");

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
        std::cout << password << '\n';
        std::stringstream query;
        query << "INSERT INTO ";
        addToTable(customers, query);
        return saveNewUserToRegistry(login, password);
    }
    else {
        signIn();
    }
}

bool Bookshop::saveNewUserToRegistry(const std::string& login, const std::string& password) {
    SHA256 sha256;
    std::string hahsPass = sha256(password);
    customerID = getMaxId(customers);
    return registry.addNewUser(customerID, login, hahsPass);
}

void Bookshop::welcomeUser(unsigned id) const {
    system("cls");
    std::cout << "Logged as: " << *registry.getLogin(id) << ", ";
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

char Bookshop::adminMenuChoice() {
    showAdminMenu();

    unsigned choice{};
    checkInput(choice, 0, 7);
    switch(choice)
    {
        case 1: bookMenu();
            break;
        case 2: manageCustomer();
            break;
        case 3: orderMenu();
            break;
        case 4: findInDatabase();
            break;
        case 5: addToDatabase();
            break;
        case 6: showBestSellingBooks();
            break;
        case 7: showIncomes();
            break;
        case 0 : return '0';
    }
    return 'a';
}

void Bookshop::showCustomerMenu() const {
    std::cout << "\n\n1. Show all books\n";
    std::cout << "2. Show bestsellers \n";
    std::cout << "3. Order book \n";
    std::cout << "4. Show your books\n";
    std::cout << "5. Find book\n";
    std::cout << "0. Logut\n";
    std::cout << *registry.getLogin(customerID) << ", ";
}

char Bookshop::customerMenuChoice(){
    std::stringstream query;
    query << "SELECT * FROM ";
    showCustomerMenu();
    unsigned choice{};
    checkInput(choice, 0, 5);
    system("cls");

    switch(choice)
    {
        case 1: showTable(books);
            break;
        case 2: showBestSellingBooks();
            break;
        case 3: orderBook();
            break;
        case 4: showCustomerBooks();
            break;
        case 5:  query << books->getName().c_str();
                   findBook(query);
            break;
        case 0: return '0';
    }
    return 'a';
}


void Bookshop::manageCustomer() {
    showTable(customers);
    std::cout << "1. Show customer's books\n";
    std::cout << "2. Edit customer's data\n";
    std::cout << "3. Remove customer\n";
    std::cout << "0. Main menu\n";

    unsigned choice{};
    checkInput(choice, 0, 3);
    system("cls");
    switch(choice)
    {
        case 1: showCustomerBooks(); break;
        case 2: editRecord(customers); break;
        case 3: removeRecord(customers); break;
        case 0: return;
    }
 }

void Bookshop::orderBook() {
    showTable(books);
    std::cout << "Choose book id to order: ";
    unsigned bookId;
    unsigned maxId = getMaxId(books);
    checkInput(bookId, 1, maxId);

    std::stringstream query;
    query << "INSERT INTO " << orders->getName().c_str() << " VALUES (NULL, " << customerID << ", " << bookId << ", '2021-03-20', 'pending')";
    if (dbconnection.sendQuery(query.str().c_str())) {
        system("cls");
        std::cout << "Order placed!\n";
    }
}

void Bookshop::showTable(Table* table) const {
    system("cls");
    welcomeUser(customerID);

    std::stringstream query;
    query << "SELECT * FROM " << table->getName().c_str();
    sendQueryShowResult(query);
}

void Bookshop::bookMenu(){
    showTable(books);
    std::cout << "1. Edit book\n";
    std::cout << "2. Remove book\n";
    std::cout << "0. Main menu\n";

    unsigned choice{};
    checkInput(choice, 0, 2);
    system("cls");
    switch(choice)
    {
        case 1: editRecord(books); break;
        case 2: removeRecord(books); break;
        case 0: return;
    }
 }

 void Bookshop::addToTable(Table* table, std::stringstream& query) const {

    auto res = table->getName().c_str();
    std::cout << res << '\n';
    query << table->getName().c_str() << " (";

    Functor showFieldsFillRecord(query);
    std::cin.ignore(10, '\n');
    showFieldsFillRecord(table);

    if (dbconnection.sendQuery(query.str().c_str())) {
        system("cls");
        std::cout << "Record saved\n";
    }
 }

void Bookshop::editRecord(Table* table) {
    showTable(table);
    std::cout << "\nEdit:\n";
    unsigned recordID = showRecordById(table);
    const std::string* userInput = showColumnsTakeInput(table);
    unsigned columnsSize = table->getColsSize();

    std::stringstream query;
    query << "UPDATE " << table->getName() << " SET ";

    int i = 1;
    for (; i < columnsSize -1; i++) {
        query << table->getColName(i) <<"='" << userInput[i -1] << "', ";
    }

    query << table->getColName(i).c_str()
    << "='" << userInput[i-1].c_str() << "'";
    query <<" WHERE ";
    query << table->getColName(table->getIdPos()) << "=" << recordID;

    delete[] userInput;

     if (dbconnection.sendQuery(query.str().c_str())) {
        system("cls");
        std::cout << "Record saved\n";
    }
}

bool Bookshop::removeRecord(Table* table) {
    std::string columnIdAsText = table->getColName(table->getIdPos());

    int maxIdfromTable = getMaxId(table);
    if (maxIdfromTable == 0) {
        return false;
    }

    std::cout << "Type id to remove: ";
    unsigned idToDelete{};
    checkInput(idToDelete, 1, maxIdfromTable, "Wrong id! Try again\n");
    if (verifyTable(table) == TableType::customers && (idToDelete == 1 || idToDelete == 2)) {
        std::cout << "You can not remove Administrator account!\n";
        return false;
    }

    std::stringstream query;
    query << "DELETE FROM " << table->getName()<< " WHERE " << columnIdAsText << "=" << idToDelete;

    if (dbconnection.sendQuery(query.str().c_str())) {
        std::cout << "Record id: " << idToDelete << " removed from table " << table->getName() << '\n';
        return true;
    }
    return false;
}

/*
const std::string Bookshop::getColumnIdName(Table* table) const {
    return table->getColName(table->getIdPos());
}
*/

unsigned Bookshop::getMaxId(Table* table) const {
    std::string columnIdAsText = table->getColName(table->getIdPos());
    std::stringstream query;
    query << "SELECT " << table->getName() << "." << columnIdAsText << " FROM " << table->getName() << " WHERE " << columnIdAsText
          << "=( SELECT MAX(" << columnIdAsText << ") FROM " << table->getName() << ")";

    auto result = dbconnection.getOneField(query.str().c_str());
    if (result->size() == 0) {
        std::cout << "No records at the table!\n";
        return 0;
    }
    return stoi(*result);
}

void Bookshop::orderMenu(){
    showTable(orders);
    std::cout << "1. Change status\n";
    std::cout << "0. Main menu\n";

    unsigned choice{};
    checkInput(choice, 0, 1);
    switch(choice)
    {
        case 1: changeOrderStatus(); break;
        case 0: return;
    }
}

void Bookshop::changeOrderStatus() {
    showTable(orders);
    unsigned orderId;
    std::cout << "Order ID: ";
    unsigned maxId = getMaxId(orders);
    checkInput(orderId, 1, maxId);

    std::cout << "1. Pending\n";
    std::cout << "2. Sent\n";
    std::cout << "0. Main menu\n";

    unsigned choice{};
    checkInput(choice, 0, 2);
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

const std::string* Bookshop::showColumnsTakeInput(Table* table) const {
    std::cin.clear();
    std::cin.ignore();
    auto tempTable = new std::string[table->getColsSize()];

    for (unsigned i = 1; i < table->getColsSize(); i++) {
        std::cout << table->getColName(i) << ": ";
        std::string input;
        std::getline(std::cin, input);
        tempTable[i-1] = input;
    }
    return tempTable;
}

unsigned Bookshop::showRecordById(Table* table) const {
    unsigned id;
    unsigned maxId = getMaxId(table);
    std::cout << "Type " << table->getName() << " ID: ";
    checkInput(id, 1, maxId);

    std::stringstream query;
    query << "SELECT ";
    addColumsToQuery(table, query);
    query << " FROM " << table->getName() << " WHERE ";
    query << table->getColName(table->getIdPos()) << "=" << id;

    sendQueryShowResult(query);
    return id;
}

void Bookshop::addColumsToQuery(Table* table, std::stringstream& query) const {
    int i = 1;
    for (; i < table->getColsSize() -1; i++) {
        query << table->getColName(i) << ", ";
    }
    query << table->getColName(i);
    std::cout << query.str() << '\n';
}

 void Bookshop::sendQueryShowResult(std::stringstream& query) const {
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

    unsigned choice{};
    checkInput(choice, 0, 3);
    if(choice == '0') return;

    std::cout << "Find by\n";

    switch(choice)
    {
    case 1:
        query << books->getName().c_str();
        findBook(query);
    break;
    case 2:
        query << customers->getName().c_str();
        findCustomer(query);
    break;
    case 3:
        query << orders->getName().c_str();
        findOrder(query);
    break;
    }
}

void Bookshop::findBook(std::stringstream& query) const {
    std::cout << "1. Id\n";
    std::cout << "2. Author's name\n";
    std::cout << "3. Author's surname\n";
    std::cout << "4. Title\n";

    unsigned choice{};
    checkInput(choice, 1, 4);
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE " << books->getColName(choice -1) << "='" << value.c_str() << "'";
    sendQueryShowResult(query);
}

void Bookshop::findCustomer(std::stringstream& query) const {

    std::cout << "1. Id\n";
    std::cout << "2. Name\n";
    std::cout << "3. Surname\n";
    std::cout << "4. Email\n";

    unsigned choice;
    checkInput(choice, 1, 4);
    std::cout << ": ";
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE ";
    query << customers->getColName(choice -1) << "='" << value << "'";
    std::cout << query.str() << '\n';

    sendQueryShowResult(query);
}

void Bookshop::findOrder(std::stringstream& query) const {
    unsigned choice{};

    std::cout << "1. Order Id\n";
    if (admin) {
        std::cout << "2. Client Id\n";
        std::cout << "3. Book Id\n";
        std::cout << "4. Date\n";
        std::cout << "5. Status\n";
        checkInput(choice, 1, 5);
    }
    else {
        choice = 1;
    }
    std::cout << ": ";
    std::string value = *dbconnection.validateStringInput();

    query << " WHERE ";
    query << orders->getColName(choice -1) << "=";
    query << value.c_str();

    sendQueryShowResult(query);
}

void Bookshop::addToDatabase() const{
    std::cout << "1. Add book\n";
    std::cout << "2. Add customer\n";
    std::cout << "3. Add order\n";
    std::cout << "0. Main menu\n";

    std::stringstream query;
    query << "INSERT INTO ";

    unsigned choice{};
    checkInput(choice, 0, 3);

    switch(choice)
    {
    case 1: addToTable(books, query);
        break;
    case 2:  addToTable(customers, query);
        break;
    case 3:  addToTable(orders, query);
        break;
    case 0: return;
    }
}

const TableType Bookshop::verifyTable(Table* table) const {
    return table->type();
}

void Bookshop::showCustomerBooks() const {
    system("cls");
    std::stringstream query;
    unsigned id{};

    if (admin) {

        unsigned maxId = getMaxId(customers);
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
    system("cls");
    std::stringstream query;
    query << "SELECT ROUND(SUM(books.Price),2) AS Incomes"
             " FROM books, orders"
             " WHERE books.Book_ID=orders.Book_ID";

    sendQueryShowResult(query);
}
