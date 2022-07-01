#include "./Edge.h"
#include "iostream"

namespace Graph {
  #ifndef NODE
  #define NODE
  class Node {
    public:
      Node();
      ~Node();
      int id;
      static Node* make(int id) {
        Node* n = new Node();
        n->id = id;
        return n;
      }
      int getNumberOfNodes();
      Node* getLastBeforeNull();
      Node* getNext();
      void instanceNew(int id) {
        this->next = make(id);
      }
      Edge* getEdge() {
        return this->edge;
      }
      void makeRelationship(int id, int weight);
      int getEdgeCount() {
        return this->edgeCount;
      }

      bool beenVisited() {
        return this->wasVisited;
      }
      void setVisitFalse() {
        this->wasVisited = false;
      }
      void visited() {
        this->wasVisited = true;
      }
    private:
      Node* next;
      Edge* edge;
      int edgeCount = 0;
      bool wasVisited = false;
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