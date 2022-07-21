#include "Graph.h"
#include "Edge.h"
#include "vector"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  std::vector<Graph::Edge*> returnEdges;
  
  returnEdges = graph->deepPathTree(0);
  
  std::cout << "Arestas de retorno: ";
  for(auto i : returnEdges) {
    std::cout << i->getFrom() << " -> " << i->getTo() << ", ";
  }
  std::cout << "\n";
  return -1;
}