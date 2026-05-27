#include "Station.h"
#include <cmath>
#include <vector>
#include <cstring>

using namespace std;
double Distance(const Station& From, const Station& To) {
    double stationDistance = sqrt(pow((To.x - From.x), 2) + pow((To.y - From.y), 2));
    return stationDistance;
}
double multiDistance(const vector<Station> stations) {
    int n=stations.size();
    double distance=0;
    for (int i=1; i<n; i++){
        distance=Distance(stations[i-1], stations[i])+distance;
    }
    return distance;
}
int StationIdx(const vector<Station>& stations, string name){
    int n=stations.size();
    for (int i=0; i<n; i++){
        if (stations[i].stationName==name) return i;
    }
}
vector<Station> PathFinder(const vector<Station>& stations, int startIdx, int endIdx, double fuelLimit, double INF) {
    int n=stations.size();
    vector<double> dist(n, INF);
    dist[startIdx]=0;
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    for (int i=0; i<n; ++i) {
        int v=-1;
        for (int j=0; j<n; ++j) {
            if (!visited[j] && (v==-1 || dist[j]<dist[v])) {
                v=j;
            }
        }
        if (dist[v]==INF) break;
        visited[v] = true;
        for (int to = 0; to < n; ++to) {
            if (v == to) continue;
            double edgeWeight=Distance(stations[v], stations[to]);
            if (edgeWeight>fuelLimit) continue; 
            if (dist[v]+edgeWeight<dist[to]) {
                dist[to]=dist[v]+edgeWeight;
                parent[to]=v;
            }
        }
    }
    vector<Station> path;
    for (int v=endIdx; v!=-1; v=parent[v]) {
        path.push_back(stations[v]);
    }
    reverse(path.begin(), path.end());
    return path;
}