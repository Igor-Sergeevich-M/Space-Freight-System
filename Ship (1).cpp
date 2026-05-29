#include "Ship.h"
#include <cmath>
using namespace std;
double fuelDistance(const Ship& Ship) {
    double fuelDistance=Ship.fuelTank/Ship.fuelEfficiency;
    return fuelDistance;
}