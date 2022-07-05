#include "../include/Dot.h"
#include "iostream"

namespace Utils {
  Dot::Dot() {
    
  }

  Dot::~Dot() {

  }

  std::string Dot::generateDotRepresentation(std::vector<int> p, int id) {
    processedInfo.append("digraph D {");
    processedInfo.append("\n");
    processedInfo.append(std::to_string(id));
    processedInfo.append(" -> {");
    for(int b = 0; b < p.size(); b++) {
      if(b < p.size() - 1 ) {
        processedInfo.append(std::to_string(p[b]));
        processedInfo.append(",");
      }
      else {
        processedInfo.append(std::to_string(p[b]));
      }
    }
    processedInfo.append("}\n}");
    return processedInfo;
  }

  void Dot::startGraph(std::string path) {
    std::cout << path << "\n";
    of.open(path, std::ios::trunc | std::ios::out);
    if(!of.is_open()) {
      std::cout << "Error\n";
    }
    data.append("digraph D {\n");
    of << data;
  }

  void Dot::generateDotRepresentation(DotType p) {
    data.append(std::to_string(p.id));
    data.append(" -> {");
    for(int i = 0; i < p.connected.size(); i++) {
      data.append(std::to_string(p.connected[i].id));
      if(i != p.connected.size() - 1) data.append(",");
    }
    data.append("}\n");
    for(auto b : p.connected) {
      if(b.active) {
        generateDotRepresentation(b);
      }
    }
  }

  void Dot::outputDotRepresentation(DotType p) {
    of << p.id;
    of << " -> {";
    for(int i = 0; i < p.connected.size(); i++) {
      of << p.connected[i].id;
      if(i != p.connected.size() - 1) of << ",";
    }
    of << "}\n";
  }

  void Dot::consoleDotRepresentation(DotType p) {
    std::cout << p.id;
    std::cout << " -> {";
    for(int i = 0; i < p.connected.size(); i++) {
      std::cout << p.connected[i].id;
      if(i != p.connected.size() - 1) std::cout << ",";
    }
    std::cout << "}\n";
  }

  void Dot::endGraph() {
    of << "}";
    std::cout << "Finalizada renderizacao\n";
    of.close();
  }
}