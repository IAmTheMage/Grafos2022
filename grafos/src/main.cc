#include "iostream"
#include "../include/Node.h"
#include "../include/Graph.h"

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph();
  graph->instanceNewNode();
  graph->instanceNewNode();
  std::cout << graph->countNumberOfNodes() << std::endl;
  return 0;
}