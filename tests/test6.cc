#include "Graph.h"
#include "vector"

int main(int argc, char** argv) {
  std::cout << "test\n";
  Graph::Graph *graph = new Graph::Graph(argv);
  std::vector<int> returnFunc;
  
  std::cout << "Hello\n";
  returnFunc = graph->indirectTransitiveClosure(0);

  std::cout << "Fecho transitivo indireto: ";
  for(auto i : returnFunc) {
    std::cout << i << " ";
  }
  std::cout << "\n";

  return -1;
}