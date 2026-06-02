#include "Station.h"
#include <cmath>
#include <vector>
#include <cstring>

//Вычисление дистанции между станциями
double Distance(const Station& From, const Station& To) {
    double stationDistance = sqrt(pow((To.x - From.x), 2) + pow((To.y - From.y), 2));
    return stationDistance;
}
//Вычисление дистанции между несколькими станциями
double multiDistance(const std::vector<Station>& stations) {
    int n=stations.size();
    double distance=0;
    for (int i=1; i<n; i++){
        distance=Distance(stations[i-1], stations[i])+distance;
    }
    return distance;
}
//Поиск станции по имени
int StationIdx(const std::vector<Station>& stations, std::string name){
    int n=stations.size();
    for (int i=0; i<n; i++){
        if (stations[i].stationName==name) return i;
    }
    return -1;
}
//Вычисление кратчайшего маршрута по алгоритму Дейкстры
std::vector<Station> PathFinder(const std::vector<Station>& stations, const Station& From, const Station& To, double fuelLimit, double INF) {
    int startIdx=StationIdx(stations, From.stationName);
    int endIdx=StationIdx(stations, To.stationName);
    int n=stations.size();
    std::vector<double> dist(n, INF);//Расстояние каждой точки до начальной (пока не нашли-бесконечное)
    dist[startIdx]=0;
    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);//Каждой звезде будет присваеватся индекс системы из которой в неё прилетели
    
    for (int i=0; i<n; ++i) {
        int v=-1;
        //Поиск ближайшего соседа (в начале ищет станцию откуда стартует)
        for (int j=0; j<n; ++j) {
            if (!visited[j] && (v==-1 || dist[j]<dist[v])) {
                v=j;
            }
        }
        if (v == -1 || dist[v] == INF) break;
        visited[v] = true;
        if (v == endIdx) break;

        for (int to = 0; to < n; ++to) {
            if (v == to) continue;
            double edgeWeight=Distance(stations[v], stations[to]);
            if (edgeWeight>fuelLimit) continue;//Если корабль не может долететь до станции пропускаем еЁ
            if (dist[v]+edgeWeight<dist[to]) {
                dist[to]=dist[v]+edgeWeight;
                parent[to]=v;
            }
        }
    }
    if (dist[endIdx] == INF) {
        return std::vector<Station>(); // Пути нет, возвращаем пустой вектор!
    }
    std::vector<Station> path;
    for (int v=endIdx; v!=-1; v=parent[v]) {
        path.push_back(stations[v]);
    }
    reverse(path.begin(), path.end());
    return path;
}
//Стоимость проезда через все станции на пути
double serviceCost(const std::vector<Station>& stations, double cost_per_station){
    int n=stations.size();
    double price=0;
    for (int i=1; i<n; i++){
        price=cost_per_station+price;
    }
    return price;
}
