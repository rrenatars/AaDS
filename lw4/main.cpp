#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>
#include <sstream>
#include <string>
#include <regex>
#include <queue>

/*
24. �  �����  ��������  ����������� �� ������ ����������� �
�������� ���������� ������, ���������� ��������� �� ����������
������,  ������� ������� � ��������.  ������� �������� �������
����� �������� �� 2 ������ ���� ��������. ��������� �� ������
�������  ���������� ������� ������.  ������������� �����������
���������� ����������� �������, � ������� ��������� ����� ����
������.  ��������� �������� ������ � ������� ����������� �����
����������� � ������ ������.  �������,  ��� ��������  ��������
����� ������� �� ��������� � �� ������������ �������� ��������
(11).

�����: �������� ������ ��-21
����� ����������: Visual Studio 2019
*/

//��������� ������� ����� ����� ��� ������� ������� �������
struct Apartment {
    int NumberOfRooms; //����������� ������
    bool Balcony; //������� �������
    bool Phone; //������� ��������
};
struct HouseAvailable {
    std::vector<Apartment> ApartmentsAvailable;
};
struct HomeNeeded {
    std::vector<Apartment> ApartmentsNeeds;
};
struct Client {
    std::string NameClient; //��� �������
    HouseAvailable ClientHouseAvailable;// �������� ������� ���� � �������
    HomeNeeded ClientHomeNeeded;// �������� ������� �������� �������
};

struct DateForViewing {
    int NumberOfEligibleClients;
    int IndexClientInVector;
};

std::string GetNameInputFile();

void TryOpenFile(std::string FilePath);

std::vector<Client> GetDateFromInputFile(std::string NameInputFile);

Client CreatingCustomerData(std::string LineFromFile);

std::string GetNameClientFromFileLine(std::string LineFromFile);

HouseAvailable GetHouseAvailableFromFileLine(std::string LineFromFile);

HomeNeeded GetHomesNeededFromFileLine(std::string LineFromFile);

int GetNumberOfRoomsFromCustomerData(std::string CustomerData);

bool GetTrueOrFalseBalconyFromCustomerData(std::string CustomerData);

bool GetTrueOrFalsePhoneFromCustomerData(std::string CustomerData);

struct Apartment CreateApartmentData(std::string CustomerData);

std::vector<std::vector<int>> StoringGraphUsingMatrix(std::vector<Client> AllClients);

void SortClientsApartmentsInAscendingOrder(std::vector<Client> &AllClients);

int GetNumberOfParticipants(std::vector<Client> AllClients);

void
DepthFirstSearch(std::vector<std::vector<int>> MatrixGraf, std::vector<Client> AllClients, DateForViewing ClientStart,
                 std::vector<int> Used, int Vertex, std::vector<std::string> &AllCycles);

void SortedCycles(std::vector<std::string> &AllCycles, DateForViewing ClientStart);

void SetLengthsCycles(std::vector<std::string> &AllCycles, std::map<std::string, int> &LengthsCycles);

void OutputingCycles(std::map<std::string, int> &LengthsCycles, int MaxLenghtCycles);

int GetMaxLenghtCycles(std::map<std::string, int> &LengthsCycles);

//������ � �������
std::string GetNameInputFile() //������� ��� ���������� ����� �������� �����
{
    std::string InputFilePath;
    std::cout << "������� ��� ����� ��� ���������� ������ � ��������: ";
    std::cin >> InputFilePath;
    return InputFilePath;
}

void TryOpenFile(std::string FilePath) //�������� �������� �����
{
    std::cout << "��� �������� �������� �����...";
    std::ifstream InputFile;
    InputFile.open(FilePath);
    if (!InputFile.is_open()) {
        std::cout << std::endl;
        std::cout << "!WARNING! ���� �� ��� ������, ��������� ������������ �������� �����..." << std::endl;
        system("pause");
        exit(-1);
    }
    std::cout << " �������" << std::endl;
    InputFile.close();
}

