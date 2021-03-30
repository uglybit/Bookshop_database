#include "..\include\Functor.h"

void Functor::operator()(Table* table) {
    unsigned numOfColumns = table->getColsSize();
    std::string userInput[numOfColumns];

    for (int i = 1; i < numOfColumns; i++) { // skip column: id
        query << table->getColName(i);  // adding column name to query
        if(i < numOfColumns -1 ) { // separate column names with comma
            query << ", ";
        }
        std::cout << table->getColName(i) << ": "; // show column name
        std::string input;
        std::getline(std::cin, input);
        userInput[i] = input;
    }
    query << ") VALUES (";

    for(int i = 1; i < numOfColumns -1; i++) { // i = 1: because id is autoincremented
        query << "'" << userInput[i].c_str() << "', "; // adding users input
    }
    query << "'" << userInput[numOfColumns -1].c_str() << "')"; // last field - end query
}
