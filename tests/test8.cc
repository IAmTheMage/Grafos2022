#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  graph->print();
  std::cout << "Minimum path: " << std::endl;
  for(auto i : graph->dijkstra(4, 3)) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
  return -1;
}