#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <cmath>
#include "SongDatabase.h"

using namespace std;

class Graph {
private:
    vector<Song> songs;
    int numVertices;
    vector<vector<double>> adjacencyMatrix;
    
    // Calculate Euclidean distance between two songs
    double calculateWeight(int i, int j);
    
public:
    Graph(vector<Song> songList);
    
    // Build the weighted graph
    void buildGraph();
    
    // Get weight of edge between vertices i and j
    double getWeight(int i, int j);
    
    // Get number of vertices
    int getNumVertices();
    
    // Get song by index
    Song getSong(int index);
    
    // Get all songs
    vector<Song> getSongs();
    
    // Print adjacency matrix (for debugging)
    void printGraph();
};

#endif