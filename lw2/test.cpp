#include <iostream>
#include <string>

using namespace std;

struct Node {
    string element;
    Node *next{};
};

struct Queue {
    Node *first;
    Node *last;
    int size;
};

void Init(Queue *Q) {
    Q->first = new Node;
    Q->first->next = nullptr;
    Q->last = Q->first;
    Q->size = 0;
}

void Add(Queue *Q, const string& elem) {
    Q->last->next = new Node;
    Q->last = Q->last->next;
    Q->last->element = elem;
    Q->last->next = nullptr;
    Q->size++;
}

void Output(Queue *Q) {
    Node *current = Q->first;

    if (!Q->size) {
        cout << "������� �����" << endl;
    } else {
        cout << "�������� � �������: ";

        while (current->next != nullptr) {
            cout << current->next->element << " ";
            current = current->next;
        }
    }

    cout << endl << endl;
}

void Remove(Queue *Q, const string& elem) {
    Node *current = Q->first;

    int counterOfNodes = 0;

    if (!Q->size) {
        cout << "������� �����" << endl;
    } else {
        while (current->next != nullptr) {
            counterOfNodes++;
            if (current->next->element == elem) {
                Node *temp = current->next;
                current->next = temp->next;
                cout << "������� " << elem << " ������" << endl << endl;
                if (counterOfNodes == Q->size) {
                    Q->last = current;
                }
                Q->size--;
//                if (!Q->size) {
//                    cout << "������� ����� ����� ��������" << endl;
//                    Q->first->next = new Node;
//                    Q->first->next = nullptr;
//                    Q->last = Q->first;
//                }
                 delete temp;
                return;
            }
            current = current->next;
        }

        cout << "������� �� ������" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Queue Q{};
    Init(&Q);

    string element;

    int numberOfOperation = 0;

    while (numberOfOperation != 4) {
        cout << "������� ����� ��������" << endl;
        cout << "1: �������� ������� � �������" << endl;
        cout << "2: ������� ������� �� �����" << endl;
        cout << "3: ������� ������� �� �������" << endl;
        cout << "4: ����� �� ���������" << endl;

        cin >> numberOfOperation;
        cout << endl;
        switch (numberOfOperation) {
            case 1:
                cout << "������� �������, ������� �� ������ �������� � �������:   ";
                cin >> element;
                cout << endl;
                Add(&Q, element);
                break;
            case 2:
                Output(&Q);
                break;
            case 3:
                cout << "������� �������, ������� �� ������ �������: ";
                cin >> element;
                cout << endl;
                Remove(&Q, element);
                break;
        }
    }
}