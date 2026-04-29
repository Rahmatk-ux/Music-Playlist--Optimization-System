#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include "Graph.h"

using namespace std;

// Edge struct for Kruskal (needs weight for sorting)
struct KEdge {
    int from;
    int to;
    double weight;
    // For sorting by weight ascending
    bool operator<(const KEdge& other) const {
        return weight < other.weight;
    }
};

// Union-Find (Disjoint Set Union) data structure
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;

public:
    UnionFind(int n);

    // Find root of set containing x (with path compression)
    int find(int x);

    // Union two sets (by rank)
    bool unite(int x, int y);

    // Check if x and y are in same set
    bool connected(int x, int y);
};

class Kruskal {
private:
    Graph* graph;
    int numVertices;
    vector<KEdge> mstEdges;

public:
    Kruskal(Graph* g);

    // Run Kruskal's algorithm, returns MST edges
    vector<KEdge> runKruskal();

    // Convert MST to playlist using greedy traversal
    vector<int> mstToPlaylist(int startVertex);

    // Get total weight of MST
    double getTotalWeight(const vector<KEdge>& edges);

    void printResults(int startVertex);
};

#endif
