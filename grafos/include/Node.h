#include "./Edge.h"
#include "iostream"
#include "vector"


#define NODE_TOTAL_WEIGHT_RELEVANCE 0.007
#define NODE_DEGREE_RELEVANCE 0.2
#define NODE_WEIGHT_RELEVANCE 0.95

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


      void instanceNew(int id, int weight) {
        this->next = new Node();
        this->next->id = id;
        this->next->weight = weight;
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

      void makeRelationship(int id, float weight);
      int getEdgeCount() {
        return this->edgeCount;
      }

      float getTotalWeight() {
        return this->totalWeight;
      }

      float getClusteringRelativeValue() {
        return (float)(this->edgeCount * NODE_DEGREE_RELEVANCE)
        + (float)(this->weight * NODE_WEIGHT_RELEVANCE)
        + (float)(this->totalWeight * NODE_TOTAL_WEIGHT_RELEVANCE);
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

      void setWeight(int weight) {
        this->weight = weight;
      }

      int getWeight() {
        return this->weight;
      }

      void deleteLast() {
        Node* p = getLastBeforeNull();
        p = nullptr;
      }

      void instanceMultiplesNodes(int size, std::vector<int>& weights) {
        int index = 1;
        while(index < size) {
          Node* p = getLastBeforeNull();
          p->instanceNew(index, weights[index]);
          index++;
        }
      }

      std::vector<int> visitedBy;
    private:
      Node* next;
      Edge* edge;
      int edgeCount = 0;
      bool wasVisited = false;
      float totalWeight = 0.0f;
      int weight = 0;
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