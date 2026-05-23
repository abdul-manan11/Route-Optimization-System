#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <utility>
#include "Graph.h"

using namespace std;

class Algorithms {
public:
    // Returns (path, totalCost)
    static pair<vector<int>, double> bruteForce(const Graph& graph, int start);
    static pair<vector<int>, double> greedySearch(const Graph& graph, int start);
    static pair<vector<int>, double> mananAlgorithm(const Graph& graph, int start);
    // Dijkstra from source to destination
    static pair<vector<int>, double> dijkstra(const Graph& graph, int source, int destination);
};

#endif
