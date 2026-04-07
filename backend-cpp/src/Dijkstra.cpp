#include "Dijkstra.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();

Dijkstra::Dijkstra(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

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

void Dijkstra::run(int startVertex) {
    distances.assign(numVertices, INF);
    visited.assign(numVertices, false);
    previous.assign(numVertices, -1);
    
    distances[startVertex] = 0.0;
    
    for (int i = 0; i < numVertices; i++) {
        int u = findMinDistance();
        
        if (u == -1) break; 
        
        visited[u] = true;
        
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

vector<int> Dijkstra::getOptimizedPath(int startVertex) {
    // Step 1: Run real Dijkstra
    run(startVertex);
    
    vector<pair<double, int>> distIndexPairs;
    for (int v = 0; v < numVertices; v++) {
        distIndexPairs.push_back({distances[v], v});
    }
    sort(distIndexPairs.begin(), distIndexPairs.end());
    vector<int> path;
    for (auto& p : distIndexPairs) {
        path.push_back(p.second);
    }
    
    return path;
}

double Dijkstra::getTotalDistance(vector<int> path) {
    double totalDist = 0.0;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        totalDist += graph->getWeight(path[i], path[i + 1]);
    }
    
    return totalDist;
}

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
         << fixed << setprecision(2) << (totalDist / (path.size() -1)) << endl;
}