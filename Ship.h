#pragma once
#include <string>
#include "json.hpp"
using namespace std;
struct Ship {
    string shipName;
    int maxMass;
    int maxGab;
    int maxSpeed;
    int fuelEfficiency;
    int fuelTank;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Ship, shipName, maxMass, maxGab, maxSpeed, fuelEfficiency, fuelTank)

double fuelDistance(const Ship& Ship);
void sortShipMass(std::vector<Ship>& ships);
void sortShipGab(std::vector<Ship>& ships);