#include "string"
#include "vector"
#include "fstream"


namespace Utils {
  #ifndef DOT
  #define DOT
  struct DotType {
    int id;
    std::vector<DotType> connected;
    bool active = true;
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
      void endGraph();
      void begin() {
        data.append("digraph D {\n");
        of << data;
      }
    private:
      std::string processedInfo;
      std::string data;
      std::ofstream of;
  };
  #endif
}