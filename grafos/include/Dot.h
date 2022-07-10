#include "string"
#include "vector"
#include "fstream"

namespace Utils {
  #ifndef DOT
  #define DOT
  struct DotType {
    int id;
    bool active = true;
    std::vector<DotType> connected;
    bool firstInstance = true;
  };

  struct ReturnEdge {
    int from;
    int to;
  };

  class Dot {
    public:
      Dot();
      ~Dot();
      std::string generateDotRepresentation(std::vector<int> p, int id);
      void generateDotRepresentation(Utils::DotType p);
      void outputDotRepresentation(Utils::DotType p);
      void consoleDotRepresentation(Utils::DotType p);
      void startGraph(std::string path);
      void generateGraphRepresentation(Utils::DotType p);
      void endGraph();
      void begin() {
        data.append("digraph D {\n");
        of << this->data;
      }
      void commit() {
        of << data << std::endl;
      }

      std::string getData() {
        return this->data;
      }
    private:
      std::string processedInfo;
      std::string data;
      std::ofstream of;
  };
  #endif
}