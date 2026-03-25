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
    
    // Prim's MST Algorithm
    vector<Edge> primMST(int startVertex);
    
    // Convert MST to playlist order (DFS traversal)
    vector<int> mstToPlaylist(int startVertex);
    
    // Get total MST weight
    double getTotalWeight(vector<Edge> edges);
    
    // Print MST results
    void printResults(int startVertex);
};

#endif