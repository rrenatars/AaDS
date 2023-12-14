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
24. В  файле  записаны  предложения по обмену жилплощадью в
пределах некоторого города, включающие пожелания по количеству
комнат,  наличию балкона и телефона.  Имеются варианты размена
одной квартиры на 2 других либо наоборот. Требуется по заявке
клиента  предложить способы обмена.  Предусмотреть возможность
нахождения циклических обменов, в которых участвуют более двух
сторон.  Найденные варианты выдать в порядке возрастания числа
участвующих в обмене сторон.  Считать,  что клиентам  подходят
более хорошие по сравнению с их требованиями варианты переезда
(11).

Автор: Шестаков Андрей БИ-21
Среда разработки: Visual Studio 2019
*/

//структуры которые легко дадут мне описать каждого клиента
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

//работа с файлами
std::string GetNameInputFile() //функция для считывания имени входного файла
{
    std::string InputFilePath;
    std::cout << "Введите имя файла для считывания данных о клиентах: ";
    std::cin >> InputFilePath;
    return InputFilePath;
}

void TryOpenFile(std::string FilePath) //проверка открытия файла
{
    std::cout << "Идёт проверка открытия файла...";
    std::ifstream InputFile;
    InputFile.open(FilePath);
    if (!InputFile.is_open()) {
        std::cout << std::endl;
        std::cout << "!WARNING! Файл не был открыт, проверьте правильность названия файла..." << std::endl;
        system("pause");
        exit(-1);
    }
    std::cout << " УСПЕШНО" << std::endl;
    InputFile.close();
}

//считывание данных из файла и формирование данных о клиентах
std::vector<Client> GetDateFromInputFile(std::string NameInputFile)//создаю вектор всех клиентов
{
    std::ifstream InputFile;
    std::vector<Client> Clients;
    std::string LineFromFile;
    InputFile.open(NameInputFile);

    while (!InputFile.eof()) //пока в файле есть данные
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

Client CreatingCustomerData(std::string LineFromFile)//заполняю данные о каждом клиенте
{
    Client Client = {GetNameClientFromFileLine(LineFromFile), GetHouseAvailableFromFileLine(LineFromFile),
                     GetHomesNeededFromFileLine(LineFromFile)};
    return Client;
}

std::string GetNameClientFromFileLine(std::string LineFromFile) //считываю имя клиента
{
    std::string Result;

    std::regex RegSearchNameClient("[a-zA-z]+");//ищет имя
    auto Words_begin = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                            RegSearchNameClient); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по всем найденным словам
    {
        std::smatch match = *i;//записываю в переменную первое найденное слово
        std::string Result = match.str();//перевожу его в строку
        return Result;
    }
}

