#include <string>
#include <iostream>
using namespace std;

#include "Graph.h"
int main(int argc, char *argv[])
{
  Graph::Graph* graph = new Graph::Graph(argv);
  for(int i = 0; i < 100; i++) {
    graph->instanceNewNode();
  }
  if(graph->countNumberOfNodes() == 100) return 0;
  return -1;
}
