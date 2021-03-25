#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <sstream>
#include <iostream>
#include "Table.h"

struct Functor {
    std::stringstream& query;
    explicit Functor(std::stringstream& q) : query(q) { }

    void operator()(Table* table);
};


#endif // FUNCTOR_H
