#ifndef DATAVALIDATION_H
#define DATAVALIDATION_H
#include <iostream>


void checkInput(char& character, const char& first, const char& last, const char* text = "");
void checkInput(const std::string& str, const char* text = "");
void checkInput(unsigned& number, int low, int high, const std::string& text = "");



#endif // DATAVALIDATION_H
