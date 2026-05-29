#pragma once
#include <string>

struct Cargo {
    std::string cargoName;
    int cargoMass = 0;
    int cargoGabarits = 0;
    bool cargoPasanger = false;
    bool cargoCreature = false;
    bool cargoAmaxophobe = false;
    bool cargoDanger = false;
};
