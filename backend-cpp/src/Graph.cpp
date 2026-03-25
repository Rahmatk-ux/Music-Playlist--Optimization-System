#include "Graph.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor
Graph::Graph(vector<Song> songList) {
    this->songs = songList;
    this->numVertices = songList.size();
    
    // Initialize adjacency matrix with zeros
    adjacencyMatrix.resize(numVertices);
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i].resize(numVertices, 0.0);
    }
}

// Calculate edge weight using Euclidean distance
// Weight = sqrt((BPM1 - BPM2)^2 + (Energy1 - Energy2)^2 * 10000)
// Energy is scaled by 100 to balance with BPM range
double Graph::calculateWeight(int i, int j) {
    Song song1 = songs[i];
    Song song2 = songs[j];
    
    double bpmDiff = song1.bpm - song2.bpm;
    double energyDiff = (song1.energy - song2.energy) * 100.0; // Scale energy
    
    double weight = sqrt(bpmDiff * bpmDiff + energyDiff * energyDiff);
    
    return weight;
}

// Build the complete weighted graph
void Graph::buildGraph() {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (i != j) {
                adjacencyMatrix[i][j] = calculateWeight(i, j);
            }
        }
    }
}

// Get weight of edge between i and j
double Graph::getWeight(int i, int j) {
    if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
        return adjacencyMatrix[i][j];
    }
    return 0.0;
}

// Get number of vertices
int Graph::getNumVertices() {
    return numVertices;
}

// Get song by index
Song Graph::getSong(int index) {
    if (index >= 0 && index < numVertices) {
        return songs[index];
    }
    return Song();
}

// Get all songs
vector<Song> Graph::getSongs() {
    return songs;
}

// Print adjacency matrix for debugging
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