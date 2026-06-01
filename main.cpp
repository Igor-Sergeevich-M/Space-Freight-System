#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <windows.h> // Добавили для корректной работы кодировки и очистки в Windows
#include "Cargo.h"
#include "Station.h"
#include "Ship.h"

using json = nlohmann::json;

const double INF = 1e9;

// Функция вывода красивой шапки
void title() {
#ifdef _WIN32
    // Настраиваем кодировку консоли Windows на UTF-8 перед каждым выводом
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n";
    std::cout << "______________________________________________________________________________________________\n";
    std::cout << "----------------------------------------------------------------------------------------------\n";
    std::cout << "      ********************                                          ********************\n";
    std::cout << "     ********************            SPACE FREIGHT SYSTEM            ********************\n";
    std::cout << "      ********************                                          ********************\n";
    std::cout << "----------------------------------------------------------------------------------------------\n";
    std::cout << "______________________________________________________________________________________________\n";
    std::cout << "\n";
}

double shipCost(const Ship& ship, std::vector<Station> path){

    double parkingPricePerGab=1.0;
    double fuelPricePerGallon=100.0;

    int numberStations=path.size();
    int parkingPrice=parkingPricePerGab*ship.maxGab;
    int totalParkingPrice=parkingPrice*numberStations;
    double totalFuelPrice=multiDistance(path)*ship.fuelEfficiency*fuelPricePerGallon;
    double totalShipCost=totalFuelPrice+totalParkingPrice;
    return totalShipCost;
}
double allShipsPrice(const vector<Ship>& ships, const vector<Station>& stations, const Station& From, const Station& To, double INF){
    double totalCost=0;
    int n=ships.size();
    for(int i=0; i<n; i++){
        totalCost=totalCost+shipCost(ships[i], PathFinder(stations, From, To, fuelDistance(ships[i]), INF));
    }
    return totalCost;
}
double shipTime(const Ship& ship, std::vector<Station> path){
    return (365.0/ship.maxSpeed)*multiDistance(path);
}
double totalShipTime(const vector<Ship>& ships, const vector<Station>& stations, const Station& From, const Station& To, double INF){
    double maxTime=0.0;
    int n=ships.size();
    for(int i=0; i<n; i++){
        double curTime=shipTime(ships[i], PathFinder(stations, From, To, fuelDistance(ships[i]), INF));
        if (maxTime<curTime) maxTime=curTime;
    }
    return maxTime;
}
void printTableHead() {
    std::cout << "┌" << std::string(62, '-') << "┬" << std::string(13, '-') << "┬" << std::string(11, '-') << "┐\n";
    
    std::cout << "| " << "СОСТАВ КОСМИЧЕСКОГО ФЛОТА" << std::string(35, ' ') << " | "
              << "ЦЕНА (кред)" << " | "
              << "ВРЕМЯ (д)"  << " |\n";
              
    std::cout << "├" << std::string(62, '-') << "┼" << std::string(13, '-') << "┼" << std::string(11, '-') << "┤\n";
}

