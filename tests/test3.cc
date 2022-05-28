#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  for(int i = 0; i < 100; i++) {
    graph->instanceNewNode();
  }
  std::cout << "Number of edges: " << graph->searchById(1)->getEdgeCount() << std::endl;
  return 0;
}