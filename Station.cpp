#include "Station.h"
#include <cmath>
using namespace std;
double Distance(const Station& From, const Station& To) {
    double d = sqrt(pow((To.x - From.x), 2) + pow((To.y - From.y), 2));
    return d;
}