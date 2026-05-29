#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include "Cargo.h"
#include "Ship.h"
#include "Station.h"
using json = nlohmann::json;

const double INF = 1e9;

// ========================================================================================================================
// ⚙️ НАСТРОЙКИ БАЛАНСА И ЭКОНОМИКИ (ДЛЯ РЕДАКТИРОВАНИЯ)
// ========================================================================================================================
const double FUEL_PRICE = 10.0;

const double STOP_TIME_SMALL = 1.0;
const double STOP_TIME_MEDIUM = 3.0;
const double STOP_TIME_LARGE = 5.0;

const double SERVICE_PRICE_SMALL = 50.0;
const double SERVICE_PRICE_MEDIUM = 150.0;
const double SERVICE_PRICE_LARGE = 400.0;
// ========================================================================================================================

void title() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    system("cls");
#else
    system("clear");
#endif

    std::cout << "\n";
    std::cout << "________________________________________________________________________________________________________________________\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "   ******************** ********************\n";
    std::cout << "  ******************** SPACE FREIGHT SYSTEM                            ********************\n";
    std::cout << "   ******************** ********************\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "________________________________________________________________________________________________________________________\n";
    std::cout << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    // Загрузка файлов конфигурации
    std::ifstream file1("ships.json");
    if (!file1.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл ships.json!\n";
        return 1;
    }
    json j_ships;
    file1 >> j_ships;
    file1.close();
    std::vector<Ship> allShips = j_ships.get<std::vector<Ship>>();

    std::ifstream file2("stations.json");
    if (!file2.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл stations.json!\n";
        return 1;
    }
    json j_stations;
    file2 >> j_stations;
    file2.close();
    std::vector<Station> stations = j_stations.get<std::vector<Station>>();

    // Ввод и распределение грузов
    Cargo cargoList[100];
    int cargoAmount;
    int tempBool;

    int normalMass = 0, normalVol = 0;
    int passengerMass = 0, passengerVol = 0;
    int creatureMass = 0, creatureVol = 0;
    int amaxophobeMass = 0, amaxophobeVol = 0;
    int dangerMass = 0, dangerVol = 0;

    title();
    std::cout << "Введите количество грузов для оформления: ";
    std::cin >> cargoAmount;

    if (cargoAmount > 100) cargoAmount = 100;
    if (cargoAmount <= 0) cargoAmount = 1;

    for (int i = 0; i < cargoAmount; i++) {
        title();
        std::cout << ">>> Оформление груза № " << i + 1 << " из " << cargoAmount << " <<<\n\n";

        if (std::cin.peek() == '\n') std::cin.ignore();

        std::cout << "1. Введите название груза: ";
        std::getline(std::cin, cargoList[i].cargoName);

        std::cout << "2. Масса груза (тонны): ";
        std::cin >> cargoList[i].cargoMass;

        std::cout << "3. Объём груза (М^3): ";
        std::cin >> cargoList[i].cargoGabarits;

        std::cout << "4. Пассажиры составляют часть груза? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoPasanger = (tempBool == 1);

        std::cout << "5. Животные/растения? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoCreature = (tempBool == 1);

        std::cout << "6. Чувствителен к скорости > 25 СС (Амаксофобы)? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoAmaxophobe = (tempBool == 1);

        std::cout << "7. Груз опасный? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoDanger = (tempBool == 1);

        if (cargoList[i].cargoDanger) {
            dangerMass += cargoList[i].cargoMass;
            dangerVol += cargoList[i].cargoGabarits;
        }
        else if (cargoList[i].cargoAmaxophobe) {
            amaxophobeMass += cargoList[i].cargoMass;
            amaxophobeVol += cargoList[i].cargoGabarits;
        }
        else if (cargoList[i].cargoPasanger) {
            passengerMass += cargoList[i].cargoMass;
            passengerVol += cargoList[i].cargoGabarits;
        }
        else if (cargoList[i].cargoCreature) {
            creatureMass += cargoList[i].cargoMass;
            creatureVol += cargoList[i].cargoGabarits;
        }
        else {
            normalMass += cargoList[i].cargoMass;
            normalVol += cargoList[i].cargoGabarits;
        }
    }

    // Навигация
    title();
    std::string startName, endName;
    std::cout << "====================================== НАСТРОЙКА НАВИГАЦИИ ======================================\n\n";
    std::cout << "Введите точку отправления (например, Sun): ";
    std::cin >> startName;
    std::cout << "Введите точку прибытия (например, Centauri): ";
    std::cin >> endName;

    int startIdx = StationIdx(stations, startName);
    int endIdx = StationIdx(stations, endName);

    // Защитная проверка существования станций
    if (startIdx == -1 || endIdx == -1) {
        std::cout << "\n[ ОШИБКА]: Одна или обе станции не найдены в навигационной базе данных!\n";
        std::cout << "Нажмите Enter для выхода из системы...";
        if (std::cin.peek() == '\n') std::cin.ignore();
        std::cin.get();
        return 1;
    }

    int masses[5] = { dangerMass, amaxophobeMass, passengerMass, creatureMass, normalMass };
    int volumes[5] = { dangerVol, amaxophobeVol, passengerVol, creatureVol, normalVol };
    std::string containerNames[5] = { "Опасные грузы", "Амаксофобы", "Пассажиры", "Флора/Фауна", "Стандарт" };

    title();
    std::cout << "====================================== ЛОГИСТИЧЕСКИЙ АНАЛИЗ МАРШРУТА ======================================\n\n";

    for (int k = 0; k < 5; k++) {
        if (masses[k] == 0 && volumes[k] == 0) continue;

        std::cout << "► АНАЛИЗ ДЛЯ КАТЕГОРИИ: \"" << containerNames[k] << "\" (" << masses[k] << " т. / " << volumes[k] << " м^3)\n";

        Ship bestTimeShip, bestPriceShip;
        double minTime = INF;
        double minPrice = INF;
        std::vector<Station> bestTimePath, bestPricePath;

        int countTimeLarge = 0, countTimeMed = 0, countTimeSmall = 0;
        int countPriceLarge = 0, countPriceMed = 0, countPriceSmall = 0;

        double bestTimeFuelCost = 0, bestTimeServiceCost = 0, bestTimeTotalPrice = 0;
        double bestPriceFuelCost = 0, bestPriceServiceCost = 0, bestPriceTime = 0;

        for (const auto& ship : allShips) {

            if (k == 1 && ship.maxSpeed > 25) continue; // Защита амаксофобов

            double limit = fuelDistance(ship);
            std::vector<Station> currentPath = PathFinder(stations, startIdx, endIdx, limit, INF);

            if (currentPath.empty() || currentPath.size() < 2) continue;

            double totalDistance = 0;
            for (size_t s = 0; s < currentPath.size() - 1; ++s) {
                totalDistance += Distance(currentPath[s], currentPath[s + 1]);
            }

            int neededShips = 0;
            int remMass = masses[k];
            int remVol = volumes[k];

            while (remMass > 0 || remVol > 0) {
                neededShips++;
                remMass -= ship.maxMass;
                remVol -= ship.maxGab;
            }

            double currentStopTime = 0;
            double currentServicePrice = 0;

            if (ship.shipName == "Small") {
                currentStopTime = STOP_TIME_SMALL;
                currentServicePrice = SERVICE_PRICE_SMALL;
            }
            else if (ship.shipName == "Medium") {
                currentStopTime = STOP_TIME_MEDIUM;
                currentServicePrice = SERVICE_PRICE_MEDIUM;
            }
            else if (ship.shipName == "Large") {
                currentStopTime = STOP_TIME_LARGE;
                currentServicePrice = SERVICE_PRICE_LARGE;
            }

            // Расчет времени
            double travelTime = totalDistance / ship.maxSpeed + (currentPath.size() - 2) * currentStopTime;

            // Расчет финансов с детализацией расходов
            double fuelSpent = totalDistance * ship.fuelEfficiency * neededShips;
            double fuelCost = fuelSpent * FUEL_PRICE;

            int servicesCount = currentPath.size() * neededShips;
            double serviceCost = servicesCount * currentServicePrice;

            double totalPrice = fuelCost + serviceCost;

            if (travelTime < minTime) {
                minTime = travelTime;
                bestTimeShip = ship;
                bestTimePath = currentPath;
                countTimeLarge = (ship.shipName == "Large") ? neededShips : 0;
                countTimeMed = (ship.shipName == "Medium") ? neededShips : 0;
                countTimeSmall = (ship.shipName == "Small") ? neededShips : 0;

                bestTimeFuelCost = fuelCost;
                bestTimeServiceCost = serviceCost;
                bestTimeTotalPrice = totalPrice;
            }

            if (totalPrice < minPrice) {
                minPrice = totalPrice;
                bestPriceShip = ship;
                bestPricePath = currentPath;
                countPriceLarge = (ship.shipName == "Large") ? neededShips : 0;
                countPriceMed = (ship.shipName == "Medium") ? neededShips : 0;
                countPriceSmall = (ship.shipName == "Small") ? neededShips : 0;

                bestPriceFuelCost = fuelCost;
                bestPriceServiceCost = serviceCost;
                bestPriceTime = travelTime;
            }
        }

        if (minTime == INF || minPrice == INF) {
            std::cout << "  [ ОШИБКА]: Автономности кораблей недостаточно для преодоления пустоты между станциями!\n";
        }
        else {
            std::cout << "   А) ВЫГОДНО ПО ВРЕМЕНИ (Самый быстрый рейс):\n";
            std::cout << "     • Класс кораблей: " << bestTimeShip.shipName << " (Скорость: " << bestTimeShip.maxSpeed << " CC)\n";
            std::cout << "     • Состав флота:   ";
            if (countTimeLarge) std::cout << countTimeLarge << " ед. класса Large\n";
            if (countTimeMed) std::cout << countTimeMed << " ед. класса Medium\n";
            if (countTimeSmall) std::cout << countTimeSmall << " ед. класса Small\n";
            std::cout << "     • Минимальное время пути: " << minTime << " дн.\n";
            std::cout << "     • Финансовые затраты за скорость: " << bestTimeTotalPrice << " кр.\n";
            std::cout << "       > на топливо: " << bestTimeFuelCost << " кр.\n";
            std::cout << "       > на обслуживание: " << bestTimeServiceCost << " кр.\n";
            std::cout << "     • Маршрутный лист (" << bestTimePath.size() << " обслуж.): ";
            for (size_t p = 0; p < bestTimePath.size(); p++) {
                std::cout << bestTimePath[p].stationName << (p == bestTimePath.size() - 1 ? "" : " -> ");
            }
            std::cout << "\n\n";

            std::cout << "   Б) ВЫГОДНО ПО ДЕНЬГАМ (Самый дешёвый рейс):\n";
            std::cout << "     • Класс кораблей: " << bestPriceShip.shipName << "\n";
            std::cout << "     • Состав флота:   ";
            if (countPriceLarge) std::cout << countPriceLarge << " ед. класса Large\n";
            if (countPriceMed) std::cout << countPriceMed << " ед. класса Medium\n";
            if (countPriceSmall) std::cout << countPriceSmall << " ед. класса Small\n";
            std::cout << "     • Минимальный бюджет миссии: " << minPrice << " космических кредитов.\n";
            std::cout << "       > из них на топливо: " << bestPriceFuelCost << " кр.\n";
            std::cout << "       > из них на обслуживание: " << bestPriceServiceCost << " кр.\n";
            std::cout << "     • Плата за экономию (время в пути): " << bestPriceTime << " дн.\n";
            std::cout << "     • Маршрутный лист (" << bestPricePath.size() << " обслуж.): ";
            for (size_t p = 0; p < bestPricePath.size(); p++) {
                std::cout << bestPricePath[p].stationName << (p == bestPricePath.size() - 1 ? "" : " -> ");
            }
            std::cout << "\n";
        }
        std::cout << "________________________________________________________________________________________________________________________\n\n";
    }

    std::cout << "Нажмите Enter для выхода из системы снабжения...";
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::cin.get();

    return 0;
}
