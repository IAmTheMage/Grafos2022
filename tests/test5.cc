#include "Graph.h"

int TEST_SUIT = 25;

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  int i[4] = {4088, 1412, 1866,2234 };
  for(int b = 0; b < 4; b++) {
    graph->directTransitiveClosure(i[b]);
  }
  return 0;
}