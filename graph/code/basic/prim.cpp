/*
Graph Representation: The graph is represented as an adjacency matrix where graph[i][j] gives the weight of the edge between vertices i and j. If there's no edge, the value is 0.
Priority Queue: A priority queue (min-heap) is used to always pick the edge with the smallest weight that connects a vertex not yet included in the MST.
Prim's Algorithm:
Start from an arbitrary vertex (0th vertex in this case).
Include it in the MST and explore all adjacent vertices.
For each adjacent vertex, if it is not already in the MST, push it into the priority queue with the edge weight.
Continue until all vertices are included in the MST.
Result: The edges selected for the MST are printed.
*/

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define V 5  // Number of vertices in the graph

// A structure to represent a weighted edge in the graph
struct Edge {
    int dest;
    int weight;
    bool operator>(const Edge &e) const {
        return weight > e.weight; // weight shorter have higher priory (pop first)
    }
};

// Function to implement Prim's Algorithm
void prim(int graph[V][V]) {
    // Priority queue to pick the minimum weight edge
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
    // ex : [{w=6, d=5};{w=5, dest=1};{w=3, d=2};{weight=2, dest=5}] -> front ->
    // using weight for compare; can duplicate vertexDestination but each have diffirence weight 
    
    // Array to track vertices included in MST
    bool inMST[V] = {false};

    // Vector to store the result (MST)
    vector<pair<int, int>> mstEdges; // (parent, child)

    // Start with the first vertex (0th index)
    inMST[0] = true;
    for (int i = 0; i < V; ++i) {
        if (graph[0][i] != 0) {
            pq.push(Edge{i, graph[0][i]});
        }
    }

    // Prim's Algorithm: finding the MST
    while (!pq.empty()) {
        Edge current = pq.top();
        pq.pop();

        int u = current.dest;

        if (inMST[u]) continue; // Skip if already included in MST

        // Include vertex u in MST
        inMST[u] = true;
        mstEdges.push_back({0, u});

        // Add all the adjacent edges to the priority queue
        for (int i = 0; i < V; ++i) {
            if (graph[u][i] != 0 && !inMST[i]) {
                pq.push(Edge{i, graph[u][i]});
            }
        }
    }

    // Output the result (MST edges)
    cout << "Edges in the Minimum Spanning Tree (MST):\n";
    for (const auto &edge : mstEdges) {
        cout << edge.first << " - " << edge.second << endl;
    }
}

void prim2(int graph[V][V]) {
    // Array to track vertices included in MST
    bool inMST[V] = {false};

    // Vector to store the result (MST)
    vector<pair<int, int>> mstEdges; // (parent, child)

    // Start with the first vertex (0th index)
    inMST[0] = true;

    // Loop until the MST includes all vertices
    for (int count = 1; count < V; ++count) {
        int minWeight = INT_MAX;
        int u = -1, v = -1;

        // Find the minimum weight edge that connects a vertex in MST to a vertex not in MST
        for (int i = 0; i < V; ++i) {
            if (inMST[i]) {
                for (int j = 0; j < V; ++j) {
                    if (!inMST[j] && graph[i][j] != 0 && graph[i][j] < minWeight) {
                        minWeight = graph[i][j];
                        u = i;
                        v = j;
                    }
                }
            }
        }

        // Include vertex v in MST
        inMST[v] = true;
        mstEdges.push_back({u, v});
    }

    // Output the result (MST edges)
    cout << "Edges in the Minimum Spanning Tree (MST):\n";
    for (const auto &edge : mstEdges) {
        cout << edge.first << " - " << edge.second << endl;
    }
}


int main() {
    // Example graph represented as an adjacency matrix
    int graph[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };

    prim(graph);  // Call the function to find MST using Prim's Algorithm
    //prim2(graph); // simple
    return 0;
}
