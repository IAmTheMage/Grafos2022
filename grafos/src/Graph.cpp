#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"
#include "config.h"
#include "utility"
#include <queue>
#define INF 99999999

namespace Graph {
  Graph::Graph(char** args) {
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
  }

  void Graph::directTransitiveClosure(int id) {
    Utils::Dot* dot = new Utils::Dot();
    Node* n = searchById(id);
    std::string path(ROOT_DIR);
    std::cout << path << "\n";
    path.append("out.dot");
    std::cout << path << "\n";
    std::fstream of;
    of.open(path, std::ios::trunc | std::ios::out);
    if(!of.is_open()) {
      std::cout << "NOT OPEN" << "\n";
    }
    Edge* s = n->getEdge();
    int index = 0;
    std::vector<int> closure;
    while(s != nullptr) {
      int to = s->getTo();
      if(!(std::find(closure.begin(), closure.end(), to) != closure.end())) {
        closure.push_back(to);
      }
      for(auto i : getAllNodesConnected(to)) {
        if(!(std::find(closure.begin(), closure.end(), i) != closure.end())) {
          closure.push_back(i);
        }
      }
      s = s->getNext();
    } 
    of << dot->generateDotRepresentation(closure, id);
    of.close();
  }

  std::vector<int> Graph::getAllNodesConnected(int id) {
    Node* node = searchById(id);
    std::vector<int> vec;
    vec.reserve(5);
    Edge* edge = node->getEdge();
    while(edge != nullptr) {
      vec.push_back(edge->getTo());
      edge = edge->getNext();
    }
    return vec;
  }

  std::vector<Utils::DotType> Graph::generateDotTypeVector() {
    Node* p = node;
    Utils::Dot* dot = new Utils::Dot();
    std::vector<Utils::DotType> dots;
    while(p != nullptr) {
      std::vector<int> connected = getAllNodesConnected(node->id);
      Utils::DotType type = {node->id, connected};
      dots.push_back(type);
    }
    std::cout << dot->generateDotRepresentation(dots);
    return dots;
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


  void Graph::algorithmPrim(std::vector< std::vector<std::pair<int,int>>> &subGraph) {
    
    int order = subGraph.size();

    std::vector<int> distance(order, INF);
    distance[0] = 0;

    std::vector<bool> visited(order, false);
    
    std::vector<int> parent(order,-1);

    std::priority_queue<std::pair<int,int>,
    std::vector<std::pair<int,int>>,
    std::greater<std::pair<int,int>> > priorityQueue;

    priorityQueue.push( std::make_pair(distance[0], 0));

    while(!priorityQueue.empty()) {
      int u = priorityQueue.top().second;
      priorityQueue.pop();

      visited[u] = true;

      for(std::pair<int,int> edge: subGraph[u]) {
        int v = edge.first;
        int weigth = edge.second;

        if(!visited[v] && distance[v] > weigth) {
          distance[v] = weigth;
          priorityQueue.push(std::make_pair(distance[v], v));
          parent[v] = u;
        }
      }
    }

     for(int i=1;i<order;i++){
          std::cout << parent[i] << " - "  << i << std::endl;
      }
  }

  std::vector<std::vector<std::pair<int, int>>> Graph::vertexInducedSubgraph(std::vector<int> &subVertex) {

    Node* node;
    Edge* edge;
    std::vector< std::vector<std::pair<int,int>>> subGraph(subVertex.size());

    for(int i = 0;i < subVertex.size(); i++) {    
      node = searchById(subVertex[i]);
      for(edge = node->getEdge(); edge != nullptr; edge = edge->getNext()) {
        for(int j = 0; j < subVertex.size();j++) {
          if(edge->getTo() == subVertex[j]) {
            std::cout << edge->getFrom() << "-->" << edge->getTo() << " - " << edge->getWeight() << std::endl;
            subGraph[edge->getFrom()].push_back( {edge->getTo(), edge->getWeight()} );
          }
        }
      }
    }
    
    return subGraph;
 }
}