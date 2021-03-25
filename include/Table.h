#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <vector>
#include <initializer_list>


enum class TableType {
    books,
    customers,
    orders
};


class Table
{   std::string name;
    const unsigned COLUMNS_SIZE;
    const unsigned COLUMN_ID_POS = 0;
    std::vector<std::string> columnNames;
public:
    Table(std::string name, std::initializer_list<const char*> columns);
    Table() : COLUMNS_SIZE{3}{}
    virtual ~Table();

    const std::string& getName() const;
    const unsigned getColsSize() const;
    const unsigned getIdPos() const;
    const std::string getColName(unsigned number) const;
    virtual const TableType type() = 0;
};

class Books : public Table {
public:
    Books(std::string name, std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Books() {}
    const TableType type() { return TableType::books; }
};

class Customers : public Table {
public:
    Customers(std::string name, std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Customers() {}
    const TableType type() { return TableType::customers; }
};

class Orders : public Table {
public:
    Orders(std::string name, std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Orders() {}
    const TableType type() { return TableType::orders; }
};



#endif // TABLE_H
