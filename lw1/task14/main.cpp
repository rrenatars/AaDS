#include <iostream>
#include <fstream>

int main() {
    std::cout << "Write input file name" << std::endl;
    std::string inputFileName;
    std::getline(std::cin, inputFileName);
    std::ifstream inFile(inputFileName);

    if (!inFile.is_open()) {
        std::cout << "Error in open file for read" << std::endl;
        return 0;
    } else {
        char character;
        int number = 0;

        while (!inFile.eof()) {
            inFile.get(character);
            if ('0' <= character && character <= '9' || character == '-') {
                number = number * 10 + (int) character - '0';
//                std::cout << "character" << character << "to" << (int)character - '0' << "int" << std::endl;
            } else {
                std::cout << number << std::endl;
                number = 0;
            }
        }
    }
}