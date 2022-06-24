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

  std::string Dot::generateDotRepresentation(std::vector<DotType> p) {
    processedInfo.append("digraph D {");
    processedInfo.append("\n");
    for(DotType type : p) {
      processedInfo.append(std::to_string(type.id));
      processedInfo.append(" -> {");
      for()
    }
    processedInfo.append("}");
    return processedInfo;
  }
}