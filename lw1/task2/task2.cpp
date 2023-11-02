//2. Задан текстовый файл.  Переносов слов нет. Создать новый
//файл, в котором каждое слово будет записано в обратном порядке
//(6).
//
//Арсланов Р. ПС-22
//CLion 2023.2.1

#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "Write input file name with file name extension" << std::endl;
    std::wstring inputFileName;
    std::getline(std::wcin, inputFileName);
    std::ifstream inFile(inputFileName);

    if (!inFile.is_open()) {
        std::wcout << "Error in open file for read" << std::endl;
        return 0;
    }

    std::cout << "Write output file name with file name extension" << std::endl;
    std::wstring outputFileName;
    std::getline(std::wcin, outputFileName);
    std::ofstream outFile(outputFileName);

    if (!outFile.is_open()) {
        std::wcout << "Error in open file for record" << std::endl;
        return 0;
    }

    char character;
    std::string word;
    while (!inFile.eof()) {
        inFile.get(character);
        bool found = false;
        if (('A' <= character && character <= 'Z') || ('a' <= character && character <= 'z') || ('А' <= character && character <= 'я')) {
            found = true;
        }

        if (found) {
            word += character;
        } else {
            std::string reversedWord;
            for (int i = word.length() - 1; i >= 0; i--) {
                reversedWord += word[i];
            }
            outFile << reversedWord;
            word.clear();
            reversedWord.clear();
            outFile << character;
        }
    }
    inFile.close();
    outFile.close();
}