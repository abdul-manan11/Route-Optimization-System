#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <limits>

using namespace std;

class Graph {
private:
    int vertices;                       // number of vertices
    vector<vector<double>> adjMatrix;   // adjacency matrix storing edge weights

public:
    Graph(int n = 0);
    int getVertices() const;
    void setVertices(int n);
    void addEdge(int u, int v, double weight);
    void removeEdge(int u, int v);
    double getWeight(int u, int v) const;
    bool hasEdge(int u, int v) const;
    const vector<vector<double>>& getAdjMatrix() const;
    void clear();
};

#endif
