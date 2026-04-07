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
    
    vector<int> greedyTSP(int startVertex);
    
    double getTourCost(vector<int> tour);
    
    void printResults(int startVertex);
};

#endif