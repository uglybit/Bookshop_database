#include "Functor.h"

void Functor::operator()(const unsigned& numOfColumns, const char* const columnsName[]) {
    std::string userInput[numOfColumns];  
    for (int i = 1; i < numOfColumns; i++) { // skip column: id
        query << columnsName[i];  // adding column name to query
        if(i < numOfColumns -1 ) { // separate column names with comma
            query << ", ";
        }
        std::cout << columnsName[i] << ": "; // show column name 
        std::string input;
        std::getline(std::cin, input); 
        userInput[i] = input; // 
    }
    query << ") VALUES (";

    for(int i = 1; i < numOfColumns -1; i++) { // i = 1: column id is autoincremented
        query << "'" << userInput[i].c_str() << "', "; // adding users input
    }
    query << "'" << userInput[numOfColumns -1].c_str() << "')"; // last field - end query
}
