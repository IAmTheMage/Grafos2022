#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"
#include "config.h"

#ifdef LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif



namespace Graph {
  Graph::Graph(char** args) {
    dt = new Utils::Dot();
    std::cout << "Instance new graph" << std::endl;
    if(strcmp(args[4], "1") == 0) {
      edgeType = EdgeType::PONDERED;
    }
    else {
      edgeType = EdgeType::NONPONDERED;
    }
    if(strcmp(args[3], "1") == 0) {
      graphType = GraphType::DIRECTED;
    }
    else {
      graphType = GraphType::NONDIRECTED;
    }
    std::string in(args[1]);
    std::string out(args[2]);
    setFiles(in, out);
  }

  Node* Graph::searchById(int id) {
    Node* t = this->node;
    Node* p = nullptr;
    while(t != NULL) {
      if(t->id == id) {
        p = t;
        break;
      }
      t = t->getNext();
    }
    return p;
  }

  void Graph::setFiles(std::string in, std::string out) {
    std::string path(ROOT_DIR);
    path.append(in);
    std::fstream ip(path, std::ios::in);
    if(ip.is_open()) {
      std::string b;
      std::getline(ip, b);
      int numberOfNodes = std::atoi(b.c_str());
      for(int j = 0; j < numberOfNodes; j++) {
        this->instanceNewNode();
      }
      std::vector<int> values;
      while(getline(ip, b)) {
        std::string s;
        std::stringstream temp(b);
        if(edgeType == PONDERED) {
          while(getline(temp, s, ' ')) {
            values.push_back(std::atoi(s.c_str()));
          }
          this->searchById(values[0])->makeRelationship(values[1], values[2]);
          if(graphType == GraphType::NONDIRECTED) {
            this->searchById(values[1])->makeRelationship(values[0], values[2]);
          }
          values.clear();
        }
        else {
          while(getline(temp, s, ' ')) {
            values.push_back(std::atoi(s.c_str()));
          }
          this->searchById(values[0])->makeRelationship(values[1], 0);
          if(graphType == GraphType::NONDIRECTED) {
            this->searchById(values[1])->makeRelationship(values[0], 0);
          }
          values.clear();
        }
      }
      std::fstream of;
      std::string path(ROOT_DIR);
      path.append("graph.dot");
      of.open(path, std::ios::trunc | std::ios::out);
      Utils::Dot* dot = new Utils::Dot();
      //of << dot->generateDotRepresentation(generateDotTypeVector());
      of.close();
    }
    else {
      std::cout << "Is close" << "\n";
    }
  }

  void Graph::directTransitiveClosure(int id) {
    Utils::PerformanceMetrics* metrics = new Utils::PerformanceMetrics();
    metrics->setupOutputFile("metrics.txt");
    Node* n = searchById(id);
    std::string path(ROOT_DIR);
    #ifdef LINUX
    struct sysinfo memInfo;
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;
    metrics->setMemoryUsed(physMemUsed);
    #endif
    path.append("out.dot");
    std::cout << path << "\n";
    #ifdef TEST_CASE
    metrics->setupTestSuit("FileSystem");
    metrics->startClock();
    #endif
    dt->startGraph(path);
    Utils::DotType type = getAllNodesConnected(id);
    dt->endGraph();
    #ifdef TEST_CASE
    metrics->endClock();
    #endif
    visited.clear();
  }

  Utils::DotType Graph::getAllNodesConnected(int id) {
    Utils::DotType value;
    value.id = id;
    visited.push_back(id);
    Node* node = searchById(id);
    if(node->beenVisited()) {
      value.active = false;
      return value;
    }
    else {
      node->visited();
    }
    Edge* edge = node->getEdge();
    while(edge != nullptr) {
      value.connected.push_back(getAllNodesConnected(edge->getTo()));
      edge = edge->getNext();
    }
    #ifdef OUTPUTMODE_FILESYSTEM
    dt->outputDotRepresentation(value);
    #else
    dt->consoleDotRepresentation(value);
    #endif
    return value;
  }

