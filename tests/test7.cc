#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  for(int i = 0; i < 6; i++) {
    std::cout << "Coeficient for " << i << " is: " << graph->clusteringCoeficient(i) << std::endl;
  }
  return -1;
}