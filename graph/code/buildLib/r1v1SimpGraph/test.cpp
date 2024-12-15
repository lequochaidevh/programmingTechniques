#include <iostream>
#include "SimpGraph.h"

int main() {
    int vertices = 6;
    bool isDirected = false; // Change to true for directed graph
    SimpGraph g(vertices, isDirected);

    // Example graph
    g.addEdge(0, 1);
    g.addEdge(0, 2, true);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // Print graph representations
    g.printAdjList();
    g.printAdjMatrix();
    g.printEdgeList();

    // Perform DFS
    g.DFS(0);

    // Perform BFS
    g.BFS(0);

    // Check if the graph contains a cycle
    if (!isDirected && g.isCyclic()) {
        cout << "The graph contains a cycle." << endl;
    } else {
        cout << "The graph does not contain a cycle." << endl;
    }

    return 0;
}
