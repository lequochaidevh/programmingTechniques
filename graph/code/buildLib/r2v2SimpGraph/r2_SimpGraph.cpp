#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip> // For formatted printing


class SimpGraph {
private:
    int vertices;                            // Number of vertices
    bool isDirected;                        // Whether the graph is directed

    std::vector<std::vector<int>> adjList;      // Adjacency list representation
    std::vector<std::vector<int>> adjMatrix;    // Adjacency matrix representation
    std::vector<std::pair<int, int>> edgeList;  // Edge list representation

    std::vector<std::pair<int, bool>> visited;
    std::vector<std::pair<int, int>> parentList;  // To store parent of each vertex
public:
    SimpGraph(int v, bool directed = false) : vertices(v), isDirected(directed) \
                                            , adjList(v), adjMatrix(v, std::vector<int>(v, 0)) {}
    // Clear all representations
    void clear() {
        adjList.clear();
        adjMatrix.clear();
        edgeList.clear();
    }
    /*pagram:
     * u,v is a vertice,
     * directed = true => create path directed from u to v
     * directed = false => create path undirected
    */
    void addSingleNodeList(int u, int v, bool directed = false)
    {
        edgeList.emplace_back(u, v);
        if(!directed) {
            edgeList.emplace_back(v, u);
        }
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
    // Input methods
    // input directed Adjacency List
    void inputAdjacencyList(int n)
    {
        clear();
        adjList.resize(n);
        for (int i = 0; i < n; ++i) {
            int numEdges;
            std::cin >> numEdges;
            for (int j = 0; j < numEdges; ++j) {
                int neighbor;
                std::cin >> neighbor;
                adjList[i].push_back(neighbor);
            }
        }
    }
    // input directed Adjacency Matrix
    void inputAdjacencyMatrix(int n)
    {
        clear();
        adjMatrix.resize(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cin >> adjMatrix[i][j];
            }
        }
    }

    void inputEdgeList(int m) {
        clear();
        edgeList.reserve(m);    // edgeList.resize(m); // Sets size to m
        for (int i = 0; i < m; ++i) {
            int u, v;
            std::cin >> u >> v;
            edgeList.emplace_back(u, v);// edgeList[i] = {u, v}; // Assign directly
        }
    }

    // Conversion functions
    void adjacencyListToAdjacencyMatrix() {
        int n = adjList.size();
        adjMatrix.clear();
        adjMatrix.assign(n, std::vector<int>(n, 0)); // specify adjMatrix[n][n] have n*n element
        for (int u = 0; u < n; ++u) {
            for (int v : adjList[u]) {
                adjMatrix[u][v] = 1;
            }
        }
    }

    void adjacencyListToEdgeList() {
        edgeList.clear();

        /* Reserve edgeList */
        int totalEdges = 0;
        for (const auto& neighbors : adjList) {
            totalEdges += neighbors.size(); // Count all edges
        }        
        edgeList.reserve(totalEdges); // Reserve exact number of edges

        /* Push data */
        for (int u = 0; u < adjList.size(); ++u) {
            for (int v : adjList[u]) {
                edgeList.emplace_back(u, v);
            }
        }
    }


