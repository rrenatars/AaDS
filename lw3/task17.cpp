//17. Информация  о  файлах  на  жестких  дисках   компьютера
//записана с помощью дерева. Листьям соответствуют файлы. Удалить
//все копии одного и того же файла, оставив самую позднюю.  Если
//папка оказывается пустой, то она тоже должна быть удалена (11).
//Арсланов Ренат ПС-22
//CLion 2023.2.2

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>
#include <chrono>
#include <iomanip>
#include <Windows.h>

using namespace std;

struct Tree {
    string name;
    vector<Tree *> sons;
    Tree *fath;
    chrono::system_clock::time_point date;
    bool isFile = false;
};

bool isFile(const string &name) {
    size_t dotPosition = name.find_last_of('.');

    return (dotPosition != string::npos && dotPosition < name.size() - 1);
}

void readFromFile(ifstream &file, Tree *&root) {
    int i, previousCountOfDashes;
    Tree *currentNode, *temp, *previousNode;

    previousCountOfDashes = -1;
    previousNode = nullptr;

    string line;
    while (getline(file, line)) {
        int countOfDashes = 0;
        for (char c: line) {
            if (c == '-') {
                countOfDashes++;
            } else {
                break;
            }
        }

        line.erase(0, line.find_first_not_of('-'));

        currentNode = new Tree;
        string nameAndDate;

        istringstream iss(line);
        while (getline(iss, nameAndDate)) {
            if (!nameAndDate.empty()) {
                istringstream iss(nameAndDate);
                iss >> currentNode->name;
                tm timeStruct = {};

                iss >> get_time(&timeStruct, "%Y-%m-%d %H:%M:%S");

                currentNode->date = chrono::system_clock::from_time_t(mktime(&timeStruct));
                currentNode->isFile = isFile(currentNode->name);

                if (countOfDashes == 0) {
                    root = currentNode;
                    previousNode = root;
                    previousCountOfDashes = 0;
                    root->fath = nullptr;
                    continue;
                }
                if (countOfDashes > previousCountOfDashes) {
                    previousNode->sons.push_back(currentNode);
                    currentNode->fath = previousNode;
                } else if (countOfDashes == previousCountOfDashes) {
                    temp = previousNode->fath;
                    temp->sons.push_back(currentNode);
                    currentNode->fath = temp;
                } else {
                    temp = previousNode;
                    for (i = 0; i <= previousCountOfDashes - countOfDashes; i++) {
                        temp = temp->fath;
                    }
                    temp->sons.push_back(currentNode);
                    currentNode->fath = temp;
                }
                previousCountOfDashes = countOfDashes;
                previousNode = currentNode;
            }
        }
    }
}

void PrintTree(Tree *currentNode, int level) {
    for (int i = 0; i < level; i++) {
        cout << '-';
    }
    cout << currentNode->name;
    auto time = std::chrono::system_clock::to_time_t(currentNode->date);
    std::tm timeStruct = *std::localtime(&time);

    cout << ' ' << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    cout << endl;
    if (currentNode->sons.size()) {
        for (Tree *&son: currentNode->sons) {
            PrintTree(son, level + 1);
        }
    }
}

void deleteNode(Tree *currentNode) {
    if (currentNode->fath != nullptr) {
        currentNode->fath->sons.erase(
                remove(currentNode->fath->sons.begin(), currentNode->fath->sons.end(), currentNode),
                currentNode->fath->sons.end()
        );
    }
    delete currentNode;
}

void deleteRepeatFiles(Tree *root) {
    if (!root) return;

    queue<Tree *> nodesQueue;
    nodesQueue.push(root);

    map<string, Tree*> latestFiles;

    while (!nodesQueue.empty()) {
        Tree *currentNode = nodesQueue.front();
        nodesQueue.pop();

        if (currentNode->isFile) {
            auto it = latestFiles.find(currentNode->name);
            if (it == latestFiles.end() || it->second->date < currentNode->date) {
                if (it != latestFiles.end()) {
                    if (it->second->fath->sons.size() == 1) {
                        deleteNode(it->second->fath);
                    }
                    deleteNode(it->second);
                }

                latestFiles[currentNode->name] = currentNode;
            } else {
                if (currentNode->fath->sons.size() == 1) {
                    deleteNode(currentNode->fath);
                }
                deleteNode(currentNode);
                continue;
            }
        }

        if (!currentNode->isFile) {
            if (!currentNode->sons.empty()) {
                for (Tree *&son: currentNode->sons) {
                    nodesQueue.push(son);
                }
            } else {
                deleteNode(currentNode);
            }
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Tree *root = nullptr;

    string fileName;
    getline(cin, fileName);
    cout << fileName << endl;

    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Ошибка в открытии файла" << endl;
    }

    readFromFile(inputFile, root);
    deleteRepeatFiles(root);
    PrintTree(root, 0);
}