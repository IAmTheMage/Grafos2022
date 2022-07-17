#include "Graph.h"


int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  graph->directTransitiveClosure(0);
}