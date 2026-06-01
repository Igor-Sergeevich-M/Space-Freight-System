#include "cargo.h"
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>

void showChoice(bool live, bool amaxo, bool danger){
    std::cout<<"===================================== Выбор типов груза ======================================\n";
    if (live==true){ std::cout<<"| 1. Живой[x] ";
    }else std::cout<<"| 1. Живой[ ] ";
    if (amaxo==true){ std::cout<<"| 2. Тахочувствительный[x] ";
    }else std::cout<<"| 2. Тахочувствительный[ ] ";
    if (danger==true){ std::cout<<"| 3. Опасный[x] ";
    }else std::cout<<"| 3. Опасный [ ] ";
    std::cout<<"| 4. Описание ";
    std::cout<<"| 0. Подвердить типы |\n";
    std::cout<<"Ваш выбор: ";
}
void helpTxt(){
    std::cout<<"\n==============================================================================================\n";
    std::cout<<"1. Живой - груз перевозящий растения, животных и другие формы жизни\n"
             <<"2. Тахочувствительный - груз чувствительный к скорости больше 25 СС\n"
             <<"3. Опасный - груз, требующий особых условий хранения и мер безопасности\n"
             <<"Перевозка живых грузов вместе с опасными - запрещена (данные грузы будут распределены по разным кораблям)\n"
             <<"Перевозка тахочувствительных грузов производится на короблях с максимальной скоростью менее 25 СС\n";
    std::cout<<"==============================================================================================\n\n\n\n\n";
}
void cargoInput(int i, std::vector<Cargo>& cargoList){
    Cargo cargo;
    cargoList.push_back(cargo);
    std::cout << ">>> Оформление груза № " << i + 1 << " <<<\n";

    // Умная очистка буфера (убираем перевод строки '\n' после ввода чисел)
    if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }

    // 1. Название груза
    std::cout << "1. Введите название груза: ";
    std::getline(std::cin, cargoList[i].cargoName);

    // 2. Масса
    std::cout << "2. Масса груза (тонны): ";
    std::cin >> cargoList[i].cargoMass;

    // 3. Объём
    std::cout << "3. Объём груза (М^3): ";
    std::cin >> cargoList[i].cargoGabarits;
    int choice;
    cargoList[i].cargoCreature = false; cargoList[i].cargoAmaxophobe = false; cargoList[i].cargoDanger = false;
    do{
        showChoice(cargoList[i].cargoCreature, cargoList[i].cargoAmaxophobe, cargoList[i].cargoDanger);
        std::cin>>choice;
        switch (choice) {
            case 1: {cargoList[i].cargoCreature=!cargoList[i].cargoCreature; break;}
            case 2: {cargoList[i].cargoAmaxophobe=!cargoList[i].cargoAmaxophobe; break;}
            case 3: {cargoList[i].cargoDanger=!cargoList[i].cargoDanger; break;}
            case 4: {helpTxt(); break;}
            default: {std::cout<<"Пожалуйста введите подходящую цифру от 0 до 9\n"; break;}
        }
        std::cout << "\033[A\33[2K\r";
        std::cout << "\033[A\33[2K\r";
        std::cout << "\033[A\33[2K\r";
    }while (choice!=0);
}
void cargoOutput(std::vector<Cargo>& cargoList){
    int n=cargoList.size();
    std::cout<<"======================================= Список грузов ========================================\n";
    for(int i=0; i<n; i++){
        std::cout << "► ГРУЗ №" << i + 1 << ": \"" << cargoList[i].cargoName << "\"\n";
        std::cout << "  [Общие параметры]: Всего " << cargoList[i].cargoMass << " т. | " << cargoList[i].cargoGabarits << " м^3\n";

        std::cout << "  [Особый статус]:   ";
        if (cargoList[i].cargoDanger) std::cout << "[ КРИТИЧЕСКАЯ УГРОЗА ] ";
        if (cargoList[i].cargoAmaxophobe) std::cout << "[ ОГРАНИЧЕНИЕ СКОРОСТИ СС <= 25] ";
        if (cargoList[i].cargoCreature) std::cout << "[ ФЛОРА/ФАУНА] ";
        if (!cargoList[i].cargoDanger && !cargoList[i].cargoAmaxophobe && !cargoList[i].cargoCreature) {
            std::cout << "[Стандартный коммерческий груз]";
        }
        std::cout << "\n";
        std::cout<<"==============================================================================================\n";
    }
}
void cargoDelete(std::vector<Cargo>& cargos){
    std::string name;
    std::cout<<"Введите название груза, который хотите удалить: ";
    std::cin>>name;
    std::vector<Cargo>::iterator iter = cargos.begin();
    for(Cargo n : cargos){
        if (n.cargoName==name) cargos.erase(iter);
        ++iter;
    }
}
void cargoChange(std::vector<Cargo>& cargos){
    std::string name;
    std::cout<<"Введите название груза, который хотите изменить: ";
    std::cin>>name;
    int i=0;
    for(Cargo n : cargos){
        if (n.cargoName==name) cargoInput( i, cargos);;
        i++;
    }
    cargos.pop_back();
}
void sortCargoGab(std::vector<Cargo>& cargos) {
    int n = cargos.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Сбрасываем флаг в начале каждого прохода
        for (int j = 0; j < n - i - 1; j++) {
            if (cargos[j].cargoGabarits>cargos[j+1].cargoGabarits) {
                std::swap(cargos[j], cargos[j + 1]); 
                swapped = true; // Запоминаем, что перестановка была
            }
        }
        // Если перестановок не было — массив готов, выходим раньше времени!
        if (!swapped) {
            break;
        }
    }
}
void sortCargoMass(std::vector<Cargo>& cargos) {
    int n = cargos.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Сбрасываем флаг в начале каждого прохода
        for (int j = 0; j < n - i - 1; j++) {
            if (cargos[j].cargoMass>cargos[j+1].cargoMass) {
                std::swap(cargos[j], cargos[j + 1]); 
                swapped = true; // Запоминаем, что перестановка была
            }
        }
        // Если перестановок не было — массив готов, выходим раньше времени!
        if (!swapped) {
            break;
        }
    }
}