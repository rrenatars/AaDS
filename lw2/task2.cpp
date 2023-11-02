//2. ����������� �������� ���� ������,  �� ������� ����� ����
//        ��������� �� N �����.  ������� ���� �������� � ������ ����� ��
//        �������.  ����  � ������ �������� ������� ����������� �������,
//������ ������� ��������.  ������ �������� � �����  �������,  �
//        �����  �������  �� K (K<N) ������� ���� �������� �������.  ���
//        ������ ������ ������� ���������� � ��� �� �����. ���������:
//1) ��������������� ������ �������,  ��������������� �������
//� ������� �  ������  �����  �  �������  ����������  �  �������
//�������;
//2) ������ ��� ������ ������� �����:  ���, ������� �� ������
//        ������ �� �������, � ���, ��� �� ������ ������� ������� (6).
//�������� �. ��-22
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
                cout << "����� ���� �� �������� ��� ������ � ������� " << carNumber << endl;
                AddToQueue(cancelledDeparture, carNumber);
            } else {
                Node *temp = current->next;
                current->next = temp->next;
                if (tempK == Q->size) {
                    Q->last = current;
                }
                Q->size--;
                delete temp;
                cout << "������ � ������� " << carNumber << " �������." << endl;
            }
            return;
        }

        current = current->next;
    }

    cout << "������ � ������� " << carNumber << " �� ������� �� �������" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Queue parking{};
    Queue cancelledParking{};
    Queue cancelledDeparture{};

    InitQueue(&parking);
    InitQueue(&cancelledParking);
    InitQueue(&cancelledDeparture);

    cout << "������� ���������� ����������� ����" << endl;
    int N;
    cin >> N;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "�������� ����. ������� ���������� ����������� ���� ������" << endl;
        cin >> N;
    }

    cout << "������� ���������� ���� ��� ������" << endl;
    int K;
    cin >> K;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "�������� ����. ������� ���������� ���� ��� ������ ������" << endl;
        cin >> K;
    }

    while (K >= N) {
        cout << "���������� ���� ��� ������ ������ ���� ������ ���������� ����������� ����" << endl;
        cin >> K;
    }

    cin.ignore();

    cout << "������� ��� �����" << endl;
    string inputFileName;
    getline(cin, inputFileName);
    ifstream inFile(inputFileName);
    cout << endl;

    if (!inFile.is_open()) {
        cout << "������ � �������� �����" << endl;
        return 0;
    }

    string stringFromFile;

    while (!inFile.eof()) {
        getline(inFile, stringFromFile);

        istringstream iss(stringFromFile);

        basic_string<char> carNumber;
        string action;

        iss >> carNumber >> action;
        if (action == "��������") {
            if (parking.size >= N) {
                cout << "������� ���������. ������ � ������� " << carNumber << " �� ����� ������� �� �������" << endl;
                AddToQueue(&cancelledParking, carNumber);
                cout << "������ ����� �� �������: ";
                OutputQueue(&parking);
            } else {
                AddToQueue(&parking, carNumber);
                cout << "������ � ������� " << carNumber << " ���������" << endl;
                cout << "������ ����� �� �������: ";
                OutputQueue(&parking);
            }
        } else if (action == "�����") {
            RemoveFromQueue(&parking, &cancelledDeparture, carNumber, K);
            cout << "������ ����� �� �������: ";
            OutputQueue(&parking);
        }
    }

    cout << "������ �����, ������� �� ������ ������ �� �������: ";
    OutputQueue(&cancelledParking);
    cout << "������ �����, ������� �� ������ ������� ������� �� �������: ";
    OutputQueue(&cancelledDeparture);
}
