#include "../include/Dot.h"
#include "iostream"


/**
 * @brief Classe para gerar o dot
 * 
 */
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

  void Dot::writeOnFile(std::string path, std::vector<WeightedDot> p, bool digraph) {
    std::ofstream oof(path, std::ios::trunc | std::ios::out);
    std::string outData;
    if(digraph) {
      oof << "digraph {\n";
    }
    else {
      oof << "graph {\n";
    }
    for(WeightedDot dot : p) {
      std::string n1; std::string n2; std::string weight;
      n1 = std::to_string(dot.origin);
      n2 = std::to_string(dot.destination);
      weight = std::to_string(dot.weight);
      outData.append(n1);
      if(digraph) {
        outData.append(" -> ");
      }
      else {
        outData.append(" -- ");
      }
      outData.append(n2);
      outData.append("[weight=");
      outData.append(weight);
      outData.append("]\n");
    }
    oof << outData;
    oof << "}\n";
  }

  void Dot::writeOnFile(Edge* edge) {
    std::cout << "Write edge: " << edge->getTo() << "\n";
    of << edge->getFrom() << " -> " << edge->getTo() << " [weight=" << edge->getWeight() << "]\n";
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

  void Dot::startGraph(std::string path, bool digraph) {
    of.open(path, std::ios::trunc | std::ios::out);
    if(digraph) {
      data.append("digraph {\n");
    }
    else {
      data.append("graph {\n");
    }
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
    if(!p.active || p.connected.size() == 0) return;
    of << p.id;
    of << " -> {";
    for(int i = 0; i < p.connected.size(); i++) {
      of << p.connected[i].id;
      if(i != p.connected.size() - 1) of << ",";
    }
    of << "}\n";
  }

  void Dot::generateGraphRepresentation(DotType p) {
    if(!p.active || p.connected.size() == 0) return;
    of << p.id;
    of << " -> {";
    for(int i = 0; i < p.connected.size(); i++) {
      of << p.connected[i].id;
      if(i != p.connected.size() - 1) of << ",";
    }
    of << "}\n";
  }

  void Dot::consoleDotRepresentation(DotType p) {
    if(!p.active || p.connected.size() == 0) return;
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