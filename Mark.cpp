#include <iostream>
#include <windows.h> 
#include "Station.h"

using namespace std;

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Station A, B;
    A.x = 6.0;
    A.y = 2.0;
    B.x = 3.0;
    B.y = 4.0;

    cout << "Расстояние между станциями: " << Distance(A, B) << endl;

    return 0;
}