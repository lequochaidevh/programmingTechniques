#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip> // For formatted printing

using namespace std;

// SimpGraph class
class SimpGraph {
private:
    int vertices;                            // Number of vertices
    bool isDirected;                        // Whether the graph is directed
    vector<vector<int>> adjList;            // Adjacency list representation
    vector<vector<int>> adjMatrix;          // Adjacency matrix representation

public:
    SimpGraph(int v, bool directed = false) : vertices(v), isDirected(directed), adjList(v), adjMatrix(v, vector<int>(v, 0)) {}

    void addEdge(int u, int v, bool directed = false) {
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

    void printAdjList() {
        cout << "Adjacency List:\n";
        for (int i = 0; i < vertices; ++i) {
            cout << i << ": ";
            for (int neighbor : adjList[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    void printAdjMatrix() {
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

    void printEdgeList() {
        cout << "Edge List:\n";
        for (int u = 0; u < vertices; ++u) {
            for (int v : adjList[u]) {
                if (isDirected || u < v) { // To avoid duplicates in undirected graphs
                    cout << "(" << u << ", " << v << ")\n";
                }
            }
        }
    }

    // 1. Depth First Search (DFS)
    void DFS(int start) {
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

    // 2. Breadth First Search (BFS)
    void BFS(int start) {
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

    // 3. Check for Cycle in Undirected Graph
    bool isCyclicUtil(int v, vector<bool>& visited, int parent) {
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

    bool isCyclic() {
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

    // 4. Count the number of connected components
    int connectedComponents() {
        vector<bool> visited(vertices, false);
        int count = 0;

        // Perform DFS for each unvisited node
        for (int i = 0; i < vertices; ++i) {
            if (!visited[i]) {
                DFSUtil(i, visited);  // DFS or BFS can be used here
                ++count;  // Increment the connected component count
            }
        }
        return count;
    }

    // Helper DFS function to mark all vertices in the same component
    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;

        // Visit all the neighbors of the current vertex
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                DFSUtil(neighbor, visited);
            }
        }
    }
};


int main() {
    int vertices = 6;
    bool isDirected = false; // Change to true for directed graph
    SimpGraph g(vertices, isDirected);

    // Example graph
    g.addEdge(0, 1);
    g.addEdge(0, 2, true);
    //g.addEdge(1, 3);
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

    // Count connected components
    cout << "The graph has " << g.connectedComponents() << " connected components." << endl;

    return 0;
}

//g++ -o r1_GraphTheory r1_GraphTheory.cpp

