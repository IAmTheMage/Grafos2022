#include "../include/Graph.h"
#include "string"
#include "vector"

int main(int argc, char** argv) {
  std::cout << "Gerando grafo" << std::endl;
  Graph::Graph* graph = new Graph::Graph(argv);
  std::cout << "Selecione o algoritmo a ser executado: " << std::endl;
  std::cout << "0: Fecho transitivo direto" << std::endl;
  std::cout << "1: Fecho transitivo indireto" << std::endl;
  std::cout << "2: Coeficiente de agrupamento local" << std::endl;
  std::cout << "3: Coeficiente de agrupamento médio" << std::endl;
  std::cout << "4: Dijkstra" << std::endl;
  std::cout << "5: Floyd" << std::endl;
  std::cout << "6: Prim" << std::endl;
  std::cout << "7: Kruskal" << std::endl;
  std::cout << "8: Caminhamento em profundidade" << std::endl;
  std::cout << "9: Gerar representacao em dot do grafo" << std::endl;
  int selected;
  std::cin >> selected;
  switch(selected) {
    case 0:
      std::cout << "Digite o id de um vértice: ";
      int id;
      std::cin >> id;
      graph->directTransitiveClosure(id);
      break;
    case 1:
      std::cout << "Digite o id de um vértice: ";
      int id2;
      std::cin >> id2;
      graph->indirectTransitiveClosure(id2);
      break;
    case 2:
      std::cout << "Digite o id de um vértice: ";
      int id3;
      std::cin >> id3;
      std::cout << graph->clusteringCoeficient(id3) << std::endl;
      break;
    case 3:
      std::cout << graph->clusteringGlobalCoeficient() << std::endl;
      break;
    case 4:
      std::cout << "Digite o id do vértice de origem: ";
      int origin1;
      std::cin >> origin1;
      std::cout << "Digite o id do vértice de destino: ";
      int destination1;
      std::cin >> destination1;
      Graph::Graph::printList(graph->dijkstra(origin1, destination1));
      break;
    case 5:
      std::cout << "Digite o id do vértice de origem: ";
      int origin2;
      std::cin >> origin2;
      std::cout << "Digite o id do vértice de destino: ";
      int destination2;
      std::cin >> destination2;
      graph->floyd(origin2, destination2);
      break;
    case 6:
      {
        std::string path;
        std::cout << "Digite os vértices separados por ;" << std::endl;
        std::cin >> path;
        std::stringstream stream(path);
        std::string b;
        std::vector<int> subN;
        while(std::getline(stream, b, ';')) {
          subN.push_back(std::stoi(b));
        }
        Graph::Graph* graph2 = graph->vertexInducedSubgraph(subN);
        graph->algorithmPrim(graph2);
      }
      break;
    case 7:
      {
        std::string path;
        std::cout << "Digite os vértices separados por ;" << std::endl;
        std::cin >> path;
        std::stringstream stream(path);
        std::string b;
        std::vector<int> subN;
        while(std::getline(stream, b, ';')) {
          subN.push_back(std::stoi(b));
        }
        Graph::Graph* graph2 = graph->vertexInducedSubgraph(subN);
        std::cout << graph2->getCount() << "\n";
        graph->kruskal(graph2);
      }
      break;
    case 8: 
      std::cout << "Digite o id de um vértice: ";
      int id8;
      std::cin >> id8;
      graph->deepPathTree(id8);
      break;
    case 9:
      graph->generateGraphVizRepresentation();
    default:
      break;
  }

  return 0;
}