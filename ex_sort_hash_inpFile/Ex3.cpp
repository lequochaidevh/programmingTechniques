#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>

// Class to represent a directed edge
class DirectedEdge {
public:
    int from, to;
    double weight;

    DirectedEdge(int from, int to, double weight) 
        : from(from), to(to), weight(weight) {}
    
    void printEdge() const {
        std::cout << from << " -> " << to << " (" << weight << ")" << std::endl;
    }
};

// Class to represent the edge-weighted digraph
class EdgeWeightedDigraph {
private:
    int V; // Number of vertices
    int E; // Number of edges
    std::unordered_map<int, std::vector<DirectedEdge>> adj; // Adjacency list

public:
    EdgeWeightedDigraph(int V) : V(V), E(0) {}

    void addEdge(int from, int to, double weight) {
        DirectedEdge edge(from, to, weight);
        adj[from].push_back(edge);
        E++;
    }

    void printGraph() const {
        for (const auto& pair : adj) {
            std::cout << pair.first << ": ";
            for (const auto& edge : pair.second) {
                std::cout << "[" << edge.to << ", " << edge.weight << "] ";
            }
            std::cout << std::endl;
        }
    }

    // Getter for number of vertices
    int getVertices() const { return V; }

    // Getter for number of edges
    int getEdges() const { return E; }
};

int main() {
    std::ifstream inputFile("tinyEWD.txt");
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int V, E;
    inputFile >> V >> E;

    EdgeWeightedDigraph graph(V);

    int from, to;
    double weight;
    while (inputFile >> from >> to >> weight) {
        graph.addEdge(from, to, weight);
    }

    inputFile.close();

    std::cout << "Edge-weighted digraph representation:\n";
    graph.printGraph();

    return 0;
}