#include "Graph.h"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  graph->print();
  std::cout << std::endl << std::endl << std::endl;
  int first, second;
  std::cout << "Digite os valores" << std::endl;
  std::cin >> first;
  std::cin >> second;
  std::cout << "Minimum path: " << std::endl;
  std::cout << graph->getNodeReferenceIndex(first) << "\n";
  for(auto i : graph->dijkstra(first, second)) {
    std::cout << i << " ";
  }
  /*std::cout << std::endl;
  graph->floyd(first, second);*/
  return -1;
}