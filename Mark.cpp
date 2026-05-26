#include <iostream>
#include <windows.h> 
#include "Station.h"
#include <fstream>
#include <vector>

using namespace std;
using json = nlohmann::json;

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ifstream file("stations.json");
    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл stations.json!" << endl;
        return 1;
    }
    json j;
    file >> j;
    file.close();

    vector<Station> space_stations = j.get<vector<Station>>();


    cout << "Расстояние между станциями: " << Distance(space_stations[0], space_stations[1]) << endl;

    return 0;
}