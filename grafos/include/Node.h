#include "./Edge.h"
#include "iostream"

namespace Graph {
  #ifndef NODE
  #define NODE
  class Node {
    public:
      Node();
      ~Node();
      unsigned int id;
      static Node* make(unsigned int id) {
        Node* n = new Node();
        n->id = id;
        return n;
      }
      int getNumberOfNodes();
      Node* getLastBeforeNull();
      Node* getNext();
      void instanceNew(unsigned int id) {
        this->next = make(id);
      }
      void makeRelationship(unsigned int id, int weight);
      int getEdgeCount() {
        return this->edgeCount;
      }
    private:
      Node* next;
      Edge* edge;
      int edgeCount = 0;
      Edge* getLastEdgeBeforeNull();
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