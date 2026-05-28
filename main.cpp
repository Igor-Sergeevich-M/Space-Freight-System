#include <iostream>
#include <string>
#include <vector>
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