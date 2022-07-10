#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"

#include "config.h"

namespace Graph {
  Graph::Graph(char** args) {
    dt = new Utils::Dot();
    metrics = new Utils::PerformanceMetrics();
    std::string outputPath("instance.txt");
    metrics->setupOutputFile(outputPath);
    metrics->setupTestSuit("InstanceGraph");
    metrics->startClock();
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
    }
    else {
      std::cout << "Is close" << "\n";
    }
    std::string graphPath;
    graphPath.append(ROOT_DIR);
    graphPath.append("graph.dot");
    int id = node->id;
    Utils::DotType t = getAllNodes(id);
    dt->startGraph(graphPath);
    dt->generateDotRepresentation(t);
    dt->commit();
    dt->endGraph();
    metrics->endClock();
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
    Utils::DotType type = evaluateDirectTransitiveClosure(id);
    dt->endGraph();
    #ifdef TEST_CASE
    metrics->endClock();
    #endif
    visited.clear();
    std::cout << "Arestas de retorno: " << "\n";
    std::cout << output << "\n";
    output.clear();
    while(!stackz.empty()) stackz.pop();
  }

  Utils::DotType Graph::evaluateDirectTransitiveClosure(int id) {
    Utils::DotType value;
    value.id = id;
    if(!(std::find(visited.begin(), visited.end(), id) != visited.end())) {
      visited.push_back(id);
      Node* node = searchById(id);
      Edge* edge = node->getEdge();
      while(edge != nullptr) {
        value.connected.push_back(evaluateDirectTransitiveClosure(edge->getTo()));
        edge = edge->getNext();
      }
    }
    else {
      output.append(std::to_string(stackz.top()));
      output.append(" -> ");
      output.append(std::to_string(id));
      output.append("\n");
      value.active = false;
    }
    #ifdef OUTPUTMODE_FILESYSTEM
    dt->outputDotRepresentation(value);
    #else
    dt->consoleDotRepresentation(value);
    #endif
    stackz.push(id);
    return value;
  }

  Utils::DotType Graph::getAllNodes(int id) {
    Utils::DotType value;
    value.id = id;
    if(!(std::find(visited.begin(), visited.end(), id) != visited.end())) {
      visited.push_back(id);
      Node* node = searchById(id);
      Edge* edge = node->getEdge();
      while(edge != nullptr) {
        value.connected.push_back(evaluateDirectTransitiveClosure(edge->getTo()));
        edge = edge->getNext();
      }
    }
    return value;
  }

  std::vector<Utils::DotType> Graph::generateDotTypeVector() {
    /*Node* p = node;
    Utils::Dot* dot = new Utils::Dot();
    std::vector<Utils::DotType> dots;
    while(p != nullptr) {
      std::vector<int> connected = evaluateDirectTransitiveClosure(node->id);
      Utils::DotType type = {node->id, connected};
      dots.push_back(type);
    }
    std::cout << dot->generateDotRepresentation(dots);
    return dots;*/
  }

  // Fecho transitivo indireto:
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
    
    while(vertex!=nullptr) {
      this->setAllNodesVisitedFalse();
      deepPath(vertex);

      if(target->beenVisited() && (vertex->id!=target->id)) {
        indirectClosure.push_back(vertex->id);
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

  /*void Graph::deepPath(Node* node) {
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
  }*/

  Utils::DotType Graph::deepPath(Node* node) {
    Node* assistant = nullptr;
    node->visited();
    Edge *edge = node->getEdge();
    Utils::DotType values;
    values.id = node->id;
    while(edge != nullptr) {
      assistant = this->searchById(edge->getTo());
      if(!assistant->beenVisited()) {
        values.connected.push_back(this->deepPath(assistant));
      }
      else {
        values.active = false;
      }
      edge = edge->getNext();
    }
    return values;
  }
}