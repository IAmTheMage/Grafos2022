namespace Graph {
  #ifndef EDGE
  #define EDGE
  class Edge {
    public:
      Edge(int from, int to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
      };
      ~Edge();
      Edge* getNext() {
        return this->next;
      }
      int getWeight() {
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
      };
    private:
      Edge* next;
      int weight;
      int from;
      int to;
  };
  #endif
}