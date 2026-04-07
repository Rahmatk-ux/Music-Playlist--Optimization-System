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
    
    double calculateWeight(int i, int j);
    
public:
    Graph(vector<Song> songList);
    
    void buildGraph();

    double getWeight(int i, int j);
    
    int getNumVertices();
    
    Song getSong(int index);
    
    vector<Song> getSongs();
    
    void printGraph();
};

#endif