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
    
    // Find vertex with minimum distance among unvisited
    int findMinDistance();
    
public:
    Dijkstra(Graph* g);
    
    // Run Dijkstra's algorithm from start vertex
    void run(int startVertex);
    
    // Get optimized path visiting all reachable vertices
    vector<int> getOptimizedPath(int startVertex);
    
    // Get total distance of the path
    double getTotalDistance(vector<int> path);
    
    // Print results
    void printResults(int startVertex);
};

#endif