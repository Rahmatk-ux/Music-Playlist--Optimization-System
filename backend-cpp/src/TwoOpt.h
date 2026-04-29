#ifndef TWOOPT_H
#define TWOOPT_H

#include <vector>
#include "Graph.h"

using namespace std;

class TwoOpt {
private:
    Graph* graph;
    int numVertices;

    // Calculate total cost of a tour
    double tourCost(const vector<int>& tour);

    // Reverse a segment of the tour between index i and j
    void reverseSegment(vector<int>& tour, int i, int j);

public:
    TwoOpt(Graph* g);

    // Run greedy nearest-neighbor first, then improve with 2-opt
    vector<int> run(int startVertex);

    // Apply 2-opt improvement to an existing tour
    vector<int> improve(vector<int> tour);

    // Get total cost of a tour
    double getTourCost(const vector<int>& tour);

    void printResults(int startVertex);
};

#endif
