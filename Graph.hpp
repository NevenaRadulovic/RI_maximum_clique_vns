#ifndef __GRAPH__
#define __GRAPH__

#include <algorithm>
#include <filesystem>
#include <set>
#include <vector>

class Graph
{

public:
  Graph(std::filesystem::path filePath);

  int getNodeCount();
  int getEdgeCount();
  std::set<int> getNodeSet();
  std::vector<std::vector<int>> getAdjacencyMatrix();
  std::vector<int> getSortedNodes();
  void addEdge(int node, int neighbor);

  friend std::ostream& operator<<(std::ostream& out, const Graph& g);

  std::set<int> nodeSet;

private:
  int nodeCount;
  int edgeCount;
  std::vector<std::vector<int>> adjacencyMatrix;
};

#endif