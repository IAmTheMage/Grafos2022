#include "iostream"
#include "chrono"
#include "string"
#include "config.h"
#include "fstream"

using namespace std::chrono;

namespace Utils {
  #ifndef PEFORMANCEMETRICS
  #define PEFORMANCEMETRICS
  class PerformanceMetrics {
    public:
      PerformanceMetrics();
      ~PerformanceMetrics();
      void setupOutputFile(std::string path);
      void setupTestSuit(std::string suit);
      void startClock();
      void endClock();
      float _end();
      void setMemoryUsed(long long _memory) {
        this->memoryUsed = _memory;
      }
    private:
      system_clock::time_point start;
      system_clock::time_point end;
      std::chrono::duration<double> globalDiff;
      std::ofstream outputManager;
      long long memoryUsed;
  };
  #endif;
}