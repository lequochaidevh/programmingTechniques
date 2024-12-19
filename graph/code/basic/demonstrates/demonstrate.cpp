#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip> // For formatted printing

using namespace std;

// Graph class
class Graph {
private:
    int vertices;                            // Number of vertices
    vector<vector<int>> adjList;            // Adjacency list representation
    vector<vector<int>> adjMatrix;          // Adjacency matrix representation

public:
    Graph(int v) : vertices(v), adjList(v), adjMatrix(v, vector<int>(v, 0)) {}

    void addEdge(int u, int v) {
        // Add edge to adjacency list
        adjList[u].push_back(v);
        adjList[v].push_back(u); // For undirected graph

        // Add edge to adjacency matrix
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1; // For undirected graph
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
                if (u < v) { // To avoid duplicates
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
};

int main() {
    int vertices = 6;
    Graph g(vertices);

    // Example graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
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
    if (g.isCyclic()) {
        cout << "The graph contains a cycle." << endl;
    } else {
        cout << "The graph does not contain a cycle." << endl;
    }

    return 0;
}

/*
g++ -o demonstrate demonstrate.cpp

output:
        Adjacency List:
        0: 1 2
        1: 0 3 4
        2: 0 5
        3: 1
        4: 1
        5: 2
        Adjacency Matrix:
              0  1  2  3  4  5
          0   0  1  1  0  0  0
          1   1  0  0  1  1  0
          2   1  0  0  0  0  1
          3   0  1  0  0  0  0
          4   0  1  0  0  0  0
          5   0  0  1  0  0  0
        Edge List:
        (0, 1)
        (0, 2)
        (1, 3)
        (1, 4)
        (2, 5)
        DFS Traversal: 0 1 3 4 2 5
        BFS Traversal: 0 1 2 3 4 5
        The graph does not contain a cycle.
*/

