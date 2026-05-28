#pragma once
#include <string>
// Объявляем структуру для хранения данных о грузе
struct Cargo {
    std::string cargoName;
    int cargoMass;
    int cargoGabarits;   // Объём в м^3
    bool cargoPasanger;
    bool cargoCreature;   // Животные/растения
    bool cargoAmaxophobe; // Чувствительность к скорости > 25 СС
    bool cargoDanger;     // Опасный груз
};
