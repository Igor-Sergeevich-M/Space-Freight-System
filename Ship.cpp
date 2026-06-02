#include "Ship.h"
#include <cmath>
#include <vector>
//Сколько световых лет может пролететь корабль без обслуживания
double fuelDistance(const Ship& ship) {
    return static_cast<double>(ship.fuelTank) / ship.fuelEfficiency;
}
