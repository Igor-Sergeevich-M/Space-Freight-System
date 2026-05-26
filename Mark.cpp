#include <iostream>
#include <windows.h> 
#include <fstream>
#include <vector>
#include "Ship.h"
#include "Station.h"
#include <string>

using namespace std;
using json = nlohmann::json;

const double INF = 1e9;

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ifstream file1("ships.json");
    if (!file1.is_open()) {
        cout << "Ошибка: Не удалось открыть файл ships.json!" << endl;
        return 1;
    }
    json j;
    file1 >> j;
    file1.close();

    vector<Ship> ships = j.get<vector<Ship>>();

    ifstream file2("stations.json");
    if (!file2.is_open()) {
        cout << "Ошибка: Не удалось открыть файл stations.json!" << endl;
        return 1;
    }
    json f;
    file2 >> f;
    file2.close();

    vector<Station> stations = f.get<vector<Station>>();

    Ship mini=ships[0];
    string startName, endName;
    cout<<"Откуда: "; cin>>startName;
    cout<<"Куда: "; cin>>endName;

    vector<Station> RossLalande=PathFinder(stations, StationIdx(stations, startName), StationIdx(stations, endName), fuelDistance(mini), INF);
    cout<<RossLalande[0].stationName<<' '<<RossLalande[1].stationName<<' '<<RossLalande[2].stationName<<' ';

    return 0;
}