//���������� ������ �� ����� � ������������ ������ � ��������
std::vector<Client> GetDateFromInputFile(std::string NameInputFile)//������ ������ ���� ��������
{
    std::ifstream InputFile;
    std::vector<Client> Clients;
    std::string LineFromFile;
    InputFile.open(NameInputFile);

    while (!InputFile.eof()) //���� � ����� ���� ������
    {
        getline(InputFile, LineFromFile);
        if (LineFromFile == "") {
            break;
        }
        Clients.push_back(CreatingCustomerData(LineFromFile));
    }
    InputFile.close();
    return Clients;
}

Client CreatingCustomerData(std::string LineFromFile)//�������� ������ � ������ �������
{
    Client Client = {GetNameClientFromFileLine(LineFromFile), GetHouseAvailableFromFileLine(LineFromFile),
                     GetHomesNeededFromFileLine(LineFromFile)};
    return Client;
}

std::string GetNameClientFromFileLine(std::string LineFromFile) //�������� ��� �������
{
    std::string Result;

    std::regex RegSearchNameClient("[a-zA-z]+");//���� ���
    auto Words_begin = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                            RegSearchNameClient); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ���� ��������� ������
    {
        std::smatch match = *i;//��������� � ���������� ������ ��������� �����
        std::string Result = match.str();//�������� ��� � ������
        return Result;
    }
}

HouseAvailable GetHouseAvailableFromFileLine(std::string LineFromFile) //�������� ������
{
    HouseAvailable Result;

    std::string CustomerData;
    std::regex RegSearchHouseAvailable("[\\d \\+\\-]+");
    auto Words_begin = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                            RegSearchHouseAvailable); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������

    if (std::distance(Words_begin, Words_end) == 2) //���� �������� 2 ����������, �� � ���� ����� 1 �������� � �������
    {
        for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
            std::smatch match = *i;//��������� � ���������� ������ ��������� �����
            CustomerData = match.str();//�������� ��� � ������
            Result.ApartmentsAvailable.push_back(CreateApartmentData(CustomerData));
            return Result;
        }
    } else if (std::distance(Words_begin, Words_end) ==
               3) //���� ������� 3 ��������, �� ���� 2 ���� ���� 1; ���� 1 ���� � ���� 2
    {
        std::regex RegSearchHouseAvailable(",[\\d \\+\\-]+;");
        auto Words_begin_1 = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                                  RegSearchHouseAvailable); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
        auto Words_end_1 = std::sregex_iterator();//���������� ��������� �� ����� ���������

        if (std::distance(Words_begin_1, Words_end_1) == 1) //���� ����� ����� �������, �� � ���� ���� 2 ����
        {
            int index = 0;

            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ������� ���������� �����
            {
                std::smatch match = *i;//��������� � ���������� ������ ��������� �����
                CustomerData = match.str();//�������� ��� � ������
                Result.ApartmentsAvailable.push_back(CreateApartmentData(CustomerData));
                if (index == 1) {
                    break;
                }
                index++;
            }
            return Result;
        } else //���� �� �������, �� � ���� � ������� ������ 1 ��� � ���� ��������� �� 2
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
                std::smatch match = *i;//��������� � ���������� ������ ��������� �����
                CustomerData = match.str();//�������� ��� � ������
                Result.ApartmentsAvailable.push_back(CreateApartmentData(CustomerData));
                return Result;
            }
        }
    }
}

