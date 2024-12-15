#include "SimpGraph.h"

// Constructor
SimpGraph::SimpGraph(int v, bool directed) : vertices(v), isDirected(directed), adjList(v), adjMatrix(v, vector<int>(v, 0)) {}

// Function to add an edge
void SimpGraph::addEdge(int u, int v, bool directed) {
    // Add edge to adjacency list
    adjList[u].push_back(v);
    if (!directed) {
        adjList[v].push_back(u); // For undirected graph
    }

    // Add edge to adjacency matrix
    adjMatrix[u][v] = 1;
    if (!directed) {
        adjMatrix[v][u] = 1; // For undirected graph
    }
}

// Function to print adjacency list
void SimpGraph::printAdjList() {
    cout << "Adjacency List:\n";
    for (int i = 0; i < vertices; ++i) {
        cout << i << ": ";
        for (int neighbor : adjList[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Function to print adjacency matrix
void SimpGraph::printAdjMatrix() {
    cout << "Adjacency Matrix:\n";
    cout << "    ";
    for (int i = 0; i < vertices; ++i) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i = 0; i < vertices; ++i) {
        cout << setw(3) << i << " ";
        for (int j = 0; j < vertices; ++j) {
            cout << setw(3) << adjMatrix[i][j];
        }
        cout << endl;
    }
}

// Function to print edge list
void SimpGraph::printEdgeList() {
    cout << "Edge List:\n";
    for (int u = 0; u < vertices; ++u) {
        for (int v : adjList[u]) {
            if (isDirected || u < v) { // To avoid duplicates in undirected graphs
                cout << "(" << u << ", " << v << ")\n";
            }
        }
    }
}

// Depth First Search (DFS)
void SimpGraph::DFS(int start) {
    vector<bool> visited(vertices, false);
    stack<int> s;
    s.push(start);

    cout << "DFS Traversal: ";
    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            for (auto it = adjList[node].rbegin(); it != adjList[node].rend(); ++it) {
                if (!visited[*it]) {
                    s.push(*it);
                }
            }
        }
    }
    cout << endl;
}

// Breadth First Search (BFS)
void SimpGraph::BFS(int start) {
    vector<bool> visited(vertices, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

// Cycle Detection in Undirected Graph
bool SimpGraph::isCyclic() {
    if (isDirected) {
        cout << "Cycle detection not implemented for directed graphs." << endl;
        return false;
    }

    vector<bool> visited(vertices, false);

    for (int i = 0; i < vertices; ++i) {
        if (!visited[i]) {
            if (isCyclicUtil(i, visited, -1)) {
                return true;
            }
        }
    }
    return false;
}

// Helper function for cycle detection
bool SimpGraph::isCyclicUtil(int v, vector<bool>& visited, int parent) {
    visited[v] = true;

    for (int neighbor : adjList[v]) {
        if (!visited[neighbor]) {
            if (isCyclicUtil(neighbor, visited, v)) {
                return true;
            }
        } else if (neighbor != parent) {
            return true;
        }
    }
    return false;
}
