#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "Graph.h"

using namespace std;

class Dijkstra {
private:
    Graph* graph;
    int numVertices;
    vector<double> distances;
    vector<bool> visited;
    vector<int> previous;
    
    int findMinDistance();
    
public:
    Dijkstra(Graph* g);
    
    void run(int startVertex);
    
    vector<int> getOptimizedPath(int startVertex);
    
    double getTotalDistance(vector<int> path);
    
    void printResults(int startVertex);
};

#endif