#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  graph->indirectTransitiveClosure(50);
  return 0;
}