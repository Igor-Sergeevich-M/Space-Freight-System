#pragma once
#include <string>
#include <vector>
struct Cargo {
    std::string cargoName;
    int cargoMass;
    int cargoGabarits;
    bool cargoCreature;   
    bool cargoAmaxophobe; 
    bool cargoDanger; 
};

void showChoice(bool live, bool amaxo, bool danger);
void helpTxt();
void cargoInput(int i, std::vector<Cargo>& cargoList);
void cargoOutput(std::vector<Cargo>& cargoList);
void cargoDelete(std::vector<Cargo>& cargos);
void cargoChange(std::vector<Cargo>& cargos);
