#include "iostream"
#include "./Node.h"

namespace Graph {
  #ifndef GRAPH
  #define GRAPH
  class Graph {
    public:
      Graph() {};
      ~Graph() {};
      int countNumberOfNodes() {return this->node->getNumberOfNodes();};
      void instanceNewNode() {
        if(countNumberOfNodes() == 0) {
          node = Node::make();
        }
        else {
          Node* p = node->getLastBeforeNull();
          p->instanceNew();
        }
      };
    private:
      Node* node;
  };
  #endif 
}