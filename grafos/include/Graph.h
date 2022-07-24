#include "iostream"
#include "./Node.h"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"
#include "../include/Dot.h"
#include "PerformanceMetrics.h"
#include "queue"
#include <climits>
#include "list"

namespace Graph {
  #ifndef GRAPH
  #define GRAPH
  enum GraphType {
    DIRECTED, NONDIRECTED
  };

  enum EdgeType {
    PONDERED, NONPONDERED
  };

  struct Relation {
    int from;
    int to = 0;
    int weight;
  };

  struct SubTree {
    int father;
    int order;
  };

  class Graph {
    public:
      Graph(char** argv);
      Graph(EdgeType edge, GraphType graph);
      ~Graph() {};
      int countNumberOfNodes() {return count;};
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

      Node* instanceNewNode(int id) {
        if(count == 0) {
          node = new Node();
          node->id = id;
          count++;
          return node;
        }
        else {
          Node* p = node->getLastBeforeNull();
          p->instanceNew(id);
          count++;
          return node->getLastBeforeNull();
        }
      }
      Node* searchById(int id);
      std::list<int> dijkstra(int id, int destination);
      std::list<int> shortestPath(int origin ,int destination, int* predecessors);
      bool edgeExists(int from, int to);
      void directTransitiveClosure(int id);
      void indirectTransitiveClosure(int id);
      Utils::DotType getAllNodesConnected(int id);
      std::vector<Utils::DotType> generateDotTypeVector();
      float clusteringCoeficient(int id);
      float clusteringGlobalCoeficient();
      int neighborsConnected(int id, int* p, int size);
      Node* getNodeByPosition(int id);
      std::list<int> floyd(int origin, int destination);
      void floydOut(std::list<int>& p, int** pred, int origin, int destination);
      int** generateArrayRepresentation();
      int** initPred();

      void kruskal(Graph* subGraph);
      void cleanVisited() {
        Node* p = node;
        while(p != nullptr) {
          p->setVisitFalse();
          p = p->getNext();
        }
      }

      Node* getNode() {
        return this->node;
      }

      int getCount() {
        return this->count;
      }

      void algorithmPrim(Graph* subgraph);
      int auxPrim(std::vector<int> distance, std::vector<bool> visited);
      Graph* vertexInducedSubgraph(std::vector<int> nodes);
      void generateEdge(int from, int to, int weight);
      void vertexInducedSubgraphAux(Graph* sub, std::vector<int>& p);

      void setAllNodesVisitedFalse();
      void generateGraphVizRepresentation();
      int getNodeReferenceIndex(int id); 
      void deepPath(Node* node);
      void print() {
        std::cout << "Initialize Print directive: " << "\n";
        Node* p = node;
        while(p != nullptr) {
          std::cout << p->id << " ";
          p = p->getNext();
        }
        std::cout << std::endl;
      }
    private:
      Node* node;
      GraphType graphType;
      EdgeType edgeType;
      int count = 0;
      void setFiles(std::string input, std::string output);

      //std::vector<Edge> deepPathTree(Node* vertex);

      std::ifstream ip;
      std::vector<int> visited;
      Utils::Dot* dt;
  };

  #endif 
}