#include "Kruskal.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// ── UnionFind ────────────────────────────────────────────────────────────────

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.resize(n, 0);
    // Each node is its own parent initially
    for (int i = 0; i < n; i++) parent[i] = i;
}

int UnionFind::find(int x) {
    // Path compression: make every node point directly to root
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX == rootY) return false; // already connected — would form cycle

    // Union by rank: attach smaller tree under larger tree
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    return true;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}

// ── Kruskal ──────────────────────────────────────────────────────────────────

Kruskal::Kruskal(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

vector<KEdge> Kruskal::runKruskal() {
    // Step 1: Collect ALL edges from the graph
    vector<KEdge> allEdges;
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) { // j > i avoids duplicates
            double w = graph->getWeight(i, j);
            if (w > 0) {
                allEdges.push_back({i, j, w});
            }
        }
    }

    // Step 2: Sort all edges by weight ascending
    sort(allEdges.begin(), allEdges.end());

    // Step 3: Greedily add cheapest edges that don't form cycles
    UnionFind uf(numVertices);
    vector<KEdge> result;

    for (const KEdge& edge : allEdges) {
        // If from and to are in different components, add this edge
        if (uf.unite(edge.from, edge.to)) {
            result.push_back(edge);

            // MST has exactly V-1 edges — stop early
            if ((int)result.size() == numVertices - 1) break;
        }
        // Otherwise skip — would create a cycle
    }

    mstEdges = result;
    return result;
}

vector<int> Kruskal::mstToPlaylist(int startVertex) {
    if (mstEdges.empty()) runKruskal();

    // Build adjacency list from MST edges
    vector<vector<pair<int, double>>> adjList(numVertices);
    for (const KEdge& edge : mstEdges) {
        adjList[edge.from].push_back({edge.to,   edge.weight});
        adjList[edge.to].push_back  ({edge.from, edge.weight});
    }

    // Greedy traversal: always pick cheapest MST neighbor
    vector<int> playlist;
    vector<bool> visited(numVertices, false);

    int current = startVertex;
    playlist.push_back(current);
    visited[current] = true;

    while ((int)playlist.size() < numVertices) {
        double bestWeight = numeric_limits<double>::infinity();
        int bestNext = -1;

        // Check MST neighbors first
        for (size_t k = 0; k < adjList[current].size(); k++) {
            int    neighbor = adjList[current][k].first;
            double weight   = adjList[current][k].second;
            if (!visited[neighbor] && weight < bestWeight) {
                bestWeight = weight;
                bestNext   = neighbor;
            }
        }

        // If no MST neighbor available, jump to globally nearest unvisited
        if (bestNext == -1) {
            for (int v = 0; v < numVertices; v++) {
                if (!visited[v]) {
                    double w = graph->getWeight(current, v);
                    if (w < bestWeight) {
                        bestWeight = w;
                        bestNext   = v;
                    }
                }
            }
        }

        if (bestNext == -1) break;
        visited[bestNext] = true;
        playlist.push_back(bestNext);
        current = bestNext;
    }

    return playlist;
}

double Kruskal::getTotalWeight(const vector<KEdge>& edges) {
    double total = 0.0;
    for (const KEdge& e : edges) total += e.weight;
    return total;
}

void Kruskal::printResults(int startVertex) {
    cout << "\n=== Kruskal's MST Algorithm Results ===" << endl;
    cout << "Start Vertex: " << startVertex
         << " (" << graph->getSong(startVertex).title << ")" << endl;

    vector<KEdge> edges   = runKruskal();
    vector<int>   playlist = mstToPlaylist(startVertex);
    double        total    = getTotalWeight(edges);

    cout << "\nMST Edges (" << edges.size() << " edges, V-1=" << numVertices-1 << "):" << endl;
    for (size_t i = 0; i < edges.size(); i++) {
        Song from = graph->getSong(edges[i].from);
        Song to   = graph->getSong(edges[i].to);
        cout << setw(2) << (i+1) << ". "
             << setw(25) << left << from.title << " -> "
             << setw(25) << left << to.title
             << " (Cost: " << fixed << setprecision(1) << edges[i].weight << ")" << endl;
    }

    cout << "\nTotal MST Weight: " << fixed << setprecision(2) << total << endl;
    cout << "Playlist order: DFS from vertex " << startVertex << endl;
}