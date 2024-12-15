#include <iostream>
#include <vector>
using namespace std;

// Function to add an edge between two vertices
void addEdge(vector<vector<int>>& adj, int i, int j) {
    adj[i].push_back(j);
    adj[j].push_back(i); // Undirected
}
/*
	TODO:
	build directed adjacency list by commending line:    adj[j].push_back(i); // Undirected

*/
// Function to display the adjacency list
void displayAdjList(const vector<vector<int>>& adj) {
    for (int i = 0; i < adj.size(); i++) {
        cout << i << ": "; // Print the vertex
        for (int j : adj[i]) {
            cout << j << " "; // Print its adjacent
        }
        cout << endl; 
    }
}

// Main function
int main() {
    // Create a graph with 4 vertices and no edges
    int V = 4;
    vector<vector<int>> adj(V); 

    // Now add edges one by one
    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 1, 2);
    addEdge(adj, 2, 3);

    cout << "Adjacency List Representation:" << endl;
    displayAdjList(adj);

    return 0;
}


/*
	g++ -o undirectedAdjacencyMatrix undirectedAdjacencyMatrix.cpp
	
	output:
	Adjacency List Representation:
	0: 1 2 
	1: 0 2 
	2: 0 1 3 
	3: 2 


*/

