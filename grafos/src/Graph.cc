#include "../include/Graph.h" 
#include "string.h"

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
    while(t != NULL) {
      if(t->id == id) {
        break;
      }
      t = t->getNext();
    }
    return t;
  }

  void Graph::setFiles(std::string in, std::string out) {
    std::fstream ip(in, std::ios::in);
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
    of.open(out, std::ios::ate | std::ios::trunc | std::ios::out);
  }

  void Graph::directTransitiveClosure(int id) {
    Node* n = searchById(id);
    of.open("out.txt", std::fstream::out | std::fstream::app);
    std::cout << n->getEdgeCount() << "\n";
    Edge* s = n->getEdge();
    int index = 0;
    while(s != NULL) {
      int i = s->getTo();
      of.write((char*)"ss", 2);
      i = i;
      s = s->getNext();
    }
    of.close();
  }
}