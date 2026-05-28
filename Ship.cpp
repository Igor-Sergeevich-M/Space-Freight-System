#include "Ship.h"
#include <cmath>
using namespace std;
//Сколько световых лет может пролететь корабль без обслуживания
double fuelDistance(const Ship& Ship) {
    double fuelDistance=Ship.fuelTank/Ship.fuelEfficiency;
    return fuelDistance;
}
