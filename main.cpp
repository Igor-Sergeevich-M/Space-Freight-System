#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <windows.h> // Добавили для корректной работы кодировки и очистки в Windows
#include "Cargo.h"
#include "Station.h"
#include "Ship.h"

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
    std::cout << "________________________________________________________________________________________________________________________\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "   ********************                                                                           ********************\n";
    std::cout << "  ********************                    SPACE FREIGHT SYSTEM                                     ********************\n";
    std::cout << "   ********************                                                                           ********************\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "________________________________________________________________________________________________________________________\n";
    std::cout << "\n";
}

double shipCost(const Ship ship, std::vector<Station> path){

    int parkingPricePerGab=1;
    int fuelPricePerGallon=100;

    int numberStations=path.size();
    int parkingPrice=parkingPricePerGab*ship.maxGab;
    int totalParkingPrice=parkingPrice*numberStations;
    double totalFuelPrice=multiDistance(path)*ship.fuelEfficiency*fuelPricePerGallon;
    double totalShipCost=totalFuelPrice+totalParkingPrice;
    return totalShipCost;
}
double allShipsPrice(const vector<Ship> ships, const vector<Station> stations, const Station& From, const Station& To, double INF){
    double totalCost=0;
    int n=ships.size();
    for(int i=0; i<n; i++){
        totalCost=totalCost+shipCost(ships[i], PathFinder(stations, From, To, fuelDistance(ships[i]), INF));
    }
    return totalCost;
}
double shipTime(const Ship ship, std::vector<Station> path){
    return (365/ship.maxSpeed)*multiDistance(path);
}
double totalShipTime(const vector<Ship> ships, const vector<Station> stations, const Station& From, const Station& To, double INF){
    double maxTime=0.0;
    int n=ships.size();
    for(int i=0; i<n; i++){
        double curTime=shipTime(ships[i], PathFinder(stations, From, To, fuelDistance(ships[i]), INF));
        if (maxTime<curTime) maxTime=curTime;
    }
    return maxTime;
}
void infoOut(const vector<Ship> ships, const vector<Station> stations, const Station& From, const Station& To, double INF){
    // 1. Защита от пустого вектора
    if (ships.empty()) {
        std::cout << "| " << setw(60) << "Нет доступных кораблей" << " | " 
                  << setw(10) << "0.00" << " | " << setw(7) << "0.00" << " |\n";
        return;
    }
    int n=ships.size();
    std::string shipsOut=ships[0].shipName;
    std::cout<<"| ";
    int count=1;
    for(int i=1; i<n; i++){
        if (ships[i].shipName==ships[i-1].shipName){ count++;
        }else if (count==1){ shipsOut.append(", "); shipsOut.append(ships[i].shipName);
        }else{
            shipsOut.append("x");
            shipsOut.append(std::to_string(count));
            shipsOut.append(", ");
            shipsOut.append(ships[i].shipName);
            count=1;
        }
        if  (i==n-1 && count!=1){
            shipsOut.append("x");
            shipsOut.append(std::to_string(count));
        }
    }
    std::cout<<setw(60)<<left<<shipsOut<<" | ";
    std::cout<<setw(10)<<left<<fixed<<setprecision(2)<<allShipsPrice(ships,stations, From, To, INF)<<" | ";
    std::cout<<setw(7)<<left<<fixed<<setprecision(2)<<totalShipTime(ships,stations, From, To, INF)<<" |\n";
    std::cout << "├" << std::string(62, '─') << "┼" << std::string(12, '─') << "┼" << std::string(9, '─') << "┤\n";
}
void printTableHead() {
    std::cout << "┌" << std::string(62, '─') << "┬" << std::string(12, '─') << "┬" << std::string(9, '─') << "┐\n";
    std::cout << "| " << std::setw(60) << std::left  << "СОСТАВ КОСМИЧЕСКОГО ФЛОТА" << " | "
              << std::setw(10) << std::right << "ЦЕНА (кред)"  << " | "
              << std::setw(7)  << std::right << "ВРЕМЯ (д)"   << " |\n";
              
    std::cout << "├" << std::string(62, '─') << "┼" << std::string(12, '─') << "┼" << std::string(9, '─') << "┤\n";
}
void sortShips(std::vector<std::vector<Ship>>& shipsX2, const vector<Station> stations, const Station& From, const Station& To, double INF) {
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
int main() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif

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
    title();
    std::cout << "Нажмите Enter для выхода из системы снабжения...";

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::cin.get();

    return 0;
}