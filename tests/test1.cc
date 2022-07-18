#include <string>
#include <iostream>
using namespace std;

#include "Graph.h"
int main(int argc, char *argv[])
{
  Graph::Graph* graph = new Graph::Graph(argv);
  graph->print();
  return -1;
}