HomeNeeded GetHomesNeededFromFileLine(std::string LineFromFile) {
    HomeNeeded Result;

    std::string CustomerData;
    bool is_first = true;
    bool is_second = true;
    std::regex RegSearchHomesNeeded("[\\d \\+\\-]+");
    auto Words_begin = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                            RegSearchHomesNeeded); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������

    if (std::distance(Words_begin, Words_end) == 2) //���� �������� 2 ����������, �� � ���� ����� 1 �������� � �������
    {
        for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
            if (is_first) {
                is_first = false;
                continue;
            }
            std::smatch match = *i;//��������� � ���������� ������ ��������� �����
            CustomerData = match.str();//�������� ��� � ������
            Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
            return Result;
        }
    } else if (std::distance(Words_begin, Words_end) ==
               3) //���� ������� 3 ��������, �� ���� 2 ���� ���� 1; ���� 1 ���� � ���� 2
    {
        std::regex RegSearchHouseAvailable(",[\\d \\+\\-]+;");
        auto Words_begin_1 = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                                  RegSearchHouseAvailable); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
        auto Words_end_1 = std::sregex_iterator();//���������� ��������� �� ����� ���������

        if (std::distance(Words_begin_1, Words_end_1) ==
            1) //���� ����� ����� �������, �� � ���� ������ 1 ��� ��� ������ � 2 �
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ������� ���������� �����
            {
                if (is_first) {
                    is_first = false;
                    continue;
                }
                if (is_second) {
                    is_second = false;
                    continue;
                }
                std::smatch match = *i;//��������� � ���������� ������ ��������� �����
                CustomerData = match.str();//�������� ��� � ������
                Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
                return Result;
            }
        } else //���� �� �������, �� � ���� � ������� ������ 1 ��� � ���� ��������� �� 2
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
                if (is_first) {
                    is_first = false;
                    continue;
                }
                std::smatch match = *i;//��������� � ���������� ������ ��������� �����
                CustomerData = match.str();//�������� ��� � ������
                Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
            }
            return Result;
        }
    }
}

int GetNumberOfRoomsFromCustomerData(std::string CustomerData) //���� � ��������� ������ � ����������� ������
{
    std::regex RegSearchDigit("[\\d]+");//��� �����
    auto Words_begin = std::sregex_iterator(CustomerData.begin(), CustomerData.end(),
                                            RegSearchDigit); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ���� ��������� ������
    {
        std::smatch match = *i;//��������� � ���������� ������ ��������� �����
        int Result = stoi(match.str());//�������� ��� � ������
        return Result;
    }
}

bool GetTrueOrFalseBalconyFromCustomerData(std::string CustomerData) {
    std::regex RegSearchDateAboutBalcon("[\\+\\-]+");
    auto Words_begin = std::sregex_iterator(CustomerData.begin(), CustomerData.end(),
                                            RegSearchDateAboutBalcon); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ���� ��������� ������
    {
        std::smatch match = *i;//��������� � ���������� ������ ��������� �����
        if (match.str() == "+") {
            return true;
        } else {
            return false;
        }
    }
}

bool GetTrueOrFalsePhoneFromCustomerData(std::string CustomerData) {
    std::regex RegSearchDateAboutPhone("[\\+\\-]+");
    auto Words_begin = std::sregex_iterator(CustomerData.begin(), CustomerData.end(),
                                            RegSearchDateAboutPhone); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
    auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������
    bool is_first = true;

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //��������� �� ���� ��������� ������
    {
        std::smatch match = *i;//��������� � ���������� ������ ��������� �����

        if (is_first) {
            is_first = false;
            continue;
        }
        if (match.str() == "+") {
            return true;
        } else {
            return false;
        }
    }
}

Apartment CreateApartmentData(std::string CustomerData) {
    Apartment Apart = {GetNumberOfRoomsFromCustomerData(CustomerData),
                       GetTrueOrFalseBalconyFromCustomerData(CustomerData),
                       GetTrueOrFalsePhoneFromCustomerData(CustomerData)};
    return Apart;
}

