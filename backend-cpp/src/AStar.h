#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include "Graph.h"

using namespace std;

class AStar {
private:
    Graph* graph;
    int numVertices;

    // Heuristic: straight-line distance in BPM-energy space
    // Estimates cost from vertex v to goal
    double heuristic(int v, int goal);

public:
    AStar(Graph* g);

    // Find shortest path from start to goal using A*
    // Returns ordered list of vertex indices
    vector<int> findPath(int start, int goal);

    // Get total cost of a path
    double getPathCost(const vector<int>& path);

    void printResults(int start, int goal);
};

#endif
