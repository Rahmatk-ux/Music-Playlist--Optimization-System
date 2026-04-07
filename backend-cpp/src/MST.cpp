#include "MST.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const double INF = numeric_limits<double>::infinity();

MST::MST(Graph *g)
{
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

vector<Edge> MST::primMST(int startVertex)
{
    vector<Edge> edges;
    vector<bool> inMST(numVertices, false);
    vector<double> key(numVertices, INF);
    vector<int> parent(numVertices, -1);

    key[startVertex] = 0.0;

    for (int count = 0; count < numVertices; count++)
    {
        double minKey = INF;
        int u = -1;

        for (int v = 0; v < numVertices; v++)
        {
            if (!inMST[v] && key[v] < minKey)
            {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1)
            break;

        inMST[u] = true;

        if (parent[u] != -1)
        {
            Edge edge = {parent[u], u, graph->getWeight(parent[u], u)};
            edges.push_back(edge);
        }

        for (int v = 0; v < numVertices; v++)
        {
            double weight = graph->getWeight(u, v);

            if (!inMST[v] && weight > 0 && weight < key[v])
            {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    mstEdges = edges;
    return edges;
}

vector<int> MST::mstToPlaylist(int startVertex)
{
    if (mstEdges.empty())
        return {startVertex};

    vector<vector<pair<int, double>>> adjList(numVertices);
    for (const Edge &edge : mstEdges)
    {
        adjList[edge.from].push_back({edge.to, edge.weight});
        adjList[edge.to].push_back({edge.from, edge.weight});
    }

    vector<int> playlist;
    vector<bool> visited(numVertices, false);

    int current = startVertex;
    playlist.push_back(current);
    visited[current] = true;

    while ((int)playlist.size() < numVertices)
    {
        double bestWeight = numeric_limits<double>::infinity();
        int bestNext = -1;

        for (size_t i = 0; i < adjList[current].size(); i++)
        {
            int neighbor = adjList[current][i].first;
            double weight = adjList[current][i].second;

            if (!visited[neighbor] && weight < bestWeight)
            {
                bestWeight = weight;
                bestNext = neighbor;
            }
        }

        if (bestNext == -1)
        {
            double minW = numeric_limits<double>::infinity();
            for (int v = 0; v < numVertices; v++)
            {
                if (!visited[v])
                {
                    double w = graph->getWeight(current, v);
                    if (w < minW)
                    {
                        minW = w;
                        bestNext = v;
                    }
                }
            }
        }

        if (bestNext == -1)
            break;
        visited[bestNext] = true;
        playlist.push_back(bestNext);
        current = bestNext;
    }

    return playlist;
}

double MST::getTotalWeight(vector<Edge> edges)
{
    double total = 0.0;
    for (const Edge &edge : edges)
    {
        total += edge.weight;
    }
    return total;
}

void MST::printResults(int startVertex)
{
    cout << "\n=== Prim's MST Algorithm Results ===" << endl;
    cout << "Start Vertex: " << startVertex
         << " (" << graph->getSong(startVertex).title << ")" << endl;

    vector<Edge> edges = primMST(startVertex);
    vector<int> playlist = mstToPlaylist(startVertex);
    double totalWeight = getTotalWeight(edges);

    cout << "\nMST Edges (" << edges.size() << " edges):" << endl;
    for (size_t i = 0; i < edges.size(); i++)
    {
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
    for (size_t i = 0; i < playlist.size(); i++)
    {
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