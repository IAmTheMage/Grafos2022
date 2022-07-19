#include "Graph.h"
#include "vector"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  Graph::Graph* graph = new Graph::Graph(argv);
  std::vector<int> subV;
  subV.push_back(0);
  subV.push_back(1);
  subV.push_back(3);
  subV.push_back(2);
  subV.push_back(4);
  subV.push_back(5);
  std::vector< std::vector<std::pair<int,int>>> sg = graph->vertexInducedSubgraph(subV);
  sg = graph->vertexInducedSubgraph(subV);
  graph->algorithmPrim(sg);
  return 0;
}