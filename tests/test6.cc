#include "Graph.h"
#include "vector"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  std::vector<int> subV;
  subV.push_back(0);
  subV.push_back(1);
  graph->vertexInducedSubgraph(subV);
  return 0;
}