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
    vector<pair<int, bool>> visited;
    vector<pair<int, int>> parentList;  // To store parent of each vertex
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

    void cleanComputeVals(){
        memset(visited, false, visited.size());

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

    // 4. Find path between two vertices using DFS
    bool findPath(int vertex1, int vertex2) {
        vector<bool> visited(vertices, false);
        vector<int> parentList(vertices, -1);  // To store parent of each vertex

        if (DFSUtilForPath(vertex1, visited, parentList, vertex2)) {
            // Reconstruct the path from vertex2 to vertex1 using parentList
            vector<int> path;
            for (int v = vertex2; v != -1; v = parentList[v]) {
                path.push_back(v);
            }
            reverse(path.begin(), path.end());

            // Print the path
            cout << "Path from " << vertex1 << " to " << vertex2 << ": ";
            for (int v : path) {
                cout << v << " ";
            }
            cout << endl;

            return true;
        } else {
            cout << "No path found from " << vertex1 << " to " << vertex2 << endl;
            return false;
        }
    }

    // Helper function to perform DFS and mark parentList
    bool DFSUtilForPath(int v, vector<bool>& visited, vector<int>& parentList, int target) {
        visited[v] = true;

        if (v == target) return true;

        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                parentList[neighbor] = v;  // Mark parent
                if (DFSUtilForPath(neighbor, visited, parentList, target)) {
                    return true;
                }
            }
        }
        return false;
    }
};

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
    g.printAdjMatrix();
    g.printEdgeList();

    // Find path from vertex 0 to vertex 5
    g.findPath(0, 5);



    return 0;
}


//g++ -o r1_GraphTheory r1_GraphTheory.cpp
/*
* input:
0 1 1 0 0 0
1 0 0 1 1 0
0 0 0 0 0 1
0 1 1 0 0 0
0 1 0 0 0 0
0 0 1 0 0 0
* output:
    // Input type
    std::cin >> n >> m;

*/
