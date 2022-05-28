#include "iostream"
#include "./Node.h"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"

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
      int countNumberOfNodes() {return this->node->getNumberOfNodes();};
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
      Node* searchById(unsigned int id);
    private:
      Node* node;
      GraphType graphType;
      EdgeType edgeType;
      int count = 0;
      void setFiles(std::string input, std::string output);
      std::ifstream fs;
      std::ofstream of;
  };
  #endif 
}