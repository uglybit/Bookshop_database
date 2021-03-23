#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <sstream>
#include <iostream>

struct Functor {
    std::stringstream& query;
    explicit Functor(std::stringstream& q) : query(q) { }

    void operator()(const unsigned& numOfColumns, const char* const columnsName[]);
};


#endif // FUNCTOR_H