void infoOut(const vector<Ship>& ships, const vector<Station>& stations, const Station& From, const Station& To, double INF){
    if (ships.empty()) {
        std::cout << "| " << std::setw(60) << std::left  << "Нет доступных кораблей" << " | " 
                  << std::setw(11) << std::right << "0.00" << " | " 
                  << std::setw(9)  << std::right << "0.00" << " |\n";
        std::cout << "├" << std::string(62, '-') << "┼" << std::string(13, '-') << "┼" << std::string(11, '-') << "┤\n";
        return;
    }
    
    int n = ships.size();
    std::string shipsOut = ships[0].shipName;
    int count = 1;
    
    for(int i = 1; i < n; i++){
        if (ships[i].shipName == ships[i-1].shipName){ 
            count++;
        } else if (count == 1){ 
            shipsOut.append(", "); 
            shipsOut.append(ships[i].shipName);
        } else {
            shipsOut.append("x");
            shipsOut.append(std::to_string(count));
            shipsOut.append(", ");
            shipsOut.append(ships[i].shipName);
            count = 1;
        }
        if (i == n-1 && count != 1){
            shipsOut.append("x");
            shipsOut.append(std::to_string(count));
        }
    }
    std::cout << "| " << std::setw(60) << std::left  << shipsOut << " | ";
    std::cout << std::setw(11) << std::right << std::fixed << std::setprecision(2) << allShipsPrice(ships, stations, From, To, INF) << " | ";
    std::cout << std::setw(9)  << std::right << std::fixed << std::setprecision(2) << totalShipTime(ships, stations, From, To, INF) << " |\n";
    
    std::cout << "├" << std::string(62, '-') << "┼" << std::string(13, '-') << "┼" << std::string(11, '-') << "┤\n";
}
void sortShipsByCost(std::vector<std::vector<Ship>>& shipsX2, const vector<Station> stations, const Station& From, const Station& To, double INF) {
    int n = shipsX2.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Сбрасываем флаг в начале каждого прохода
        for (int j = 0; j < n - i - 1; j++) {
            if (allShipsPrice(shipsX2[j], stations, From, To, INF)>allShipsPrice(shipsX2[j+1], stations, From, To, INF)) {
                std::swap(shipsX2[j], shipsX2[j + 1]); 
                swapped = true; // Запоминаем, что перестановка была
            }
        }
        // Если перестановок не было — массив готов, выходим раньше времени!
        if (!swapped) {
            break;
        }
    }
}
void deleteBadShips(std::vector<Ship>& ships, const std::vector<Station>& stations, const Station& From, const Station& To, double INF){
    ships.erase(
        std::remove_if(ships.begin(), ships.end(), [&](const Ship& ship) {
            // Если размер пути меньше либо равен 1, значит, долететь нельзя — удаляем корабль
            return PathFinder(stations, From, To, fuelDistance(ship), INF).size() <= 1;
        }), 
        ships.end()
    );
}
std::vector<std::vector<Ship>> cargoSort(const vector<Ship> ships, const vector<Station> stations, const vector<Cargo> cargos, const Station& From, const Station& To, double INF){
    int n=cargos.size();
    std::vector<std::vector<Ship>> allVariants;
    std::vector<Cargo> maxMass=cargos, maxGab=cargos;
    sortCargoMass(maxMass);
    sortCargoGab(maxGab);
    std::vector<Ship> allowedShips=ships;
    deleteBadShips(allowedShips, stations, From, To, INF);
    std::vector<Ship> maxMassShips=allowedShips, maxGabShips=allowedShips;
    sortShipMass(maxMassShips);
    sortShipGab(maxGabShips);
    std::vector<Ship> goShips;
    Ship minAllovedMassShip=maxMassShips[0];
    int minMassIdx;
    for(int i=0; i<maxMassShips.size(); i++){
        if (maxMass[0].cargoMass<=maxMassShips[i].maxMass){
            if(minAllovedMassShip.maxMass>maxMassShips[i].maxMass){
                minAllovedMassShip.maxMass=maxMassShips[i].maxMass;
                minMassIdx=i;
             }
        }
    }
    int massIn=0;
    for(int i=0; i<n;i++){
        if (massIn<maxMass[i].cargoMass){
            massIn=maxMassShips[minMassIdx].maxMass;
            goShips.push_back(maxMassShips[minMassIdx]);
        }
        massIn=massIn-maxMass[i].cargoMass;
    }
    allVariants.push_back(goShips);
    return allVariants;
}
int main() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif

    std::ifstream file1("ships.json");
    if (!file1.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл ships.json!\n";
        return 1;
    }
    json j_ships;
    file1 >> j_ships;
    file1.close();
    std::vector<Ship> ships = j_ships.get<std::vector<Ship>>();

    std::ifstream file2("stations.json");
    if (!file2.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл stations.json!\n";
        return 1;
    }
    json j_stations;
    file2 >> j_stations;
    file2.close();
    std::vector<Station> stations = j_stations.get<std::vector<Station>>();

    std::vector<Cargo> cargoList;
    int cargoAmount;

    title();
    std::cout << "Введите количество грузов для оформления: ";
    std::cin >> cargoAmount;


    // ========================================================================================================================
    // 1. ЦИКЛ ВВОДА ГРУЗОВ
    // ========================================================================================================================
    for (int i = 0; i < cargoAmount; i++) {
        title();
        cargoInput( i, cargoList);
    }
    int choice;
    do{
        title();
        cargoOutput(cargoList);
        std::cout<<"Изменить грузы перед тем как перейти к расчётам?\n";
        std::cout<<"| 1. Добавить груз | 2. Удалить груз | 3. Изменить груз | 0. Подтвердить грузы |\n";
        std::cout<<"Ваш выбор: ";
        std::cin>>choice;
        switch (choice) {
               case 1: {cargoInput(cargoList.size(), cargoList); break;}
              case 2: {cargoDelete(cargoList); break;}
              case 3: {cargoChange(cargoList); break;}
              default: {std::cout<<"Пожалуйста введите подходящую цифру от 0 до 9\n"; break;}
            }
    }while(choice!=0);

    //Ввод пути
    title();
    Station From;
    while (true){
        std::cout << "Введите имя станции, где вы находитесь: ";
        std::string fromName;
        std::cin>>fromName;
        if (StationIdx(stations, fromName)==-1){
            std::cout << "Станции с именем '"<< fromName<<"' не существует, введите другое имя\n";
        }else{
            std::cout << "Вы находитесь на "<<fromName<<std::endl;
            From=stations[StationIdx(stations, fromName)];
            break;
        } 
    }
    Station To;
    while (true){
        std::cout << "Введите имя станции, куда вы хотите отправить груз: ";
        std::string toName;
        std::cin>>toName;
        if (StationIdx(stations, toName)==-1){
            std::cout << "Станции с именем '"<< toName<<"' не существует, введите другое имя\n";
        }else{
            std::cout << "Груз летит на "<<toName<<std::endl;
            To=stations[StationIdx(stations, toName)];
            break;
        } 
    }
    std::cout << "Нажмите Enter для вывода таблицы...";

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::cin.get();

    //Таблица
    title();
    std::vector<std::vector<Ship>> Answer = cargoSort(ships, stations, cargoList, From, To, INF);
    sortShipsByCost(Answer, stations, From, To, INF);
    printTableHead();
    for (vector<Ship> shipN : Answer){
        infoOut(shipN, stations, From, To, INF);
    }
    
    std::cout << "Нажмите Enter для выхода из системы снабжения...";

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::cin.get();
    
    return 0;
}