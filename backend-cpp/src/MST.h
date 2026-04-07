#ifndef MST_H
#define MST_H

#include <vector>
#include "Graph.h"

using namespace std;

struct Edge {
    int from;
    int to;
    double weight;
};

class MST {
private:
    Graph* graph;
    int numVertices;
    vector<Edge> mstEdges;
    
public:
    MST(Graph* g);
    
    vector<Edge> primMST(int startVertex);
    
    vector<int> mstToPlaylist(int startVertex);
    
    double getTotalWeight(vector<Edge> edges);
    
    void printResults(int startVertex);
};

#endif