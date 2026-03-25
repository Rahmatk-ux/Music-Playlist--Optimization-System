#include "MST.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// Constructor
MST::MST(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

// Prim's MST Algorithm
// Time Complexity: O(V^2) with adjacency matrix
// Space Complexity: O(V)
vector<Edge> MST::primMST(int startVertex) {
    vector<Edge> edges;
    vector<bool> inMST(numVertices, false);
    vector<double> key(numVertices, INF);
    vector<int> parent(numVertices, -1);
    
    key[startVertex] = 0.0;
    
    // Build MST with V-1 edges
    for (int count = 0; count < numVertices; count++) {
        // Find vertex with minimum key value
        double minKey = INF;
        int u = -1;
        
        for (int v = 0; v < numVertices; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        inMST[u] = true;
        
        // Add edge to MST (except for start vertex)
        if (parent[u] != -1) {
            Edge edge = {parent[u], u, graph->getWeight(parent[u], u)};
            edges.push_back(edge);
        }
        
        // Update key values of adjacent vertices
        for (int v = 0; v < numVertices; v++) {
            double weight = graph->getWeight(u, v);
            
            if (!inMST[v] && weight > 0 && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }
    
    mstEdges = edges;
    return edges;
}

// Convert MST to playlist using DFS traversal
vector<int> MST::mstToPlaylist(int startVertex) {
    vector<int> playlist;
    vector<bool> visited(numVertices, false);
    
    // Build adjacency list from MST edges
    vector<vector<int>> adjList(numVertices);
    for (const Edge& edge : mstEdges) {
        adjList[edge.from].push_back(edge.to);
        adjList[edge.to].push_back(edge.from);
    }
    
    // DFS traversal
    vector<int> stack;
    stack.push_back(startVertex);
    visited[startVertex] = true;
    
    while (!stack.empty()) {
        int current = stack.back();
        stack.pop_back();
        playlist.push_back(current);
        
        for (int neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                stack.push_back(neighbor);
            }
        }
    }
    
    return playlist;
}

// Get total weight of MST
double MST::getTotalWeight(vector<Edge> edges) {
    double total = 0.0;
    for (const Edge& edge : edges) {
        total += edge.weight;
    }
    return total;
}

// Print MST results
void MST::printResults(int startVertex) {
    cout << "\n=== Prim's MST Algorithm Results ===" << endl;
    cout << "Start Vertex: " << startVertex 
         << " (" << graph->getSong(startVertex).title << ")" << endl;
    
    vector<Edge> edges = primMST(startVertex);
    vector<int> playlist = mstToPlaylist(startVertex);
    double totalWeight = getTotalWeight(edges);
    
    cout << "\nMST Edges (" << edges.size() << " edges):" << endl;
    for (size_t i = 0; i < edges.size(); i++) {
        Song from = graph->getSong(edges[i].from);
        Song to = graph->getSong(edges[i].to);
        cout << setw(2) << (i + 1) << ". " 
             << setw(25) << left << from.title 
             << " → " 
             << setw(25) << left << to.title 
             << " (Cost: " << fixed << setprecision(1) << edges[i].weight << ")" 
             << endl;
    }
    
    cout << "\nPlaylist Order (DFS Traversal):" << endl;
    for (size_t i = 0; i < playlist.size(); i++) {
        Song song = graph->getSong(playlist[i]);
        cout << setw(2) << (i + 1) << ". " 
             << setw(30) << left << song.title 
             << " (" << song.bpm << " BPM, " 
             << fixed << setprecision(0) << (song.energy * 100) << "% Energy)" 
             << endl;
    }
    
    cout << "\nTotal MST Weight: " << fixed << setprecision(2) << totalWeight << endl;
    cout << "Average Edge Weight: " 
         << fixed << setprecision(2) << (totalWeight / edges.size()) << endl;
}