#include "iostream"
#include "./Node.h"
#include "fstream"
#include "sstream"
#include "string"
#include "vector"
#include "../include/Dot.h"
#include "config.h"
#include "../include/PerformanceMetrics.h"
#include "stack"

namespace Graph {
  #ifndef GRAPH
  #define GRAPH
  enum GraphType {
    DIRECTED, NONDIRECTED
  };

  enum EdgeType {
    PONDERED, NONPONDERED
  };

  class Graph {
    public:
      Graph(char** args);
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
      Node* searchById(int id);
      void directTransitiveClosure(int id);
      void indirectTransitiveClosure(int id);

      Utils::DotType evaluateDirectTransitiveClosure(int id);
      Utils::DotType getAllNodes(int id);
      std::vector<Utils::DotType> generateDotTypeVector();

      void setAllNodesVisitedFalse();
      //void deepPath(Node* node);
      Utils::DotType deepPath(Node* node);
    private:
      Node* node;
      GraphType graphType;
      EdgeType edgeType;
      int count = 0;
      Utils::PerformanceMetrics* metrics;
      void setFiles(std::string input, std::string output);
      std::ifstream fs;
      Utils::Dot* dt;
      std::vector<int> visited;
      std::string output;
      std::stack<int> stackz;
  };
  #endif 
}