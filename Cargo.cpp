#include <iostream>
#include <string>
#include <windows.h> // Добавили для корректной работы кодировки и очистки в Windows
#include "Cargo.h"

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

int main() {
    // Настройка локали для совместимости
    setlocale(LC_ALL, "Russian");

    // Создаем статический массив структур (максимум на 100 грузов)
    Cargo cargoList[100];

    int cargoAmount;
    int tempBool;

    title();
    std::cout << "Введите количество грузов для оформления: ";
    std::cin >> cargoAmount;

    // Ограничим от дурака, чтобы не вышли за пределы массива
    if (cargoAmount > 100) cargoAmount = 100;
    if (cargoAmount <= 0) cargoAmount = 1;

    // ========================================================================================================================
    // 1. ЦИКЛ ВВОДА ГРУЗОВ
    // ========================================================================================================================
    for (int i = 0; i < cargoAmount; i++) {
        title();
        std::cout << ">>> Оформление груза № " << i + 1 << " из " << cargoAmount << " <<<\n\n";

        // Умная очистка буфера (убираем перевод строки '\n' после ввода чисел)
        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }

        // 1. Название груза
        std::cout << "1. Добро пожаловать, введите название груза: ";
        std::getline(std::cin, cargoList[i].cargoName);

        // 2. Масса
        std::cout << "2. Масса груза (тонны): ";
        std::cin >> cargoList[i].cargoMass;

        // 3. Объём
        std::cout << "3. Объём груза (М^3): ";
        std::cin >> cargoList[i].cargoGabarits;

        // 4. Пассажиры
        std::cout << "4. Пассажиры составляют часть этого груза? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoPasanger = (tempBool == 1);

        // 5. Животные/растения
        std::cout << "5. Животные/растения? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoCreature = (tempBool == 1);

        // 6. Амаксофобы
        std::cout << "6. Груз чувствителен к скорости > 25 СС (Амаксофобы)? (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoAmaxophobe = (tempBool == 1);

        // 7. Опасность
        std::cout << "7. Груз опасный? (Радиоактивный, взрывоопасный, биоугроза) (1 - Да, 0 - Нет): ";
        std::cin >> tempBool;
        cargoList[i].cargoDanger = (tempBool == 1);
    }

    // ========================================================================================================================
    // 2. НОВАЯ ЧАСТЬ: АЛГОРИТМ ПОДБОРА И РАСПРЕДЕЛЕНИЯ КОСМИЧЕСКОГО ФЛОТА
    // ========================================================================================================================
    title();
    std::cout << "====================================== ОТЧЁТ КОСМИЧЕСКОЙ ЛОГИСТИКИ ======================================\n\n";

    for (int i = 0; i < cargoAmount; i++) {
        int remainingMass = cargoList[i].cargoMass;
        int remainingVol = cargoList[i].cargoGabarits;

        int largeShipsCount = 0;
        int mediumShipsCount = 0;
        int smallShipsCount = 0;

        // Набиваем Большие корабли (макс. значения из таблицы: 6000 т / 1500 м^3)
        while (remainingMass > 4000 || remainingVol > 1000) {
            largeShipsCount++;
            remainingMass -= 6000;
            remainingVol -= 1500;

            if (remainingMass < 0) remainingMass = 0;
            if (remainingVol < 0) remainingVol = 0;
        }

        // Распределяем остатки груза
        if (remainingMass > 0 || remainingVol > 0) {
            // Проверяем, влезает ли в Маленький (макс: 2000 т / 500 м^3)
            if (remainingMass <= 2000 && remainingVol <= 500) {
                // Если груз — амаксофоб, отправлять на маленьком нельзя (у него скорость до 40 СС)
                if (cargoList[i].cargoAmaxophobe) {
                    mediumShipsCount++; // Повышаем класс корабля до Среднего
                }
                else {
                    smallShipsCount++;  // Обычный груз везем на Маленьком
                }
            }
            // Если в маленький не влезло, но меньше лимитов Среднего (4000 т / 1000 м^3)
            else {
                mediumShipsCount++;
            }
        }

        // Вывод результатов для каждого оформленного груза
        std::cout << "► ГРУЗ №" << i + 1 << ": \"" << cargoList[i].cargoName << "\"\n";
        std::cout << "  [Общие параметры]: Всего " << cargoList[i].cargoMass << " т. | " << cargoList[i].cargoGabarits << " м^3\n";

        std::cout << "  [Особый статус]:   ";
        if (cargoList[i].cargoDanger) std::cout << "[ КРИТИЧЕСКАЯ УГРОЗА: ИЗОЛИРОВАННЫЙ РЕЙС] ";
        if (cargoList[i].cargoAmaxophobe) std::cout << "[ АМАКСОФОБИЯ: ОГРАНИЧЕНИЕ СКОРОСТИ СС <= 25] ";
        if (cargoList[i].cargoPasanger) std::cout << "[ ЖИВОЙ МОДУЛЬ: ПАССАЖИРЫ] ";
        if (cargoList[i].cargoCreature) std::cout << "[ ФЛОРА/ФАУНА] ";
        if (!cargoList[i].cargoDanger && !cargoList[i].cargoAmaxophobe && !cargoList[i].cargoPasanger && !cargoList[i].cargoCreature) {
            std::cout << "[Стандартный коммерческий груз]";
        }
        std::cout << "\n";

        std::cout << "  [Сформированный флот для доставки]:\n";
        bool fleetAllocated = false;

        if (largeShipsCount > 0) {
            std::cout << "    • Большой корабль   (до 6000т / 1500м^3) — " << largeShipsCount << " шт.\n";
            fleetAllocated = true;
        }
        if (mediumShipsCount > 0) {
            std::cout << "    • Средний корабль   (до 4000т / 1000м^3) — " << mediumShipsCount << " шт.\n";
            fleetAllocated = true;
        }
        if (smallShipsCount > 0) {
            std::cout << "    • Маленький корабль (до 2000т / 500м^3)  — " << smallShipsCount << " шт.\n";
            fleetAllocated = true;
        }

        if (!fleetAllocated) {
            std::cout << "    • [!] Флот не требуется (нулевой объем груза).\n";
        }

        if (cargoList[i].cargoDanger) {
            std::cout << "  [ВНИМАНИЕ]: Из-за класса опасности данный флот летит выделенным коридором отдельно от других типов снабжения!\n";
        }

        std::cout << "________________________________________________________________________________________________________________________\n\n";
    }

    // ========================================================================================================================
    // 3. ЗАВЕРШЕНИЕ ПРОГРАММЫ
    // ========================================================================================================================
    std::cout << "Нажмите Enter для выхода из системы снабжения...";

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::cin.get();

    return 0;
}