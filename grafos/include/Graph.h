#include "iostream"
#include "./Node.h"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"
#include "../include/Dot.h"

namespace Graph {
  #ifndef GRAPH
  #define GRAPH
  enum GraphType {
    DIRECTED, NONDIRECTED
  };

  enum EdgeType {
    PONDERED, NONPONDERED
  };

  class Graph {
    public:
      Graph(char** args);
      ~Graph() {};
      int countNumberOfNodes() {return count;};
      void instanceNewNode() {
        if(count == 0) {
          node = Node::make(0);
        }
        else {
          Node* p = node->getLastBeforeNull();
          p->instanceNew(count);
        }
        count++;
      };
      Node* searchById(int id);
      void directTransitiveClosure(int id);
      void indirectTransitiveClosure(int id);

      std::vector<int> getAllNodesConnected(int id);
      std::vector<Utils::DotType> generateDotTypeVector();

      void setAllNodesVisitedFalse();
    private:
      Node* node;
      GraphType graphType;
      EdgeType edgeType;
      int count = 0;
      void setFiles(std::string input, std::string output);
      std::ifstream fs;
  };
  #endif 
}