HouseAvailable GetHouseAvailableFromFileLine(std::string LineFromFile) //считываю данные
{
    HouseAvailable Result;

    std::string CustomerData;
    std::regex RegSearchHouseAvailable("[\\d \\+\\-]+");
    auto Words_begin = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                            RegSearchHouseAvailable); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора

    if (std::distance(Words_begin, Words_end) == 2) //если найденно 2 совпадения, то у меня точно 1 квартира в наличии
    {
        for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
            std::smatch match = *i;//записываю в переменную первое найденное слово
            CustomerData = match.str();//перевожу его в строку
            Result.ApartmentsAvailable.push_back(CreateApartmentData(CustomerData));
            return Result;
        }
    } else if (std::distance(Words_begin, Words_end) ==
               3) //если описано 3 квартиры, то либо 2 есть надо 1; либо 1 есть и надо 2
    {
        std::regex RegSearchHouseAvailable(",[\\d \\+\\-]+;");
        auto Words_begin_1 = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                                  RegSearchHouseAvailable); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
        auto Words_end_1 = std::sregex_iterator();//переменная указывает на конец итератора

        if (std::distance(Words_begin_1, Words_end_1) == 1) //если такая штука найдена, то у меня есть 2 хаты
        {
            int index = 0;

            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по первому найденному слову
            {
                std::smatch match = *i;//записываю в переменную первое найденное слово
                CustomerData = match.str();//перевожу его в строку
                Result.ApartmentsAvailable.push_back(CreateApartmentData(CustomerData));
                if (index == 1) {
                    break;
                }
                index++;
            }
            return Result;
        } else //если не найдена, то у меня в наличие только 1 дом и надо разменять на 2
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
                std::smatch match = *i;//записываю в переменную первое найденное слово
                CustomerData = match.str();//перевожу его в строку
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
                                            RegSearchHomesNeeded); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора

    if (std::distance(Words_begin, Words_end) == 2) //если найденно 2 совпадения, то у меня точно 1 квартира в наличии
    {
        for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
            if (is_first) {
                is_first = false;
                continue;
            }
            std::smatch match = *i;//записываю в переменную первое найденное слово
            CustomerData = match.str();//перевожу его в строку
            Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
            return Result;
        }
    } else if (std::distance(Words_begin, Words_end) ==
               3) //если описано 3 квартиры, то либо 2 есть надо 1; либо 1 есть и надо 2
    {
        std::regex RegSearchHouseAvailable(",[\\d \\+\\-]+;");
        auto Words_begin_1 = std::sregex_iterator(LineFromFile.begin(), LineFromFile.end(),
                                                  RegSearchHouseAvailable); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
        auto Words_end_1 = std::sregex_iterator();//переменная указывает на конец итератора

        if (std::distance(Words_begin_1, Words_end_1) ==
            1) //если такая штука найдена, то у меня только 1 дом для поиска и 2 в
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по первому найденному слову
            {
                if (is_first) {
                    is_first = false;
                    continue;
                }
                if (is_second) {
                    is_second = false;
                    continue;
                }
                std::smatch match = *i;//записываю в переменную первое найденное слово
                CustomerData = match.str();//перевожу его в строку
                Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
                return Result;
            }
        } else //если не найдена, то у меня в наличие только 1 дом и надо разменять на 2
        {
            for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) {
                if (is_first) {
                    is_first = false;
                    continue;
                }
                std::smatch match = *i;//записываю в переменную первое найденное слово
                CustomerData = match.str();//перевожу его в строку
                Result.ApartmentsNeeds.push_back(CreateApartmentData(CustomerData));
            }
            return Result;
        }
    }
}

int GetNumberOfRoomsFromCustomerData(std::string CustomerData) //ищет в подстроке данные о колличестве комнат
{
    std::regex RegSearchDigit("[\\d]+");//ищу цифру
    auto Words_begin = std::sregex_iterator(CustomerData.begin(), CustomerData.end(),
                                            RegSearchDigit); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по всем найденным словам
    {
        std::smatch match = *i;//записываю в переменную первое найденное слово
        int Result = stoi(match.str());//перевожу его в строку
        return Result;
    }
}

bool GetTrueOrFalseBalconyFromCustomerData(std::string CustomerData) {
    std::regex RegSearchDateAboutBalcon("[\\+\\-]+");
    auto Words_begin = std::sregex_iterator(CustomerData.begin(), CustomerData.end(),
                                            RegSearchDateAboutBalcon); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по всем найденным словам
    {
        std::smatch match = *i;//записываю в переменную первое найденное слово
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
                                            RegSearchDateAboutPhone); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
    auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора
    bool is_first = true;

    for (std::sregex_iterator i = Words_begin; i != Words_end; ++i) //прохожусь по всем найденным словам
    {
        std::smatch match = *i;//записываю в переменную первое найденное слово

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

//сортировка квартиры
void SortClientsApartmentsInAscendingOrder(std::vector<Client> &AllClients) {
    std::cout << "Идёт сортировка квартир по возрастанию... ";
    for (int i = 0; i < AllClients.size(); i++)//прохожусь по всем клиентам
    {
        if (AllClients[i].ClientHouseAvailable.ApartmentsAvailable.size() == 2)//если у клиента есть 2 дома, то
        {
            if (AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0].NumberOfRooms >
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1].NumberOfRooms) //если колличество комнат в 1 квартире больше чем во второй, тогда
            {
                Apartment tmp = AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0];
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[0] = AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1];
                AllClients[i].ClientHouseAvailable.ApartmentsAvailable[1] = tmp;
            }
        }
        if (AllClients[i].ClientHomeNeeded.ApartmentsNeeds.size() == 2) //если клиенту нужно 2 дома, то
        {
            if (AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0].NumberOfRooms >
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1].NumberOfRooms) {
                Apartment tmp = AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0];
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[0] = AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1];
                AllClients[i].ClientHomeNeeded.ApartmentsNeeds[1] = tmp;
            }
        }
    }
    std::cout << "УСПЕШНО" << std::endl;
}

