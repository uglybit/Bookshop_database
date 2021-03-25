#include "Table.h"
#include "..\include\Table.h"

Table::Table(std::string name, std::initializer_list<const char*> columns) : name{name}, COLUMNS_SIZE{columns.size()} {
    for ( auto a : columns) {
        columnNames.push_back(a);
    }
}

Table::~Table()
{
    //dtor
}

const std::string& Table::getName() const {
    return name;
}

const unsigned Table::getColsSize() const {
    return COLUMNS_SIZE;
}

const unsigned Table::getIdPos() const {
    return COLUMN_ID_POS;
}

const std::string Table::getColName(unsigned number) const {
    try {
        return columnNames.at(number);
    }
    catch(std::out_of_range) {
        std::cout << "Column out of range of the table!";
    }
}
