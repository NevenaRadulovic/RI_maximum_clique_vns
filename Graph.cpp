#include "Graph.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

Graph::Graph(std::filesystem::path filePath) {
  std::ifstream file(filePath);
  std::string line;
  getline(file, line);

  int numNodes;
  int lineNum;
  file >> numNodes >> numNodes >> lineNum;

  this->nodeCount = numNodes;
  this->edgeCount = 0;
  this->adjacencyMatrix = std::vector<std::vector<int>>(
      this->nodeCount + 1, std::vector<int>(this->nodeCount + 1, 0));

  for (int i = 1; i < this->nodeCount + 1; ++i) {
    this->nodeSet.emplace(i);
  }

  int node;
  int neighbor;

  while (lineNum) {
    file >> node >> neighbor;
    this->addEdge(node, neighbor);
    --lineNum;
  }
}

int Graph::getNodeCount() {
  return this->nodeCount;
}

int Graph::getEdgeCount() {
  return this->edgeCount;
}

std::set<int> Graph::getNodeSet() {
  return this->nodeSet;
}

std::vector<std::vector<int>> Graph::getAdjacencyMatrix() {
  return this->adjacencyMatrix;
}

// Graph is undirected
void Graph::addEdge(int node, int neighbor) {
  this->adjacencyMatrix[node][neighbor] = 1;
  this->adjacencyMatrix[neighbor][node] = 1;
  ++edgeCount;
}

std::ostream& operator<<(std::ostream& out, const Graph& g) {
  for (auto& row : g.adjacencyMatrix) {
    for (auto& val : row) {
      out << val << " ";
    }
    out << std::endl;
  }
  return out;
}
