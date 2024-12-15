#ifndef SIMPGRAPH_H
#define SIMPGRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip> // For formatted printing

using namespace std;

class SimpGraph {
private:
    int vertices;                            // Number of vertices
    bool isDirected;                        // Whether the graph is directed
    vector<vector<int>> adjList;            // Adjacency list representation
    vector<vector<int>> adjMatrix;          // Adjacency matrix representation

public:
    // Constructor
    SimpGraph(int v, bool directed = false);

    // Functions to add edge
    void addEdge(int u, int v, bool directed = false);

    // Functions to print graph representations
    void printAdjList();
    void printAdjMatrix();
    void printEdgeList();

    // Depth First Search (DFS)
    void DFS(int start);

    // Breadth First Search (BFS)
    void BFS(int start);

    // Cycle Detection in Undirected Graph
    bool isCyclic();
    
private:
    bool isCyclicUtil(int v, vector<bool>& visited, int parent);
};

#endif // SIMPGRAPH_H
