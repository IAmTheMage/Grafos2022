#include "./Edge.h"
#include "iostream"
#include "vector"

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
        this->next = new Node();
        this->next->id = id;
      }
      Edge* getEdge() {
        return this->edge;
      }

      void setEdge(Edge* p) {
        this->edge = p;
      }

      Edge* searchEdge(int to) {
        Edge* ed = edge;
        while(ed != nullptr) {
          if(ed->getTo() == to) return ed;
          ed = ed->getNext();
        }
        return nullptr;
      }
 
      void setNext(Node* s) {
        this->next = s;
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

      void setPosition(int position) {
        this->position = position;
      }

      int getPosition() {
        return this->position;
      }

      std::vector<int> visitedBy;
    private:
      Node* next;
      Edge* edge;
      int edgeCount = 0;
      bool wasVisited = false;
      int position = 0;
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