#include <iostream>
#include <vector>
#include <stack>

using namespace std;

const int MAX = 1000; // Maximum number of vertices
vector<vector<int>> graph(MAX);           // Adjacency list for the graph
vector<vector<int>> reversedGraph(MAX);   // Adjacency list for the reversed graph
vector<bool> visited(MAX, false);         // Visited array
stack<int> Stack;                         // Stack to store the finishing order

void dfs(int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor);
        }
    }
    Stack.push(node);
}

void reverseDFS(int node) {
    visited[node] = true;
    cout << node << " ";
    for (int neighbor : reversedGraph[node]) {
        if (!visited[neighbor]) {
            reverseDFS(neighbor);
        }
    }
}

int main() {
    int V = 5; // Number of vertices

    // Adding edges
    graph[0].push_back(2);
    graph[2].push_back(1);
    graph[1].push_back(0);
    graph[0].push_back(3);
    graph[3].push_back(4);

    // Creating the reversed graph
    for (int u = 0; u < V; ++u) {
        for (int v : graph[u]) {
            reversedGraph[v].push_back(u);
        }
    }

    // Step 1: Perform DFS and fill the stack
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            dfs(i);
        }
    }

    // Step 2: Process vertices in reverse order
    fill(visited.begin(), visited.end(), false);
    cout << "Strongly Connected Components:\n";
    while (!Stack.empty()) {
        int node = Stack.top();
        Stack.pop();
        if (!visited[node]) {
            reverseDFS(node);
            cout << endl;
        }
    }

    return 0;
}
