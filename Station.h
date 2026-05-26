#pragma once
#include <string>
using namespace std;
struct Station {
    string stationName;
    double x;
    double y;
};

double Distance(const Station& From, const Station& To);