//���������� ��������
void SortClientsApartmentsInAscendingOrder(std::vector<Client> &AllClients) {
    std::cout << "��� ���������� ������� �� �����������... ";
    for (int i = 0; i < AllClients.size(); i++)//��������� �� ���� ��������
    {
        if (AllClients[i].ClientHouseAvailable.ApartmentsAvailable.size() == 2)//���� � ������� ���� 2 ����, ��
        {
            if (AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1].NumberOfRooms) //���� ����������� ������ � 1 �������� ������ ��� �� ������, �����
            {
                Apartment tmp = AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0];
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0] = AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1];
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1] = tmp;
            }
        }
        if (AllClients[i].ClientHomeNeeded.ApartmentsNeeds.size() == 2) //���� ������� ����� 2 ����, ��
        {
            if (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms >
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].NumberOfRooms) {
                Apartment tmp = AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0];
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0] = AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1];
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1] = tmp;
            }
        }
    }
    std::cout << "�������" << std::endl;
}

//�������� ����� � ���� �������:
std::vector<std::vector<int>> StoringGraphUsingMatrix(std::vector<Client> AllClients) {
    std::vector<std::vector<int>> Matrix(AllClients.size(), std::vector<int>(
            AllClients.size()));//������ ������� N*N ��� N ����������� ��������
    std::cout << "��� ���������� ������� ���������... ";

    for (int i = 0; i < AllClients.size(); i++) //��������� �� ���� �������� (���� �� ��� �����)
    {
        for (int j = 0; j < AllClients.size(); j++)//����� ������� �� ���� �������� (���� �� ��� ����)
        {
            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable.size() ==
                 AllClients[i].ClientHomeNeeded.ApartmentsNeeds.size()) && (AllClients[i].NameClient !=
                                                                            AllClients[j].NameClient)) //���� ����������� ������� ��������� � ��� ������� �� ���� � ����, �����
            {
                //����� ��� ���� ���������� ��������
                if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable.size() == 2)//���� ����� 2 ��������, �����
                {
                    //������� ������ ������ ��������, ����� ��� ������, ��� ��� ��� �������������, �� ����� �����
                    if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >=
                        AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms) //���� � ������ �������� ����������� ������ ������
                    {
                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == true) ||
                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == false) &&
                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Balcony == false)))//��������� �������
                        {
                            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == true) ||
                                ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == false) &&
                                 (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Phone ==
                                  false))) //��������� ������� ��������
                            {
                                //���� �� ��, �� ������ ���� �������� ������ ����
                                if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].NumberOfRooms >=
                                    AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].NumberOfRooms) //���� � ������ �������� ����������� ������ ������
                                {
                                    if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Balcony == true) ||
                                        ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Balcony == false) &&
                                         (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].Balcony ==
                                          false)))//��������� �������
                                    {
                                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Phone == true) ||
                                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Phone == false) &&
                                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].Phone ==
                                              false))) //��������� ������� ��������
                                        {
                                            Matrix[i][j] = 1;
                                            continue;
                                        }
                                    }
                                }

                            }
                        }
                    }

                } else//����� 1 ��������, ��� �����
                {
                    if (AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >=
                        AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms)//���� ����������� ������ ��������� ������ ��� �����, �����
                    {
                        if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == true) ||
                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Balcony == false) &&
                             (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Balcony == false)))//��������� �������
                        {
                            if ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == true) ||
                                ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[0].Phone == false) &&
                                 (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].Phone ==
                                  false))) //��������� ������� ��������
                            {
                                //���� �� �����, �� � ������ ����� ��� ��� ����� ����������
                                Matrix[i][j] = 1;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "�������" << std::endl;
    return Matrix;
}

//����� ������ ������ ��������� ��� ���������� �����
int GetIndexNameFromUser(std::vector<Client> AllClients)//�������� ��� �������
{
    std::string tmpName;
    bool is_exist = false;
    std::cout << "������� ��� ������� �������� ������ ���������: ";
    std::cin >> tmpName;

    while (!is_exist)//���� ��� �� ����������
    {
        for (int i = 0; i < AllClients.size(); i++) {
            if (AllClients[i].NameClient == tmpName) {
                is_exist = true;
                break;
            }
        }
        if (!is_exist) //����
        {
            tmpName = "";
            std::cout << "��� ������� �� ������� � ���� ������, ������� ������� �������: ";
            std::getline(std::cin, tmpName);
        }
    }
    int Result = 0;
    for (int i = 0; i < AllClients.size(); i++) {
        if (AllClients[i].NameClient == tmpName) {
            Result = i;
            break;
        }

    }
    return Result;
}

