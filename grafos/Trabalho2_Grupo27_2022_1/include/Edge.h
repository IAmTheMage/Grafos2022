#include "iostream"

namespace Graph {
  #ifndef EDGE
  #define EDGE
  class Edge {
    public:
      Edge(int from, int to, float weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
      };

      void makeFromChain(Edge* edge) {
        Edge* b = nullptr;
        while(edge != nullptr) {
          b = getLast();
          b->setNext(new Edge(edge->getFrom(), edge->getTo(), edge->getWeight()));
          b = b->getNext();
          edge = edge->getNext();
        }
      }


      ~Edge();
      Edge* getNext() {
        return this->next;
      }
      float getWeight() {
        return this->weight;
      }
      int getFrom() {
        return from;
      }
      int getTo() {
        return to;
      }
      void setNext(Edge* edge) {
        this->next = edge;
        count++;
      };

      void setNextBeforeNull(Edge* edge) {
        Edge* b = nullptr;
        Edge* s = this;
        while(s != nullptr) {
          b = s;
          s = s->getNext();
        }
        b->setNext(edge);
      }

      Edge* getLast() {
        Edge* edge = this;
        for(int i = 0; i < count; i++) {
          edge = edge->getNext();
        }
        return edge;
      }

      void setVisited(bool visi) {
        visited = visi;
      }
      
      bool getVisited() {
        return visited;
      }
    private:
      Edge* next;
      float weight;
      int from;
      int to;
      int count = 0;
      bool visited = false;
  };
  #endif
}