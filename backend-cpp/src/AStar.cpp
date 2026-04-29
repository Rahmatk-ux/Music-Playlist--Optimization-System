#include "AStar.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <queue>
#include <cmath>

using namespace std;

const double INF_ASTAR = numeric_limits<double>::infinity();

AStar::AStar(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

double AStar::heuristic(int v, int goal) {
    // Admissible heuristic: Euclidean distance in BPM-energy space
    // This NEVER overestimates actual cost (admissible) so A* is optimal
    Song sv   = graph->getSong(v);
    Song sg   = graph->getSong(goal);
    double bpmDiff    = sv.bpm    - sg.bpm;
    double energyDiff = (sv.energy - sg.energy) * 100.0;
    return sqrt(bpmDiff * bpmDiff + energyDiff * energyDiff);
}

vector<int> AStar::findPath(int start, int goal) {
    // f(n) = g(n) + h(n)
    // g(n) = actual cost from start to n
    // h(n) = heuristic estimate from n to goal
    vector<double> gCost(numVertices, INF_ASTAR); // actual cost from start
    vector<double> fCost(numVertices, INF_ASTAR); // g + h
    vector<int>    previous(numVertices, -1);
    vector<bool>   closed(numVertices, false);    // processed nodes

    gCost[start] = 0.0;
    fCost[start] = heuristic(start, goal);

    // Min-heap: pair<fCost, vertex>
    priority_queue<
        pair<double, int>,
        vector<pair<double, int>>,
        greater<pair<double, int>>
    > openSet;

    openSet.push({fCost[start], start});

    while (!openSet.empty()) {
        auto top     = openSet.top();
        double f     = top.first;
        int current  = top.second;
        openSet.pop();

        // Skip if already processed
        if (closed[current]) continue;
        closed[current] = true;

        // Goal reached — reconstruct path
        if (current == goal) {
            vector<int> path;
            for (int v = goal; v != -1; v = previous[v]) {
                path.push_back(v);
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors
        for (int v = 0; v < numVertices; v++) {
            if (closed[v]) continue;

            double weight = graph->getWeight(current, v);
            if (weight <= 0) continue;

            double tentativeG = gCost[current] + weight;

            if (tentativeG < gCost[v]) {
                gCost[v]    = tentativeG;
                fCost[v]    = tentativeG + heuristic(v, goal);
                previous[v] = current;
                openSet.push({fCost[v], v});
            }
        }
    }

    // No path found — return just start
    return {start};
}

double AStar::getPathCost(const vector<int>& path) {
    double cost = 0.0;
    for (size_t i = 0; i < path.size() - 1; i++) {
        cost += graph->getWeight(path[i], path[i + 1]);
    }
    return cost;
}

void AStar::printResults(int start, int goal) {
    cout << "\n=== A* Search Results ===" << endl;
    cout << "Start: " << graph->getSong(start).title << endl;
    cout << "Goal:  " << graph->getSong(goal).title  << endl;

    vector<int> path = findPath(start, goal);
    double cost      = getPathCost(path);

    cout << "\nPath (" << path.size() << " songs):" << endl;
    for (size_t i = 0; i < path.size(); i++) {
        Song song = graph->getSong(path[i]);
        cout << setw(2) << (i+1) << ". "
             << setw(30) << left << song.title
             << " (" << song.bpm << " BPM, "
             << fixed << setprecision(0) << (song.energy * 100) << "% energy)";
        if (i < path.size() - 1) {
            cout << " -> " << fixed << setprecision(1)
                 << graph->getWeight(path[i], path[i+1]);
        }
        cout << endl;
    }
    cout << "\nTotal Path Cost: " << fixed << setprecision(2) << cost << endl;
}