int GetNumberOfParticipants(std::vector<Client> AllClients) {
    int Result;
    std::cout << "������� ������������ ����������� ������ � ������: ";
    std::cin >> Result;
    std::cout << std::endl;

    if (Result > AllClients.size()) //���� ����� �������, �� � ������ ����������� ��� � ���� ��� ��� ����� ������������
    {
        Result = AllClients.size();
    }
    return Result;
}

//������ � ������
void
DepthFirstSearch(std::vector<std::vector<int>> MatrixGraf, std::vector<Client> AllClients, DateForViewing ClientStart,
                 std::vector<int> Used, int Vertex, std::vector<std::string> &AllCycles) {
    Used[Vertex] = 1; //������ ��� ��� ������� ��������� (� ������ ��� ������, ��� ������� ������� � ������� ��� ������� ��������

    for (int i = 0; i < AllClients.size(); i++) {
        if ((Used[i] == 0) && (MatrixGraf[Vertex][i] ==
                               1)) //���� ������� �� �������� � ������� ����� ��������� � ��� ���� ����� � ���� ��������, �� ����� ��� ����
        {
            //���������� ������ �������
            AllCycles[AllCycles.size() - 1] += (AllClients[i].NameClient + "->");

            if (MatrixGraf[i][ClientStart.IndexClientInVector] ==
                1) // ���� ���������� ���� �� ������� � ����������� �������
            {
                //� ���� ����, �� �������� ����� � ������� � ������ �����
                AllCycles.push_back(AllCycles[AllCycles.size() - 1]);
                AllCycles[AllCycles.size() - 2] += AllClients[ClientStart.IndexClientInVector].NameClient;
            }
            DepthFirstSearch(MatrixGraf, AllClients, ClientStart, Used, i, AllCycles);
            //���� � �������� � ������� �������, �� ��� ���� ������� ��������� ��� �
            std::regex SearchLastArrowAndName("[a-zA-Z]+->$");
            AllCycles[AllCycles.size() - 1] = std::regex_replace(AllCycles[AllCycles.size() - 1],
                                                                 SearchLastArrowAndName, "");//������ ���������� �������
        }
    }
}

void SortedCycles(std::vector<std::string> &AllCycles, DateForViewing ClientStart) {
    std::regex SearchLastArrowAndName("[a-zA-Z]+$");
    std::cmatch m;
    for (int i = 0; i < AllCycles.size(); i++) // ��� ������ ����������, ��� �������� �������, ������� ����� �� �������
    {
        if (std::regex_search(AllCycles[i].c_str(), m,
                              SearchLastArrowAndName)) //���� �� ������� ����� �� �������. �� �����
        {
            continue;
        } else {
            //���� ���� �� ��������, ��� ������ �� ������
            AllCycles.erase(AllCycles.begin() + i);
            i--;
        }
    }
    if (AllCycles.size() == 0) {
        std::cout
                << "                                        ��� ������� ������� ��� ���������� �������                                      "
                << std::endl;
        std::cout
                << "------------------------------------------------------------------------------------------------------------------------"
                << std::endl;
        system("pause");
        exit(-1);

    }
    //������ ���� ����������: ��� ���� ������ �� ������ �� ����� ���������� � �����, ��� ���������
    std::regex AllNameInCycles("[a-zA-Z]+");
    for (int i = 0;
         i < AllCycles.size(); i++) //��� ������ ����������, ��� �������� ������ � ������� ������ ��������, ��� ��������
    {
        auto Words_begin = std::sregex_iterator(AllCycles[i].begin(), AllCycles[i].end(),
                                                AllNameInCycles); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
        auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������
        if ((std::distance(Words_begin, Words_end) - 1) > ClientStart.NumberOfEligibleClients) {
            AllCycles.erase(AllCycles.begin() + i);
            i--;
        }
    }
    if (AllCycles.size() == 0) {
        std::cout
                << "                                        ��� ������� ������� ��� ���������� �������                                      "
                << std::endl;
        std::cout
                << "------------------------------------------------------------------------------------------------------------------------"
                << std::endl;
        system("pause");
        exit(-1);

    }
}

