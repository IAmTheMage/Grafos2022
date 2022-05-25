#include "./Edge.h"
#include "iostream"

namespace Graph {
  #ifndef NODE
  #define NODE
  class Node {
    public:
      Node();
      ~Node();
      static Node* make() {
        std::cout << std::endl;
        return new Node();
      }
      int getNumberOfNodes();
      Node* getLastBeforeNull();
      Node* getNext();
      void instanceNew() {
        this->next = make();
      }
    private:
      Node* next;
      Edge* edge;
  };

  class WeightedNode {
    public:
      WeightedNode();
      ~WeightedNode();
    private:
      int weight;
  };
  #endif
}