    void adjacencyMatrixToAdjacencyList() {
        int n = adjMatrix.size();
        adjList.clear(); // -> fixed size is 0
        // adjList.size()= 0 after clear, althought use push_back after that, but it can NOT update size.
        adjList.resize(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] == 1) {
                    adjList[i].push_back(j);
                }
            }
        }
    }

    void adjacencyMatrixToEdgeList() {
        int n = adjMatrix.size();
        edgeList.clear();

        /* Reserve edgeList */
        int totalEdges = 0;
        for (const auto& neighbors : adjMatrix) {
            totalEdges += neighbors.size();
        }
        edgeList.reserve(totalEdges);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjMatrix[i][j] == 1) {
                    edgeList.emplace_back(i, j);
                }
            }
        }
    }

    void edgeListToAdjacencyList() {
        adjList.clear();
        for (const auto& edge : edgeList) {
            int u = edge.first;
            int v = edge.second;
            if (u >= adjList.size()) {
                adjList.resize(u + 1);
            }
            adjList[u].push_back(v);
        }
    }

    void edgeListToAdjacencyMatrix() {
        adjMatrix.clear();
        int n = 0;
        for (const auto& edge : edgeList) {
            n = std::max({n, edge.first, edge.second});
        }
        n += 1;
        adjMatrix.assign(n, std::vector<int>(n, 0));
        for (const auto& edge : edgeList) {
            int u = edge.first;
            int v = edge.second;
            adjMatrix[u][v] = 1;
        }
    }


    /*TODO: optimize duplicate code
     * DFSUtil(int v) -> private
     * DFSUtil(int v, int destination) -> private
     * DFS(int start)
     * DFS(int start, int destination) -> private
     * BFS(int start)
     * BFS(int start, int destination) -> private
    */
    // Perform DFS
    void DFSUtil(int v) {
        visited[v].second = true;
        std::cout << v << " ";

        // Recur for all the adjacent adjList.size()
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor].second) {
                DFSUtil(neighbor);
            }
        }
    }

    void DFSUtil(int v, int destination) {
        visited[v].second = true;

        if (v == destination) {
            return;  // If we've reached the destination, stop recursion
        }
        std::cout << "\n parent: ";
        // Recur for all the adjacent vertices
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor].second) {
                std::cout << v << " ";
                parentList[neighbor] = {v, 1};  // Store the parent of the vertex
                DFSUtil(neighbor, destination);
                if (visited[destination].second) {
                    return;  // If destination is found, stop further recursion
                }
            }
        }
    }

    void DFS(int start) {
        visited.clear();
        for (int i = 0; i < adjList.size(); ++i) { // memset
            visited.emplace_back(i, false);
        }

        std::cout << "DFS starting from vertex " << start << ": ";
        DFSUtil(start);
        std::cout << std::endl;
    }

    void DFS(int start, int destination) {
        visited.clear();
        parentList.clear();
        for (int i = 0; i < adjList.size(); ++i) {
            visited.emplace_back(i, false);
            parentList.emplace_back(-1, -1);  // Initialize parentList with invalid pairs
        }

        visited[start].second = true;  // Mark the source as visited
        parentList[start] = {-1, -1}; // The source has no parent

        DFSUtil(start, destination);

        // Reconstruct the path from source to destination using parentList
        if (visited[destination].second) {
            std::cout << "DFS path from " << start << " to " << destination << ": ";
            int v = destination;
            std::vector<int> path;
            while (v != -1) {
                path.push_back(v);
                v = parentList[v].first;  // Get the parent of the current vertex
            }
            std::reverse(path.begin(), path.end());

            for (int vertex : path) {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No path found from " << start << " to " << destination << std::endl;
        }
    }

    // Perform BFS
    void BFS(int start) {
        visited.clear();
        for (int i = 0; i < adjList.size(); ++i) {
            visited.emplace_back(i, false);
        }

        std::queue<int> q; // Queue for BFS
        visited[start].second = true;
        q.push(start);

        std::cout << "BFS starting from vertex " << start << ": ";

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            std::cout << v << " ";

            // Enqueue all adjacent, unvisited vertices
            for (int neighbor : adjList[v]) {
                if (!visited[neighbor].second) {
                    visited[neighbor].second = true;
                    q.push(neighbor);
                }
            }
        }
        std::cout << std::endl;
    }

    void BFS(int start, int destination) {
        visited.clear();
        parentList.clear();
        for (int i = 0; i < adjList.size(); ++i) {
            visited.emplace_back(i, false);
            parentList.emplace_back(-1, -1);  // Initialize parentList with invalid pairs
        }

        std::queue<int> q;
        visited[start].second = true;
        parentList[start] = {-1, -1};  // The source has no parent
        q.push(start);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (v == destination) {
                // Reconstruct the path from destination to source
                std::cout << "BFS path from " << start << " to " << destination << ": ";
                std::vector<int> path;
                int current = destination;
                while (current != -1) {
                    path.push_back(current);
                    current = parentList[current].first;  // Get the parent of the current vertex
                }
                std::reverse(path.begin(), path.end());

                for (int vertex : path) {
                    std::cout << vertex << " ";
                }
                std::cout << std::endl;
                return;
            }

            // Enqueue all adjacent, unvisited vertices
            for (int neighbor : adjList[v]) {
                if (!visited[neighbor].second) {
                    visited[neighbor].second = true;
                    parentList[neighbor] = {v, 1};  // Mark the parent
                    q.push(neighbor);
                }
            }
        }

        std::cout << "No path found from " << start << " to " << destination << std::endl;
    }

    void findPath(int source, int destination, void(SimpGraph::*algorithm)(int, int)) {
        (this->*algorithm)(source, destination);  // Call either DFS or BFS based on the passed algorithm
    }

    // Utility functions
    void printAdjacencyList() const {
        std::cout << "Adjacency List:\n";
        for (int i = 0; i < adjList.size(); ++i) {    // for (int i = 0; i < vertices; ++i)
            std::cout << i << ": ";
            for (int neighbor : adjList[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    void printAdjacencyMatrix() const {
        std::cout << "Adjacency Matrix:\n";
        std::cout << "       ";
        for (int i = 0; i < adjMatrix.size(); ++i) { // adjMatrix.size() == vertices
            std::cout << std::setw(3) << "[" << i << "]";
        }
        std::cout << std::endl << std::endl;
        int col = 0;
        for (const auto& v : adjMatrix) {
            std::cout << std::setw(4) << "[" << col << "]";
            for (int val : v) {
                std::cout << std::setw(5) << val;
            }
            std::cout << "\n";
            std::cout << "\n";
            col++;
        }
    }

    void printEdgeList() const {
        std::cout << "Edge List:\n";
        for (const auto& edge : edgeList) {
            //std::cout << edge.first << " " << edge.second << "\n";
            if (!isDirected || edge.first < edge.second) { // To avoid duplicates in undirected graphs
                std::cout << "(" << edge.first << ", " << edge.second << ")\n";
            }
        }
    }
};
#define _PHASE_ED
SimpGraph *test1(){
    int n, m;
    int vertices = 6;
    bool isDirected = false; // Change to true for directed graph
    SimpGraph *g = new SimpGraph(vertices, isDirected);
#if defined (_PHASE_AD)
    g->addSingleNodeList(0, 1);
    g->addSingleNodeList(0, 2, true);
    g->addSingleNodeList(1, 3);
    g->addSingleNodeList(1, 4);
    g->addSingleNodeList(2, 5);
    g->printAdjacencyList();
    g->printEdgeList();
    g->printAdjacencyMatrix();

#elif defined (_PHASE_AL)
    std::cin >> n;
    g->inputAdjacencyList(n);               g->printAdjacencyList();
    g->adjacencyListToEdgeList();           g->printEdgeList();
    g->adjacencyListToAdjacencyMatrix();    g->printAdjacencyMatrix();
#elif defined (_PHASE_AM)
    std::cin >> n;
    g->inputAdjacencyMatrix(n);             g->printAdjacencyMatrix();
    g->adjacencyMatrixToAdjacencyList();    g->printAdjacencyList();
    g->adjacencyMatrixToEdgeList();         g->printEdgeList();
#elif defined (_PHASE_ED)
    std::cin >> m;
    g->inputEdgeList(m);                    g->printEdgeList();
    g->edgeListToAdjacencyList();           g->printAdjacencyList();
    g->edgeListToAdjacencyMatrix();         g->printAdjacencyMatrix();
#endif

    int source = 0;   // Example source
    int destination = 5;  // Example destination

    g->DFS(source);
    g->BFS(source);

    std::cout << "Finding path using DFS:\n";
    g->findPath(source, destination, &SimpGraph::DFS);  // Using DFS to find the path

    std::cout << "\nFinding path using BFS:\n";
    g->findPath(source, destination, &SimpGraph::BFS);  // Using BFS to find the path


    return g;
}
int main() {
    SimpGraph *hacker = test1();
    hacker->printAdjacencyList();
    delete hacker;
    return 0;
}
//g++ -o r2_GraphTheory r2_GraphTheory.cpp
/*
*/
