#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  graph->indirectTransitiveClosure(4);
  return 0;
}