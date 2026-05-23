#include "Graph.h"

const double INF = 1e9;

Graph::Graph(int n) : vertices(n) {
    adjMatrix.assign(n, vector<double>(n, INF));
    for (int i = 0; i < n; ++i) {
        adjMatrix[i][i] = 0.0;
    }
}

int Graph::getVertices() const {
    return vertices;
}

void Graph::setVertices(int n) {
    vertices = n;
    adjMatrix.assign(n, vector<double>(n, INF));
    for (int i = 0; i < n; ++i) {
        adjMatrix[i][i] = 0.0;
    }
}

void Graph::addEdge(int u, int v, double weight) {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        adjMatrix[u][v] = weight;
    }
}

void Graph::removeEdge(int u, int v) {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        adjMatrix[u][v] = INF;
    }
}

double Graph::getWeight(int u, int v) const {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        return adjMatrix[u][v];
    }
    return INF;
}

bool Graph::hasEdge(int u, int v) const {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        return adjMatrix[u][v] < INF && adjMatrix[u][v] > 0;
    }
    return false;
}

const vector<vector<double>>& Graph::getAdjMatrix() const {
    return adjMatrix;
}

void Graph::clear() {
    adjMatrix.clear();
    vertices = 0;
}
