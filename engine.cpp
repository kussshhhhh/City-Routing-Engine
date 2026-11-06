#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <string>
#include <stdexcept>

struct Edge {
    int targetNode;
    double weight;
};

class RoutingMicroservice {
private:
    std::unordered_map<int, std::vector<Edge>> adjacencyList;

public:
    void addRoute(int source, int destination, double weight) {
        // C++11 explicit struct initialization
        adjacencyList[source].push_back(Edge{destination, weight}); 
    }

    void calculateOptimalRoute(int startNode, int endNode) {
        using pq_element = std::pair<double, int>;
        std::priority_queue<pq_element, std::vector<pq_element>, std::greater<pq_element>> pq;
        std::unordered_map<int, double> distances;
        std::unordered_map<int, int> previousNodes;

        for (const auto& pair : adjacencyList) {
            distances[pair.first] = std::numeric_limits<double>::infinity();
        }
        
        distances[startNode] = 0.0;
        // C++11 std::make_pair
        pq.push(std::make_pair(0.0, startNode)); 

        while (!pq.empty()) {
            // C++11 standard pair access (replaces structured bindings)
            double currentDist = pq.top().first; 
            int currentNode = pq.top().second;
            pq.pop();

            if (currentNode == endNode) break;
            if (currentDist > distances[currentNode]) continue;

            for (const auto& edge : adjacencyList[currentNode]) {
                double newDist = currentDist + edge.weight;
                if (distances.find(edge.targetNode) == distances.end() || newDist < distances[edge.targetNode]) {
                    distances[edge.targetNode] = newDist;
                    previousNodes[edge.targetNode] = currentNode;
                    // C++11 std::make_pair
                    pq.push(std::make_pair(newDist, edge.targetNode)); 
                }
            }
        }

        if (distances.find(endNode) == distances.end() || distances[endNode] == std::numeric_limits<double>::infinity()) {
            throw std::runtime_error("No route found");
        }

        std::vector<int> path;
        int curr = endNode;
        while (curr != startNode) {
            path.push_back(curr);
            curr = previousNodes[curr];
        }
        path.push_back(startNode);
        std::reverse(path.begin(), path.end());
        
        std::cout << distances[endNode] << " ";
        for (int node : path) std::cout << node << " ";
        std::cout << "\n";
    }
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) throw std::invalid_argument("Requires exactly 2 arguments: startNode endNode");
        
        int startNode = std::stoi(argv[1]);
        int endNode = std::stoi(argv[2]);

        RoutingMicroservice engine;
        engine.addRoute(1, 2, 5.2);
        engine.addRoute(1, 3, 2.1);
        engine.addRoute(3, 2, 1.5);
        engine.addRoute(2, 4, 3.0);
        engine.addRoute(3, 4, 7.8);
        engine.addRoute(4, 5, 2.0);

        engine.calculateOptimalRoute(startNode, endNode);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}