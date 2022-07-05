#include "../include/PerformanceMetrics.h"

namespace Utils {
  PerformanceMetrics::PerformanceMetrics() {

  }

  PerformanceMetrics::~PerformanceMetrics() {

  }

  void PerformanceMetrics::setupOutputFile(std::string path) {
    std::string realPath = ROOT_DIR;
    realPath.append(path);
    this->outputManager.open(realPath, std::ios::app | std::ios::out);
    outputManager.seekp(0, std::ios::beg);
    int initial = outputManager.tellp();
    outputManager.seekp(0, std::ios::end);
    int end = outputManager.tellp();
    if(end - initial <= 0) {
      outputManager << "Suit; Metric; Memory Usage" << std::endl;
    }
  }
  
  void PerformanceMetrics::setupTestSuit(std::string suit) {
    std::cout << "Initializing test for suit: " << suit << std::endl;
    outputManager << suit << ";";
  }

  void PerformanceMetrics::startClock() {
    start = std::chrono::system_clock::now();
  }

  void PerformanceMetrics::endClock() {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Tempo decorrido para execucao do codigo: " << diff.count();
    outputManager << diff.count() << ";" << this->memoryUsed << "KB" << std::endl;
    outputManager.close();
  }
}