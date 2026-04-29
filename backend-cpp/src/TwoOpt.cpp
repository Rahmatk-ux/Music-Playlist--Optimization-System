#include "TwoOpt.h"
#include "TSP.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

const double INF_2OPT = numeric_limits<double>::infinity();

TwoOpt::TwoOpt(Graph* g) {
    this->graph = g;
    this->numVertices = g->getNumVertices();
}

double TwoOpt::tourCost(const vector<int>& tour) {
    double cost = 0.0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        cost += graph->getWeight(tour[i], tour[i + 1]);
    }
    return cost;
}

void TwoOpt::reverseSegment(vector<int>& tour, int i, int j) {
    while (i < j) {
        swap(tour[i], tour[j]);
        i++;
        j--;
    }
}

vector<int> TwoOpt::improve(vector<int> tour) {
    bool improved = true;
    int iterations = 0;
    const int MAX_ITER = 1000; // safety limit

    while (improved && iterations < MAX_ITER) {
        improved = false;
        iterations++;

        for (int i = 1; i < numVertices - 1; i++) {
            for (int j = i + 1; j < numVertices; j++) {

                // Current edges: tour[i-1]->tour[i] and tour[j]->tour[j+1]
                // Proposed:      tour[i-1]->tour[j] and tour[i]->tour[j+1]
                int nextJ = (j + 1 < numVertices) ? j + 1 : 0;

                double currentCost  = graph->getWeight(tour[i - 1], tour[i])
                                    + graph->getWeight(tour[j],     tour[nextJ]);

                double proposedCost = graph->getWeight(tour[i - 1], tour[j])
                                    + graph->getWeight(tour[i],     tour[nextJ]);

                // If reversing segment i..j improves cost, do it
                if (proposedCost < currentCost - 1e-10) {
                    reverseSegment(tour, i, j);
                    improved = true;
                }
            }
        }
    }

    return tour;
}

vector<int> TwoOpt::run(int startVertex) {
    // Step 1: Get initial greedy tour from TSP
    TSP tsp(graph);
    vector<int> initialTour = tsp.greedyTSP(startVertex);

    // Step 2: Improve with 2-opt
    vector<int> improvedTour = improve(initialTour);

    return improvedTour;
}

double TwoOpt::getTourCost(const vector<int>& tour) {
    return tourCost(tour);
}

void TwoOpt::printResults(int startVertex) {
    cout << "\n=== 2-opt TSP Improvement Results ===" << endl;
    cout << "Start Vertex: " << startVertex
         << " (" << graph->getSong(startVertex).title << ")" << endl;

    TSP tsp(graph);
    vector<int> greedyTour  = tsp.greedyTSP(startVertex);
    double greedyCost       = tsp.getTourCost(greedyTour);

    vector<int> improvedTour = improve(greedyTour);
    double improvedCost      = getTourCost(improvedTour);

    double improvement = ((greedyCost - improvedCost) / greedyCost) * 100.0;

    cout << "\nGreedy TSP cost:   " << fixed << setprecision(2) << greedyCost   << endl;
    cout << "2-opt improved cost: " << fixed << setprecision(2) << improvedCost << endl;
    cout << "Improvement:         " << fixed << setprecision(1) << improvement  << "%" << endl;

    cout << "\nImproved Tour (" << improvedTour.size() << " songs):" << endl;
    for (size_t i = 0; i < improvedTour.size(); i++) {
        Song song = graph->getSong(improvedTour[i]);
        cout << setw(2) << (i + 1) << ". "
             << setw(30) << left << song.title
             << " (" << song.bpm << " BPM, "
             << fixed << setprecision(0) << (song.energy * 100) << "% Energy)";
        if (i < improvedTour.size() - 1) {
            double cost = graph->getWeight(improvedTour[i], improvedTour[i + 1]);
            cout << " -> Cost: " << fixed << setprecision(1) << cost;
        }
        cout << endl;
    }
}
