#include "Graph.h"
#include "vector"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
//  Graph::Graph *subGraph = new Graph::Graph();
  std::vector<int> subN;
  subN.push_back(0);
  subN.push_back(1);
  subN.push_back(3);
  subN.push_back(2);
  subN.push_back(4);
  subN.push_back(5);

  graph->vertexInducedSubgraph(subN);
}