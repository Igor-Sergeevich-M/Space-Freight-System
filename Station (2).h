#pragma once
#include <string>
#include "json.hpp"
using namespace std;
struct Station {
    string stationName;
    double x;
    double y;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Station, stationName, x, y)

double Distance(const Station& From, const Station& To);
int StationIdx(const vector<Station>& stations, string name);
vector<Station> PathFinder(const vector<Station>& stations, int startIdx, int endIdx, double fuelLimit, double INF);
