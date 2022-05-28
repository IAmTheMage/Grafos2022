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
    private:
      Edge* next;
      int weight;
      int from;
      int to;
  };
  #endif
}