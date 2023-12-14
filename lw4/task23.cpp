#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>
#include <sstream>
#include <string>
#include <regex>
#include <queue>

struct Apartment {
    int NumberOfRooms; //колличество комнат
    bool Balcony; //наличие балкона
    bool Phone; //наличие телефона
};
struct HouseAvailable {
    std::vector<Apartment> ApartmentsAvailable;
};
struct HomeNeeded {
    std::vector<Apartment> ApartmentsNeeds;
};
struct Client {
    std::string NameClient; //имя клиента
    HouseAvailable ClientHouseAvailable;// квартира которая есть у клиента
    HomeNeeded ClientHomeNeeded;// квартира которые подходят клиенту
};

struct DateForViewing {
    int NumberOfEligibleClients;
    int IndexClientInVector;
};

Client CreateClientData(const std::string& CustomerData) {
    std::istringstream iss(CustomerData);

    Client newClient;

    iss >> newClient.NameClient;

    int ownApartmentsCount, ownRooms, desiredApartmentsCount, desiredRooms;
    char ownBalconyChar, ownPhoneChar, desiredBalconyChar, desiredPhoneChar;

    // Считываем данные о своих квартирах
    iss >> ownApartmentsCount;
    for (int i = 0; i < ownApartmentsCount; ++i) {
        iss >> ownRooms >> ownBalconyChar >> ownPhoneChar;
        Apartment ownApartment{ownRooms, (ownBalconyChar == '+'), (ownPhoneChar == '+')};
        newClient.ClientHouseAvailable.ApartmentsAvailable.push_back(ownApartment);
    }

    char separator; // Точка с запятой
    iss >> separator;

    // Считываем данные о нужных квартирах
    iss >> desiredApartmentsCount;
    for (int i = 0; i < desiredApartmentsCount; ++i) {
        iss >> desiredRooms >> desiredBalconyChar >> desiredPhoneChar;
        Apartment desiredApartment{desiredRooms, (desiredBalconyChar == '+'), (desiredPhoneChar == '+')};
        newClient.ClientHomeNeeded.ApartmentsNeeds.push_back(desiredApartment);
    }

    return newClient;
}

std::vector<Client> readData() {
    std::ifstream inputFile("exchange_data.txt");
    std::vector<Client> allClients;

    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл с данными." << std::endl;
        return allClients;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);

        allClients.push_back(CreateClientData(line));
    }

    inputFile.close();

    return allClients;
}

std::vector<std::vector<int>> StoringGraphUsingMatrix(std::vector<Client> AllClients) {
    std::vector<std::vector<int>> Matrix(AllClients.size(), std::vector<int>(
            AllClients.size()));//создаю матрицу N*N где N колличество клиентов
    std::cout << "Идёт построение матрицы смежности... ";

    for (int i = 0; i < AllClients.size(); i++) //прохожусь по всем клиентам (беру то что нужно)
    {
        for (int j = 0; j < AllClients.size(); j++)//опять прохожу по всем клиентам (беру то что есть)
        {
            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable.size() ==
                 AllClients[i].ClientHomeNeeded.ApartmentsNeeds.size()) && (AllClients[i].NameClient !=
                                                                            AllClients[j].NameClient)) //если колличество квартир совпадает и имя клиента не одно и тоже, тогда
            {
                //тогда мне надо сравнивать квартиры
                if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable.size() == 2)//если всего 2 квартиры, тогда
                {
                    //сначала сравню первые квартиры, потом уже вторые, так как они отсортированы, то будет проще
                    if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >=
                        AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms) //если в первой квартире колличество комнат больше
                    {
                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == true) ||
                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == false) &&
                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Balcony == false)))//сравниваю балконы
                        {
                            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == true) ||
                                ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == false) &&
                                 (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Phone ==
                                  false))) //сравниваю наличие телефона
                            {
                                //если всё ок, то теперь надо сравнить вторую хату
                                if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].NumberOfRooms >=
                                    AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].NumberOfRooms) //если в первой квартире колличество комнат больше
                                {
                                    if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Balcony == true) ||
                                        ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Balcony == false) &&
                                         (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].Balcony ==
                                          false)))//сравниваю балконы
                                    {
                                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Phone == true) ||
                                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Phone ==
                                              false) &&
                                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].Phone ==
                                              false))) //сравниваю наличие телефона
                                        {
                                            Matrix[i][j] = 1;
                                            continue;
                                        }
                                    }
                                }

                            }
                        }
                    }

                } else//иначе 1 квартира, тут проще
                {
                    if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >=
                        AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms)//если колличество комнат имеющихся больше или равно, тогда
                    {
                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == true) ||
                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == false) &&
                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Balcony == false)))//сравниваю балконы
                        {
                            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == true) ||
                                ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == false) &&
                                 (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Phone ==
                                  false))) //сравниваю наличие телефона
                            {
                                //если всё супер, то я ставлю метку что они могут обменяться
                                Matrix[i][j] = 1;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "УСПЕШНО" << std::endl;
    return Matrix;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<Client> AllClients = readData();
    std::vector<std::vector<int>> Matrix = StoringGraphUsingMatrix(AllClients);
    std::cout << "hello";
}


