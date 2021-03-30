#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <sstream>
#include <iostream>
#include "Table.h"

/*  Class Functor:
    Constructor gets reference to query then overloaded 'operator()'
    take polimoprhic pointer to table, then:
        1. shows fields for user
        2. take input form user
        3. modify query

*/
struct Functor {
    std::stringstream& query;
    explicit Functor(std::stringstream& q) : query(q) { }

    void operator()(Table* table);
};


#endif // FUNCTOR_H
