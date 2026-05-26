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
