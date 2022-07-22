#include "Graph.h"
#include "vector"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  std::vector<int> returnFunc;
  
  returnFunc = graph->indirectTransitiveClosure(5);

  std::cout << "Fecho transitivo indireto: ";
  for(auto i : returnFunc) {
    std::cout << i << " ";
  }
  std::cout << "\n";

  return -1;
}