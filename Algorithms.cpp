#include "Algorithms.h"
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

const double INF = 1e9;

pair<vector<int>, double> Algorithms::bruteForce(const Graph& graph, int start) {
    int n = graph.getVertices();
    vector<int> path;
    double totalCost = 0.0;

    for (int i = 0; i < n; ++i) {
        path.push_back((start + i) % n);
    }
    path.push_back(start);

    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalCost += graph.getWeight(path[i], path[i + 1]);
    }
    return {path, totalCost};
}

pair<vector<int>, double> Algorithms::greedySearch(const Graph& graph, int start) {
    int n = graph.getVertices();
    vector<bool> visited(n, false);
    vector<int> path;
    double totalCost = 0.0;

    int curr = start;
    path.push_back(curr);
    visited[curr] = true;

    for (int i = 1; i < n; ++i) {
        int next = -1;
        double minDist = INF;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && graph.hasEdge(curr, j) && graph.getWeight(curr, j) < minDist) {
                minDist = graph.getWeight(curr, j);
                next = j;
            }
        }
        if (next != -1) {
            path.push_back(next);
            totalCost += minDist;
            visited[next] = true;
            curr = next;
        }
    }
    // Return to start
    totalCost += graph.getWeight(curr, start);
    path.push_back(start);
    return {path, totalCost};
}

pair<vector<int>, double> Algorithms::mananAlgorithm(const Graph& graph, int start) {
    int n = graph.getVertices();
    // Step 1: Greedy nearest neighbor (same as greedySearch)
    auto greedyResult = greedySearch(graph, start);
    vector<int> path = greedyResult.first;

    bool improved = true;
    while (improved) {
        improved = false;
        for (size_t i = 1; i < path.size() - 2; ++i) {
            for (size_t j = i + 1; j < path.size() - 1; ++j) {
                double currentDist = graph.getWeight(path[i - 1], path[i]) +
                                     graph.getWeight(path[j], path[j + 1]);
                double newDist = graph.getWeight(path[i - 1], path[j]) +
                                 graph.getWeight(path[i], path[j + 1]);
                if (newDist < currentDist) {
                    reverse(path.begin() + i, path.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    double totalCost = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalCost += graph.getWeight(path[i], path[i + 1]);
    }
    return {path, totalCost};
}

pair<vector<int>, double> Algorithms::dijkstra(const Graph& graph, int source, int destination) {
    int n = graph.getVertices();
    vector<double> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    dist[source] = 0;
    pq.push({0.0, source});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[u]) continue;
        if (u == destination) break;

        for (int v = 0; v < n; ++v) {
            if (graph.hasEdge(u, v)) {
                double w = graph.getWeight(u, v);
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    vector<int> path;
    if (dist[destination] >= INF) {
        return {path, INF}; // no path
    }
    for (int at = destination; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return {path, dist[destination]};
}
