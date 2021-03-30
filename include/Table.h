#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <vector>
#include <initializer_list>
#include <algorithm>


enum class TableType {
    books,
    customers,
    orders,
    access
};


class Table
{   std::string name;
    const unsigned COLUMNS_SIZE;
    const unsigned COLUMN_ID_POS = 0;
    std::vector<std::string> columnNames;
public:
    Table(const std::string& name, const std::initializer_list<const char*> columns);
    virtual ~Table();

    const std::string& getName() const;
    const unsigned getColsSize() const;
    const unsigned getIdPos() const;
    const std::string getColName(unsigned number) const;
    virtual const TableType type() = 0;
};

/*  All database tables will inherit form abstract class Table
    Each class derived from Table is defined in the same header because
    the classes are short and overrides only one virtual function
*/

class Books : public Table {
public:
    Books(const std::string& name, const std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Books() {}
    const TableType type() override { return TableType::books; }
};

class Customers : public Table {
public:
    Customers(const std::string& name, const std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Customers() {}
    const TableType type() override { return TableType::customers; }
};

class Orders : public Table {
public:
    Orders(const std::string& name, const std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Orders() {}
    const TableType type() override { return TableType::orders; }
};

class Access : public Table {
public:
    Access(const std::string& name, const std::initializer_list<const char*> columns) : Table(name, columns) {}
    ~Access() {}
    const TableType type() override { return TableType::access; }
};



#endif // TABLE_H
