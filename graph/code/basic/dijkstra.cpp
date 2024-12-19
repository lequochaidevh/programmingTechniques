#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

// Define a pair to store (distance, node)
typedef pair<int, int> Pair;

// Global variables
const vector<vector<Pair>>* graph; // Pointer to graph
vector<int> dist;                  // Distance array
vector<int> parent;                // Parent array to reconstruct the path

void dijkstra(int source, int destination) {
    priority_queue<Pair, vector<Pair>, greater<Pair>> pq; // Min-heap
    dist[source] = 0;
    pq.push({0, source});
    parent[source] = -1; // Source node has no parent

    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();

        if (current_dist > dist[current_node]) {
            continue; // Skip outdated entries
        }

        // Explore neighbors
        for (const Pair& neighbor : (*graph)[current_node]) {
            int next_node = neighbor.first;
            int edge_weight = neighbor.second;

            if (dist[current_node] + edge_weight < dist[next_node]) {
                dist[next_node] = dist[current_node] + edge_weight;
                parent[next_node] = current_node; // Update parent
                pq.push({dist[next_node], next_node});
            }
        }
    }

    // Output shortest path if it exists
    if (dist[destination] == INT_MAX) {
        cout << "No path from " << source << " to " << destination << ".\n";
        return;
    }

    // Reconstruct path
    vector<int> path;
    for (int node = destination; node != -1; node = parent[node]) {
        path.push_back(node);
    }
    reverse(path.begin(), path.end());

    // Print path and distance
    cout << "Shortest path from " << source << " to " << destination << ": ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << path[i];
    }
    cout << "\nDistance: " << dist[destination] << "\n";
}

int main() {
    int n, m; // Number of nodes and edges
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;

    vector<vector<Pair>> localGraph(n); // Local graph variable
    cout << "Enter edges (u v w) where u and v are nodes (0-based) and w is weight:\n";
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        localGraph[u].emplace_back(v, w);
        localGraph[v].emplace_back(u, w); // Uncomment for undirected graph
    }

    // Set global graph pointer
    graph = &localGraph;

    dist.assign(n, INT_MAX);  // Initialize global distance vector
    parent.assign(n, -1);     // Initialize parent vector for path reconstruction

    int source, destination;
    cout << "Enter source node: ";
    cin >> source;
    cout << "Enter destination node: ";
    cin >> destination;

    dijkstra(source, destination);

	
	/*
    cout << "Shortest distances from source node " << source << ":\n";
    for (int i = 0; i < n; ++i) {
        cout << "Node " << i << ": " << (dist[i] == INT_MAX ? -1 : dist[i]) << "\n";
    }	
	*/
    return 0;
}


/*
Enter number of nodes and edges: 5 6
Enter edges (u v w) where u and v are nodes (0-based) and w is weight:
0 1 2
0 2 4
1 2 1
1 3 7
2 4 3
3 4 1
Enter source node: 
1
Enter destination node: 3
Shortest path from 1 to 3: 1 -> 2 -> 4 -> 3
Distance: 5


*/



