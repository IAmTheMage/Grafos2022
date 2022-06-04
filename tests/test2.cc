#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  for(int i = 0; i < 100; i++) {
    graph->instanceNewNode();
  }
  if(graph->searchById(55)->id == 55) return 0;
  return -1;
}