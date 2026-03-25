#include "TSP.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// Constructor
TSP::TSP(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

// Greedy TSP - Nearest Neighbor Heuristic
// Time Complexity: O(V^2)
// Space Complexity: O(V)
vector<int> TSP::greedyTSP(int startVertex) {
    vector<int> tour;
    vector<bool> visited(numVertices, false);
    
    int current = startVertex;
    tour.push_back(current);
    visited[current] = true;
    
    // Visit all vertices
    for (int i = 1; i < numVertices; i++) {
        double minWeight = INF;
        int nearestVertex = -1;
        
        // Find nearest unvisited neighbor
        for (int v = 0; v < numVertices; v++) {
            if (!visited[v]) {
                double weight = graph->getWeight(current, v);
                if (weight < minWeight) {
                    minWeight = weight;
                    nearestVertex = v;
                }
            }
        }
        
        if (nearestVertex != -1) {
            tour.push_back(nearestVertex);
            visited[nearestVertex] = true;
            current = nearestVertex;
        }
    }
    
    return tour;
}

// Calculate total tour cost
double TSP::getTourCost(vector<int> tour) {
    double totalCost = 0.0;
    
    for (size_t i = 0; i < tour.size() - 1; i++) {
        totalCost += graph->getWeight(tour[i], tour[i + 1]);
    }
    
    return totalCost;
}

// Print TSP results
void TSP::printResults(int startVertex) {
    cout << "\n=== Greedy TSP (Nearest Neighbor) Results ===" << endl;
    cout << "Start Vertex: " << startVertex 
         << " (" << graph->getSong(startVertex).title << ")" << endl;
    
    vector<int> tour = greedyTSP(startVertex);
    double totalCost = getTourCost(tour);
    
    cout << "\nTSP Tour (" << tour.size() << " songs):" << endl;
    for (size_t i = 0; i < tour.size(); i++) {
        Song song = graph->getSong(tour[i]);
        cout << setw(2) << (i + 1) << ". " 
             << setw(30) << left << song.title 
             << " (" << song.bpm << " BPM, " 
             << fixed << setprecision(0) << (song.energy * 100) << "% Energy)";
        
        if (i < tour.size() - 1) {
            double transitionCost = graph->getWeight(tour[i], tour[i + 1]);
            cout << " → Cost: " << fixed << setprecision(1) << transitionCost;
        }
        cout << endl;
    }
    
    cout << "\nTotal Tour Cost: " << fixed << setprecision(2) << totalCost << endl;
    cout << "Average Cost per Transition: " 
         << fixed << setprecision(2) << (totalCost / (tour.size() - 1)) << endl;
}