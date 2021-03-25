#include "DataValidation.h"

void checkInput(char& character, const char& first, const char& last, const char* text) {
    std::cout << text;
    char input = 'a';
    while (input < first || input > last) {
        std::cout << "Choose between " << first << " and " << last << '\n';
        std::cin >> input;
    };
    character = input;
    system("cls");
}

void checkInput(const std::string& str, const char* text) {

}

void checkInput(unsigned& number, int first, int last, const std::string& text) {
    int input = -1;

    while (std::cin.fail() || input < first || input > last) {
        std::cout << text;
        std::cout << "Choose between " << first << " and " << last << '\n';
        std::cin >> input;
        std::cin.clear();
        std::cin.ignore(100, '\n');
        
    }
    number = input;
}
