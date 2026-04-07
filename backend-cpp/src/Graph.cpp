#include "Graph.h"
#include <iostream>
#include <iomanip>

using namespace std;

Graph::Graph(vector<Song> songList) {
    this->songs = songList;
    this->numVertices = songList.size();
    
    adjacencyMatrix.resize(numVertices);
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i].resize(numVertices, 0.0);
    }
}

double Graph::calculateWeight(int i, int j) {
    Song song1 = songs[i];
    Song song2 = songs[j];
    
    double bpmDiff = song1.bpm - song2.bpm;
    double energyDiff = (song1.energy - song2.energy) * 100.0; 
    
    double weight = sqrt(bpmDiff * bpmDiff + energyDiff * energyDiff);
    
    return weight;
}

void Graph::buildGraph() {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (i != j) {
                adjacencyMatrix[i][j] = calculateWeight(i, j);
            }
        }
    }
}

double Graph::getWeight(int i, int j) {
    if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
        return adjacencyMatrix[i][j];
    }
    return 0.0;
}

int Graph::getNumVertices() {
    return numVertices;
}


Song Graph::getSong(int index) {
    if (index >= 0 && index < numVertices) {
        return songs[index];
    }
    return Song();
}

vector<Song> Graph::getSongs() {
    return songs;
}

void Graph::printGraph() {
    cout << "\n=== Graph Adjacency Matrix ===" << endl;
    cout << "Total Songs: " << numVertices << endl;
    cout << "\nShowing first 5 songs:\n" << endl;
    
    for (int i = 0; i < min(5, numVertices); i++) {
        cout << "Song " << i << " (" << songs[i].title << "):" << endl;
        for (int j = 0; j < min(5, numVertices); j++) {
            cout << fixed << setprecision(2) << setw(8) << adjacencyMatrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
}