void SetLengthsCycles(std::vector<std::string> &AllCycles,
                      std::map<std::string, int> &LengthsCycles) //������ ������� ����� ���� �������
{
    std::regex AllNameInCycles("[a-zA-Z]+");
    for (int i = 0; i < AllCycles.size(); i++) //��������� �� ���� ������
    {
        auto Words_begin = std::sregex_iterator(AllCycles[i].begin(), AllCycles[i].end(),
                                                AllNameInCycles); //������ ����������, ���� ���������, ������� ����� ������� � ���� ��� ���������� �����
        auto Words_end = std::sregex_iterator();//���������� ��������� �� ����� ���������
        LengthsCycles[AllCycles[i]] = (std::distance(Words_begin, Words_end) - 1);
    }
}

int GetMaxLenghtCycles(std::map<std::string, int> &LengthsCycles) {
    std::map<std::string, int>::iterator it = LengthsCycles.begin();
    int Result = it->second;
    for (it = LengthsCycles.begin(); it != LengthsCycles.end(); it++) {
        if (Result < it->second) {
            Result = it->second;
        }
    }
    return Result;
}

void OutputingCycles(std::map<std::string, int> &LengthsCycles, int MaxLenghtCycles) {
    int Start = 2;
    while (Start != MaxLenghtCycles + 1) {
        for (auto it: LengthsCycles) {
            if (it.second == Start) {
                std::cout
                        << "------------------------------------------------------------------------------------------------------------------------"
                        << std::endl;
                std::cout << it.first << std::endl;
                std::cout
                        << "------------------------------------------------------------------------------------------------------------------------"
                        << std::endl;
            }
        }
        Start++;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string NameInputFile = GetNameInputFile(); //�������� ��� �����
    TryOpenFile(NameInputFile);//�������� ����������� �������� �����
    std::vector<Client> AllClients = GetDateFromInputFile(NameInputFile);//�������� ������ ���������
    SortClientsApartmentsInAscendingOrder(AllClients);//�������� �������� � ������� �����������
    std::vector<std::vector<int>> MatrixGraf = StoringGraphUsingMatrix(AllClients); //������ ������� ���������

    DateForViewing ClientStart;

    ClientStart.IndexClientInVector = GetIndexNameFromUser(AllClients);
    ClientStart.NumberOfEligibleClients = GetNumberOfParticipants(AllClients);

    std::vector<int> Used(AllClients.size(), 0);
    int Vertex = ClientStart.IndexClientInVector;
    std::vector<std::string> AllCycles;

    AllCycles.push_back(AllClients[Vertex].NameClient + "->");

    std::cout << "��� ������� ���������� ���� ��������� ������� � �������� " << AllClients[Vertex].NameClient
              << " ...";
    DepthFirstSearch(MatrixGraf, AllClients, ClientStart, Used, Vertex, AllCycles); //����� ���������� ���� ������
    std::cout << " �������" << std::endl;

    std::cout
            << "------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
    std::cout << "                                         ������������ ������ � �������� "
              << AllClients[Vertex].NameClient << "         " << std::endl;
    std::cout
            << "------------------------------------------------------------------------------------------------------------------------"
            << std::endl;

    //������ ��� ���� ������������� ��������� �����
    SortedCycles(AllCycles, ClientStart);

    std::map<std::string, int> LengthsCycles;
    SetLengthsCycles(AllCycles, LengthsCycles);

    int MaxLenghtCycles = GetMaxLenghtCycles(LengthsCycles);

    OutputingCycles(LengthsCycles, MaxLenghtCycles);


    return 0;
}