  // b) Fecho transitivo indireto:
  void Graph::indirectTransitiveClosure(int id) {
    std::vector<int> indirectClosure;
    Node* vertex = this->node;
    Node* assistant = nullptr;
    Node* target = searchById(id);
    Edge* edge;

    // Ideia similar ao fecho transitivo direto:
    Utils::Dot* dot = new Utils::Dot();
    std::string path(ROOT_DIR);
    std::cout << path << "\n";
    path.append("indirectTransitiveClosure.dot");
    std::cout << path << "\n";
    std::fstream file;
    file.open(path, std::ios::trunc | std::ios::out);
    if(!file.is_open()) {
      std::cout << "NOT OPEN" << "\n";
    }
    
    std::cout << "Fecho transitivo indireto: ";
    while(vertex!=nullptr) {
      this->setAllNodesVisitedFalse();
      deepPath(vertex);

      if(target->beenVisited() && (vertex->id!=target->id)) {
        indirectClosure.push_back(vertex->id);
        std::cout << vertex->id << " ";
      }
      
      vertex = vertex->getNext();
    }

    file << dot->generateDotRepresentation(indirectClosure, id);
    file.close();
  }

  void Graph::setAllNodesVisitedFalse() {
    Node* node = this->node;
    while(node!=nullptr) {
      node->setVisitFalse();
      node = node->getNext();
    }
  }

  void Graph::deepPath(Node* node) {
    Node* assistant = nullptr;
    node->visited();
    Edge *edge = node->getEdge();

    while(edge != nullptr) {
      assistant = this->searchById(edge->getTo());
      if(!assistant->beenVisited()) {
        this->deepPath(assistant);
      }
      edge = edge->getNext();
    }
  }

  int Graph::neighborsConnected(int id, int* p, int size) {
    Node* s = searchById(id);
    Edge* edge = s->getEdge();
    int count = 0;
    while(edge != nullptr) {
      for(int b = 0; b < size; b++) {
        if(p[b] == edge->getTo() && !(std::find(s->visitedBy.begin(), s->visitedBy.end(), p[b]) != s->visitedBy.end())) {
          count++;
          searchById(p[b])->visitedBy.push_back(id);
          break;
        }
      }
      edge = edge->getNext();
    }
    return count;
  }

  float Graph::clusteringCoeficient(int id) {
    Node* s = searchById(id);
    Edge* edge = s->getEdge();
    int* connected = new int[s->getEdgeCount()];
    int clustering = 0;
    int index = 0;
    while(edge != nullptr) {
      connected[index] = edge->getTo();
      index++;
      edge = edge->getNext();
    }
    for(int i = 0; i < index; i++) {
      clustering += neighborsConnected(connected[i], connected, index);
    }
    if(s->getEdgeCount() == 0) return 0;
    float clusteringRealValue = (float)((float)clustering/s->getEdgeCount());
    return clusteringRealValue;
  }

  float Graph::clusteringGlobalCoeficient() {
    float sum = 0.0;
    Node* p = this->node;
    while(p != nullptr) {
      sum += clusteringCoeficient(p->id);
      p = p->getNext();
    }
    return sum;
  }

  // i) Árvore dada pelo caminhamento em profundidade:

  /*std::vector<Edge> Graph::deepPathTree(Node* vertex) {
    vertex->visited();
    Node* assistant = nullptr;
    Edge* edgeAssistant = vertex->getEdge();

    std::allocator<Edge> aloc;
    std::vector<Edge, std::allocator<Edge>> edgesOfDeepPathTree;

    while(edgeAssistant != nullptr) {
      assistant = searchById(edgeAssistant->getTo());
      if(!assistant->beenVisited()) {
        edgesOfDeepPathTree.push_back(edgeAssistant, aloc);
        edgesOfDeepPathTree.push_back(deepPathTree(assistant), aloc);
      }
    }

    return edgesOfDeepPathTree;
  }*/
}