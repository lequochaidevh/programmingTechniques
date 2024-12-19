#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Global variables
vector<int> parent;
vector<int> rootSize;
vector<int> vertices;
vector<Edge> edges;

// Structure to represent an edge
typedef struct Edge {
    int src, dest, weight;
} Edge;

// Function to compare edges by weight
bool compareEdges(Edge e1, Edge e2) {
    return e1.weight < e2.weight;
}

// Find function for Disjoint Set Union (DSU)
int find(int vertex) {
    if (parent[vertex] != vertex) {
        parent[vertex] = find(parent[vertex]);
    }
    return parent[vertex];
}

// Union function for DSU
void unionSets(int root1, int root2) {
    if (rootSize[root1] < rootSize[root2]) {
        parent[root1] = root2;
    } else if (rootSize[root1] > rootSize[root2]) {
        parent[root2] = root1;
    } else {
        parent[root2] = root1;
        rootSize[root1]++;
    }
}

// Kruskal's Algorithm
void kruskal() {
    // Sort edges by weight
    sort(edges.begin(), edges.end(), compareEdges);

    // Initialize DSU
    parent.resize(vertices.size());
    rootSize.resize(vertices.size(), 0);
    for (int i = 0; i < vertices.size(); i++) {
        parent[i] = i;
    }

    vector<Edge> mst; // Store edges of the MST

    // Iterate through edges and build MST
    for (Edge edge : edges) {
        int root1 = find(edge.src);
        int root2 = find(edge.dest);

        // If adding this edge doesn't form a cycle
        if (root1 != root2) {
            mst.push_back(edge);
            unionSets(root1, root2);
        }
    }

    // Print the MST
    cout << "Edges in the MST:\n";
    for (Edge edge : mst) {
        cout << edge.src << " -- " << edge.dest << " == " << edge.weight << endl;
    }
}

int main() {
    vertices = 4; // Number of vertices
    edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    kruskal();

    return 0;
}
