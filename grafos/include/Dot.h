#include "string"
#include "vector"


namespace Utils {
  #ifndef DOT
  #define DOT
  struct DotType {
    int id;
    std::vector<int> connected;
  }

  class Dot {
    public:
      Dot();
      ~Dot();
      std::string generateDotRepresentation(std::vector<int> p, int id);
      std::string generateDotRepresentation(std::vector<Utils::DotType> p);
    private:
      std::string processedInfo;
  };
  #endif
}