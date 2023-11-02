//2. Автостоянка содержит одну полосу,  на которой может быть
//        размещено до N машин.  Имеется план прибытия и убытия машин на
//        стоянку.  Если  в момент прибытия стоянка оказывается занятой,
//машина уезжает восвояси.  Машины въезжают с южной  стороны,  а
//        могут  выехать  из K (K<N) крайних мест северной стороны.  При
//        выезде машины очередь сдвигается с юга на север. Требуется:
//1) промоделировать работу стоянки,  последовательно сообщая
//о приходе и  убытии  машин  с  выдачей  информации  о  стоящих
//машинах;
//2) выдать два списка номеров машин:  тех, которые не смогли
//        встать на стоянку, и тех, что не смогли выехать вовремя (6).
//Арсланов Р. ПС-22
//CLion 2023.2.1

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

struct Node {
    string carNumber;
    Node *next{};
};

struct Queue {
    Node *first;
    Node *last;
    int size;
};

void InitQueue(Queue *Q) {
    Q->first = new Node;
    Q->first->next = nullptr;
    Q->last = Q->first;
    Q->size = 0;
}

void OutputQueue(Queue *Q) {
    Node *current = Q->first;

    while (current->next != nullptr) {
        cout << current->next->carNumber << " ";
        current = current->next;
    }

    cout << endl << endl;
}

void AddToQueue(Queue *Q, const string &carNumber) {
    Q->last->next = new Node;
    Q->last = Q->last->next;
    Q->last->carNumber = carNumber;
    Q->last->next = nullptr;
    Q->size++;
}

void RemoveFromQueue(Queue *Q, Queue *cancelledDeparture, const string &carNumber, int K) {
    Node *current = Q->first;

    int tempK = 0;

    while (current->next != nullptr) {
        tempK++;
        if (current->next->carNumber == carNumber) {
            if (tempK > K) {
                cout << "Выезд пока не доступен для машины с номером " << carNumber << endl;
                AddToQueue(cancelledDeparture, carNumber);
            } else {
                Node *temp = current->next;
                current->next = temp->next;
                if (tempK == Q->size) {
                    Q->last = current;
                }
                Q->size--;
                delete temp;
                cout << "Машина с номером " << carNumber << " выехала." << endl;
            }
            return;
        }

        current = current->next;
    }

    cout << "Машина с номером " << carNumber << " не найдена на стоянке" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Queue parking{};
    Queue cancelledParking{};
    Queue cancelledDeparture{};

    InitQueue(&parking);
    InitQueue(&cancelledParking);
    InitQueue(&cancelledDeparture);

    cout << "Введите количество парковочных мест" << endl;
    int N;
    cin >> N;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод. Введите количество парковочных мест заново" << endl;
        cin >> N;
    }

    cout << "Введите количество мест для выезда" << endl;
    int K;
    cin >> K;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод. Введите количество мест для выезда заново" << endl;
        cin >> K;
    }

    while (K >= N) {
        cout << "Количество мест для выезда должно быть меньше количества парковочных мест" << endl;
        cin >> K;
    }

    cin.ignore();

    cout << "Введите имя файла" << endl;
    string inputFileName;
    getline(cin, inputFileName);
    ifstream inFile(inputFileName);
    cout << endl;

    if (!inFile.is_open()) {
        cout << "Ошибка в открытии файла" << endl;
        return 0;
    }

    string stringFromFile;

    while (!inFile.eof()) {
        getline(inFile, stringFromFile);

        istringstream iss(stringFromFile);

        basic_string<char> carNumber;
        string action;

        iss >> carNumber >> action;
        if (action == "прибытие") {
            if (parking.size >= N) {
                cout << "Стоянка заполнена. Машина с номером " << carNumber << " не может прибыть на стоянку" << endl;
                AddToQueue(&cancelledParking, carNumber);
                cout << "Номера машин на стоянке: ";
                OutputQueue(&parking);
            } else {
                AddToQueue(&parking, carNumber);
                cout << "Машина с номером " << carNumber << " добавлена" << endl;
                cout << "Номера машин на стоянке: ";
                OutputQueue(&parking);
            }
        } else if (action == "выезд") {
            RemoveFromQueue(&parking, &cancelledDeparture, carNumber, K);
            cout << "Номера машин на стоянке: ";
            OutputQueue(&parking);
        }
    }

    cout << "Номера машин, которые не смогли встать на стоянку: ";
    OutputQueue(&cancelledParking);
    cout << "Номера машин, которые не смогли выехать вовремя со стоянки: ";
    OutputQueue(&cancelledDeparture);
}
