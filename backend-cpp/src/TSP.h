#ifndef TSP_H
#define TSP_H

#include <vector>
#include "Graph.h"

using namespace std;

class TSP {
private:
    Graph* graph;
    int numVertices;
    
public:
    TSP(Graph* g);
    
    // Greedy TSP using Nearest Neighbor heuristic
    vector<int> greedyTSP(int startVertex);
    
    // Get total tour cost
    double getTourCost(vector<int> tour);
    
    // Print tour results
    void printResults(int startVertex);
};

#endif