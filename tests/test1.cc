#include <string>
#include <iostream>
using namespace std;

#include "Graph.h"
int main(int argc, char *argv[])
{
  Graph::Graph* graph = new Graph::Graph(argv);
  if(graph->countNumberOfNodes() == 6) return 0;
  return -1;
}
