#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  Graph::Graph* sub = graph->vertexInducedSubgraph(graph->getGraphIds());
  graph->kruskal(sub);
  return -1;
}