#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  Graph::Graph *graph2 = new Graph::Graph(argv);
  std::vector<int> subN;
  subN.push_back(861);
  subN.push_back(16);
  Graph::Graph* sub = graph->vertexInducedSubgraph(subN);
  graph->kruskal(sub);
  return -1;
}