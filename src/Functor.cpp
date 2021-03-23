#include "Functor.h"

void Functor::operator()(const unsigned& numOfColumns, const char* const columnsName[]) {
    std::string userInput[numOfColumns];  // tresc do bazy danych
    for (int i = 1; i < numOfColumns; i++) { // pomijamy id wiec i = 1
        query << columnsName[i];  // dopisujemy do zapytania nazwe kloumny
        if(i < numOfColumns -1 ) { // przecinek wewnatrz nawiasow ale nie po ostatniej kolumnie
            query << ", ";
        }
        std::cout << columnsName[i] << ": ";
        std::string input;
        std::getline(std::cin, input); // wielowyrazoe wartosci moga sie zdarzyc
        userInput[i] = input; // zapisaujemy po kolei nowe wartosci do bazy
    }
    query << ") VALUES (";

    for(int i = 1; i < numOfColumns -1; i++) {
        query << "'" << userInput[i].c_str() << "', "; // dopisujemy kazde nowe pole wprowadzone przez usera
    }
    query << "'" << userInput[numOfColumns -1].c_str() << "')"; // ostatnia nie moze miec przecinka wiec oddzielnie
    }
