#include "Dijkstra.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// Constructor
Dijkstra::Dijkstra(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

// Find vertex with minimum distance among unvisited vertices
int Dijkstra::findMinDistance() {
    double minDist = INF;
    int minVertex = -1;
    
    for (int v = 0; v < numVertices; v++) {
        if (!visited[v] && distances[v] < minDist) {
            minDist = distances[v];
            minVertex = v;
        }
    }
    
    return minVertex;
}

// Run Dijkstra's algorithm from start vertex
void Dijkstra::run(int startVertex) {
    // Initialize distances and visited arrays
    distances.assign(numVertices, INF);
    visited.assign(numVertices, false);
    previous.assign(numVertices, -1);
    
    distances[startVertex] = 0.0;
    
    // Find shortest path for all vertices
    for (int i = 0; i < numVertices; i++) {
        int u = findMinDistance();
        
        if (u == -1) break; // All reachable vertices visited
        
        visited[u] = true;
        
        // Update distances to neighbors
        for (int v = 0; v < numVertices; v++) {
            double weight = graph->getWeight(u, v);
            
            if (!visited[v] && weight > 0) {
                double newDist = distances[u] + weight;
                
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    previous[v] = u;
                }
            }
        }
    }
}

// Get optimized path - creates path visiting all vertices
// by repeatedly choosing nearest unvisited vertex
vector<int> Dijkstra::getOptimizedPath(int startVertex) {
    vector<int> path;
    vector<bool> inPath(numVertices, false);
    
    int current = startVertex;
    path.push_back(current);
    inPath[current] = true;
    
    // Build path by always choosing nearest unvisited neighbor
    for (int i = 1; i < numVertices; i++) {
        double minWeight = INF;
        int nextVertex = -1;
        
        for (int v = 0; v < numVertices; v++) {
            if (!inPath[v]) {
                double weight = graph->getWeight(current, v);
                if (weight < minWeight) {
                    minWeight = weight;
                    nextVertex = v;
                }
            }
        }
        
        if (nextVertex != -1) {
            path.push_back(nextVertex);
            inPath[nextVertex] = true;
            current = nextVertex;
        }
    }
    
    return path;
}

// Get total distance of a path
double Dijkstra::getTotalDistance(vector<int> path) {
    double totalDist = 0.0;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        totalDist += graph->getWeight(path[i], path[i + 1]);
    }
    
    return totalDist;
}

// Print results
void Dijkstra::printResults(int startVertex) {
    cout << "\n=== Dijkstra's Algorithm Results ===" << endl;
    cout << "Start Vertex: " << startVertex 
         << " (" << graph->getSong(startVertex).title << ")" << endl;
    
    vector<int> path = getOptimizedPath(startVertex);
    double totalDist = getTotalDistance(path);
    
    cout << "\nOptimized Path (" << path.size() << " songs):" << endl;
    for (size_t i = 0; i < path.size(); i++) {
        Song song = graph->getSong(path[i]);
        cout << setw(2) << (i + 1) << ". " 
             << setw(30) << left << song.title 
             << " (" << song.bpm << " BPM, " 
             << fixed << setprecision(0) << (song.energy * 100) << "% Energy)";
        
        if (i < path.size() - 1) {
            double transitionCost = graph->getWeight(path[i], path[i + 1]);
            cout << " → Cost: " << fixed << setprecision(1) << transitionCost;
        }
        cout << endl;
    }
    
    cout << "\nTotal Transition Cost: " << fixed << setprecision(2) << totalDist << endl;
    cout << "Average Cost per Transition: " 
         << fixed << setprecision(2) << (totalDist / (path.size() - 1)) << endl;
}