#include "Graph.h"
#include "Edge.h"
#include "vector"

int main(int argc, char** argv) {
  Graph::Graph *graph = new Graph::Graph(argv);
  std::vector<Graph::Edge*> returnEdges;
  
  returnEdges = graph->deepPathTree(0);
  
  return -1;
}