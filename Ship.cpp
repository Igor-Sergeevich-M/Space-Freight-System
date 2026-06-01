#include "Ship.h"
#include <cmath>
#include <vector>
using namespace std;
//Сколько световых лет может пролететь корабль без обслуживания
double fuelDistance(const Ship& Ship) {
    double fuelDistance=Ship.fuelTank/Ship.fuelEfficiency;
    return fuelDistance;
}
void sortShipMass(std::vector<Ship>& ships) {
    int n = ships.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Сбрасываем флаг в начале каждого прохода
        for (int j = 0; j < n - i - 1; j++) {
            if (ships[j].maxMass>ships[j+1].maxMass) {
                std::swap(ships[j], ships[j + 1]); 
                swapped = true; // Запоминаем, что перестановка была
            }
        }
        // Если перестановок не было — массив готов, выходим раньше времени!
        if (!swapped) {
            break;
        }
    }
}
void sortShipGab(std::vector<Ship>& ships) {
    int n = ships.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Сбрасываем флаг в начале каждого прохода
        for (int j = 0; j < n - i - 1; j++) {
            if (ships[j].maxGab>ships[j+1].maxGab) {
                std::swap(ships[j], ships[j + 1]); 
                swapped = true; // Запоминаем, что перестановка была
            }
        }
        // Если перестановок не было — массив готов, выходим раньше времени!
        if (!swapped) {
            break;
        }
    }
}
