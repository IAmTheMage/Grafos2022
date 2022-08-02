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
#define INF INT_MAX

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
      std::vector<Edge*> deepPathTree(int id);
      void deepPathTreeAssistant(Node* vertex, std::vector<int>& nodesInThePath, 
      std::vector<Utils::WeightedDot>& dots, std::vector<Edge*>& returnEdges);
      int witchSubTree(SubTree subTree[], int n);
      void joinSubTrees(SubTree subTree[], int u, int v);
      static bool searchDots(std::vector<Utils::WeightedDot>& p,int from, int to);
      void algorithmPrim(Graph* subgraph);
      void printPrim(Graph* subgraph, std::vector<int>& mgt);
      void readClusteringFile(std::ifstream& stream) {
      };

      int minPos(std::vector<int>& distances, std::vector<bool>& isVisited) {
        int min = INF;
        int pos;
        bool cond = false;
        for(int i = 0; i < distances.size(); i++) {
          if(distances[i] < min && isVisited[i] == false) {
            min = distances[i];
            pos = i;
            cond = true;
          }
        }
        if(cond) {
          return pos;
        }
        else {
          for(int i = 0; i < distances.size(); i++) {
            if(distances[i] == min && isVisited[i] == false) {
              min = distances[i];
              pos = i;
              cond = true;
              return pos;
            }
          }
        }
        return pos;
      }

      std::vector<int> getGraphIds() {
        Node* p = node;
        std::vector<int> s;
        while(p != nullptr) {
          s.push_back(p->id);
          p = p->getNext();
        }
        return s;
      }

      static void printList(std::list<int> path) {
        if(path.empty()) std::cout << "Caminho infinito" << std::endl;
        int index = 0;
        for(int b : path) {
          if(index == path.size() - 1) {
            std::cout << b << std::endl;
          }
          else {
            std::cout << b << "->";
          }
          index++;
        }
      }

      int findSet(int* parent, int i) {
        if (i == parent[i])
          return i;
        else
          // Else if i is not the parent of itself
          // Then i is not the representative of his set,
          // so we recursively call Find on its parent
          return findSet(parent ,parent[i]);
      }

      void unionSet(int*parent, int u, int v) {
        parent[u] = parent[v];
      }

      std::vector<std::pair<int, std::pair<int, int>>> getGraphInPairFormat() {
        std::vector<std::pair<int, std::pair<int, int>>> pairs;
        for(int i = 0; i < count; i++) {
          Node* p = getNodeByPosition(i);
          Edge* edge = p->getEdge();
          /*if(p->getEdgeCount() == 0) {
            pairs.push_back({0, {p->id, p->id}});
          }*/
          for(int b = 0; b < p->getEdgeCount(); b++) {
            bool cond = true;
            for(auto l : pairs) {
              if((l.second.first == p->id && l.second.second == edge->getTo()) || (l.second.first == edge->getTo() && l.second.second == p->id)) {
                cond = false;
              }
            }
            if(cond) {
              pairs.push_back({edge->getWeight(), {p->id, edge->getTo()}});
            }
            edge = edge->getNext();
          }
        }
        return pairs;
      }

      void printKruskal(
        std::vector<std::pair<int, std::pair<int, int>>>& edges, Graph* subGraph );

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

      Graph* vertexInducedSubgraph(std::vector<int> nodes);
      void generateEdge(int from, int to, int weight);
      void vertexInducedSubgraphAux(Graph* sub, std::vector<int>& p);

      void setAllNodesVisitedFalse();
      void generateGraphVizRepresentation();
      int getNodeReferenceIndex(int id);
      int getNodeReferenceIndex(int id, bool t);  
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
      void setFiles(std::string input);
      std::string outputFilePath;
      //std::vector<Edge> deepPathTree(Node* vertex);
      char* outFile;
      std::ifstream ip;
      std::vector<int> visited;
      Utils::Dot* dt;
  };

  #endif 
}