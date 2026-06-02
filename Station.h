#pragma once
#include <string>
#include "json.hpp"
struct Station {
    std::string stationName;
    double x;
    double y;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Station, stationName, x, y)

double Distance(const Station& From, const Station& To);
double multiDistance(const std::vector<Station>& stations);
int StationIdx(const std::vector<Station>& stations, std::string name);
std::vector<Station> PathFinder(const std::vector<Station>& stations, const Station& From, const Station& To, double fuelLimit, double INF);