//хранение графа в виде матрицы:
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
                                            ((AllClients[j].ClientHouseAvailable.ApartmentsAvailable[1].Phone == false) &&
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

//прошу ввести нужные параметры для запускания цикла
int GetIndexNameFromUser(std::vector<Client> AllClients)//считываю имя клиента
{
    std::string tmpName;
    bool is_exist = false;
    std::cout << "Введите имя клиента которого хотите проверить: ";
    std::cin >> tmpName;

    while (!is_exist)//пока его не существует
    {
        for (int i = 0; i < AllClients.size(); i++) {
            if (AllClients[i].NameClient == tmpName) {
                is_exist = true;
                break;
            }
        }
        if (!is_exist) //если
        {
            tmpName = "";
            std::cout << "Имя клиента не найдено в базе данных, введите другого клиента: ";
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
    std::cout << "Введите максимальное колличество сторон в сделке: ";
    std::cin >> Result;
    std::cout << std::endl;

    if (Result > AllClients.size()) //если число большое, то я просто приравниваю его к тому что все могут учавствовать
    {
        Result = AllClients.size();
    }
    return Result;
}

//работа с графом
void
DepthFirstSearch(std::vector<std::vector<int>> MatrixGraf, std::vector<Client> AllClients, DateForViewing ClientStart,
                 std::vector<int> Used, int Vertex, std::vector<std::string> &AllCycles) {
    Used[Vertex] = 1; //говорю что эта вершина проверена (в первый раз говорю, что посетил вершину с которой мне сказали налинать

    for (int i = 0; i < AllClients.size(); i++) {
        if ((Used[i] == 0) && (MatrixGraf[Vertex][i] ==
                               1)) //если вершина не посещена с помощью этого алгоритма и там есть связь с этой вершиной, то тогда иду туда
        {
            //запечятляю данный переход
            AllCycles[AllCycles.size() - 1] += (AllClients[i].NameClient + "->");

            if (MatrixGraf[i][ClientStart.IndexClientInVector] ==
                1) // надо посмотреть есть ли переход в изначальную вершину
            {
                //и если есть, то завершаю линию в векторе и создаю новую
                AllCycles.push_back(AllCycles[AllCycles.size() - 1]);
                AllCycles[AllCycles.size() - 2] += AllClients[ClientStart.IndexClientInVector].NameClient;
            }
            DepthFirstSearch(MatrixGraf, AllClients, ClientStart, Used, i, AllCycles);
            //если я вернулся с вершины обратно, то мне надо удалить последнее имя и
            std::regex SearchLastArrowAndName("[a-zA-Z]+->$");
            AllCycles[AllCycles.size() - 1] = std::regex_replace(AllCycles[AllCycles.size() - 1],
                                                                 SearchLastArrowAndName, "");//удаляю последнего клиента
        }
    }
}

void SortedCycles(std::vector<std::string> &AllCycles, DateForViewing ClientStart) {
    std::regex SearchLastArrowAndName("[a-zA-Z]+$");
    std::cmatch m;
    for (int i = 0; i < AllCycles.size(); i++) // Эта первая сортировка, для удаления лишнего, которое точно не подойдёт
    {
        if (std::regex_search(AllCycles[i].c_str(), m,
                              SearchLastArrowAndName)) //если он находит такое не находит. то тогда
        {
            continue;
        } else {
            //этот цикл не подходит, его удаляю из списка
            AllCycles.erase(AllCycles.begin() + i);
            i--;
        }
    }
    if (AllCycles.size() == 0) {
        std::cout
                << "                                        Для данного клиента нет подходящих обменов                                      "
                << std::endl;
        std::cout
                << "------------------------------------------------------------------------------------------------------------------------"
                << std::endl;
        system("pause");
        exit(-1);

    }
    //второй этап сортировки: мне надо понять не больше ли число участников в цикле, чем дозволено
    std::regex AllNameInCycles("[a-zA-Z]+");
    for (int i = 0;
         i < AllCycles.size(); i++) //Это вторая сортировка, для удаления циклов с большим числом клиентов, чем заданное
    {
        auto Words_begin = std::sregex_iterator(AllCycles[i].begin(), AllCycles[i].end(),
                                                AllNameInCycles); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
        auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора
        if ((std::distance(Words_begin, Words_end) - 1) > ClientStart.NumberOfEligibleClients) {
            AllCycles.erase(AllCycles.begin() + i);
            i--;
        }
    }
    if (AllCycles.size() == 0) {
        std::cout
                << "                                        Для данного клиента нет подходящих обменов                                      "
                << std::endl;
        std::cout
                << "------------------------------------------------------------------------------------------------------------------------"
                << std::endl;
        system("pause");
        exit(-1);

    }
}

void SetLengthsCycles(std::vector<std::string> &AllCycles,
                      std::map<std::string, int> &LengthsCycles) //говорю сколько какой цикл длинной
{
    std::regex AllNameInCycles("[a-zA-Z]+");
    for (int i = 0; i < AllCycles.size(); i++) //прохожусь по всем циклам
    {
        auto Words_begin = std::sregex_iterator(AllCycles[i].begin(), AllCycles[i].end(),
                                                AllNameInCycles); //создаю переменную, типа итератора, которая будет хранить в себе все найдненные слова
        auto Words_end = std::sregex_iterator();//переменная указывает на конец итератора
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

    std::string NameInputFile = GetNameInputFile(); //считываю имя файла
    TryOpenFile(NameInputFile);//вроверка возможности открытия файла
    std::vector<Client> AllClients = GetDateFromInputFile(NameInputFile);//заполняю вектор клиентами
    SortClientsApartmentsInAscendingOrder(AllClients);//сортирую квартиры в порядке возрастания
    std::vector<std::vector<int>> MatrixGraf = StoringGraphUsingMatrix(AllClients); //создаю матрицу смежности

    DateForViewing ClientStart;

    ClientStart.IndexClientInVector = GetIndexNameFromUser(AllClients);
    ClientStart.NumberOfEligibleClients = GetNumberOfParticipants(AllClients);

    std::vector<int> Used(AllClients.size(), 0);
    int Vertex = ClientStart.IndexClientInVector;
    std::vector<std::string> AllCycles;

    AllCycles.push_back(AllClients[Vertex].NameClient + "->");

    std::cout << "Идёт процесс нахождение всех возможных обменов с клиентом " << AllClients[Vertex].NameClient
              << " ...";
    DepthFirstSearch(MatrixGraf, AllClients, ClientStart, Used, Vertex, AllCycles); //здесь нахождение всех циклов
    std::cout << " УСПЕШНО" << std::endl;

    std::cout
            << "------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
    std::cout << "                                         Всевозможные обмены с клиентом "
              << AllClients[Vertex].NameClient << "         " << std::endl;
    std::cout
            << "------------------------------------------------------------------------------------------------------------------------"
            << std::endl;

    //теперь мне надо отсортировать найденные циклы
    SortedCycles(AllCycles, ClientStart);

    std::map<std::string, int> LengthsCycles;
    SetLengthsCycles(AllCycles, LengthsCycles);

    int MaxLenghtCycles = GetMaxLenghtCycles(LengthsCycles);

    OutputingCycles(LengthsCycles, MaxLenghtCycles);


    return 0;
}