#include <iostream>
#include <windows.h> 
#include <fstream>
#include <vector>
#include "Ship.h"

using namespace std;
using json = nlohmann::json;

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ifstream file("ships.json");
    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл ships.json!" << endl;
        return 1;
    }
    json j;
    file >> j;
    file.close();

    vector<Ship> space_ship = j.get<vector<Ship>>();


    cout << "Максимальное расстояние которое может пролететь корабль: " << fuelDistance(space_ship[0]) << endl;

    return 0;
}