/*
  Trabalho por Gustavo Gonçalves Silva, Yan Messias, Pedro Leão e Matheus Neves
*/

#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"
#include "math.h"
#include "time.h"
#include "map"
#include "cmath"
#ifdef IS_CMAKE
#include "config.h"
#endif

#ifdef LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif

#ifndef ROOT_DIR
#define ROOT_DIR "./grafos/src"
#endif



namespace Graph {
  Graph::Graph(char** args) {
    dt = new Utils::Dot();
    edgeType = EdgeType::PONDERED;
    graphType = GraphType::NONDIRECTED;
    std::string in(args[1]);
    std::string temp(args[2]);
    outputFilePath = temp;
    setFiles(in);
  }

  Node* Graph::searchById(int id) {
    Node* t = this->node;
    Node* p = nullptr;
    while(t != NULL) {
      if(t->id == id) {
        p = t;
        break;
      }
      t = t->getNext();
    }
    return p;
  }
  
  Graph::Graph(EdgeType edge, GraphType graph) {
    this->edgeType = edge;
    this->graphType = graph;
    this->node = nullptr;
    this->count = 0;
    dt = new Utils::Dot();
  }

  /**
   * @brief Função que lê os dados do arquivo e gera tanto os nós quanto as arestas do grafo
   * 
   * @param in 
   * @param out 
   */
  void Graph::setFiles(std::string in) {
    std::string path("");
    path.append(in);
    std::fstream ip(path, std::ios::in);
    if(ip.is_open()) {
      std::string b;
      std::getline(ip, b);
      std::stringstream sstream(b);
      std::string tp;
      int numberOfNodes = 0;
      while(std::getline(sstream, tp, ' ')) {
        numberOfNodes++;
      }
      
      std::vector<int> values;
      if(numberOfNodes == 1) {
        ip.close();
        readNCSFile(in);
      }
      else {
        std::cout << "Clustering file" << std::endl;
        ip.close();
        readClusteringFile(in);
      }
    }
    else {
      std::cout << "Is close" << "\n";
    }
    std::cout << "Grafo gerado" << std::endl;
  }

  /**
   * @brief Função que lê arquivos do tipo NCS (Exemplo: 30_5_270003)
   * 
   * @param in 
   */
  void Graph::readNCSFile(std::string in) {
    std::ifstream ip(in, std::ios::in);
    int numberOfNodes;
    int numberOfClusters;
    int weightOfClusters;
    std::string temp;
    std::getline(ip, temp);
    numberOfNodes = std::atoi(temp.c_str());
    std::getline(ip, temp);
    numberOfClusters = std::atoi(temp.c_str());
    std::getline(ip, temp);
    weightOfClusters = std::atoi(temp.c_str());
    for(int i = 0; i < numberOfClusters; i++) {
      Cluster cluster;
      cluster.lowerLimit = 0;
      cluster.totalWeight = 0;
      cluster.upperLimit = weightOfClusters;
      clusters.push_back(cluster);
    }
    std::vector<int> nodeWeights;
    for(int i = 0; i < numberOfNodes; i++) {
      std::getline(ip, temp);
      instanceNewNode(i, std::atoi(temp.c_str()));
    }
    for(int i = 0; i < numberOfNodes; i++) {
      for(int j = 0; j < numberOfNodes; j++) {
        std::getline(ip, temp, ' ');
        searchById(i)->makeRelationship(j, std::atoi(temp.c_str()));
      }
    } 
  }

  /**
   * @brief Função que lê arquivos do tipo RAN_REAL ou SPARSE
   * 
   * @param path 
   */
  void Graph::readClusteringFile(std::string path) {
    std::ifstream ip(path, std::ios::in);
    std::string b;
    ClusterInfo clusterInfo;
    int index = 0;
    while(getline(ip, b)) {
      if(index == 0) {
        clusterInfo = readClusteringFirstPart(b);
        index++;
      }
      else if(index >= 1) {
        addClusteringEdge(b);
      }
    }
    clusters = clusterInfo.clusters;
  }

  /**
   * @brief Função auxiliar para criar uma aresta no grafo baseado nos inputs tipo RAN_REAL ou SPARSE
   * 
   * @param data 
   */
  void Graph::addClusteringEdge(std::string data) {
    std::stringstream stream(data);
    int index = 0;
    std::string temp;
    Node *from, *to;
    float weight = 0.0f;
    while(getline(stream, temp, ' ')) {
      if(index == 0) {
        from = searchById(std::atoi(temp.c_str()));
      }
      else if(index == 1) {
        to = searchById(std::atoi(temp.c_str()));
      }
      else if(index == 2) {
        weight = std::atof(temp.c_str());
      }
      index++;
    }
    from->makeRelationship(to->id, weight);
    to->makeRelationship(from->id, weight);
  }



  /**
   * @brief Função para auxiliar na leitura de arquivos do tipo RAN_REAL e SPARSE
   * 
   * @param data 
   * @return ClusterInfo 
   */
  ClusterInfo Graph::readClusteringFirstPart(std::string data) {
    std::stringstream strstream(data);
    int index = 0;
    ClusterInfo clusterInfo;
    int nodeAmount = 0;
    int nodeRealAmount = 0;
    int nodeIndex = 0;
    std::vector<Cluster> clusters;
    std::vector<int> weights;
    int ind = 0;
    while(getline(strstream, data, ' ')) {
      if(index == 0) {
        nodeAmount = std::atoi(data.c_str());
        nodeRealAmount = nodeAmount;
      }
      else if(index == 1) {
        clusterInfo.numberOfClusters = std::atoi(data.c_str());
      }
      else if(index == 2) {
        if(data == "ds") {
          clusterInfo.type = 'd';
        }
        else {
          clusterInfo.type = 's';
        }
        std::string pt;
        getline(strstream, pt, 'W');
        std::stringstream strstreamtemp(pt);
        std::string temp;
        bool cond = false;
        while(getline(strstreamtemp, temp, ' ')) {
          Cluster cluster;
          cluster.totalWeight = 0.0f;
          cluster.lowerLimit = std::atoi(temp.c_str());
          getline(strstreamtemp, temp, ' ');
          cluster.upperLimit = std::atoi(temp.c_str());
          clusters.push_back(cluster);
        }
      }
      else if(index >= 3) {
        weights.push_back(std::atoi(data.c_str()));
        ind++;
      }
      index++;
    }
    int nodeId = 0;
    weights.erase(weights.begin());
    this->instanceNewNode(nodeId, weights[0], clusterInfo.numberOfClusters);  
    this->node->instanceMultiplesNodes(nodeAmount, weights, clusterInfo.numberOfClusters);  
    clusterInfo.clusters = clusters;
    return clusterInfo;
  }

  /**
   * @brief Função para gerar a representação em .dot do grafo
   * 
   */
  void Graph::generateGraphVizRepresentation() {
    std::string filePath(outputFilePath.c_str());
    //Vetor de refêrencia para gerar o dot
    std::vector<Utils::WeightedDot> dots;
    Node* p = node;
    Edge* edge = nullptr;
    int index = 0;
    while(p != nullptr) {
      edge = p->getEdge();
      for(int i = 0; i < p->getEdgeCount();i++) {
        //Algoritmo que pega todas as arestas, verifica se elas não repetem e caso não,
        //adiciona no vetor de refêrencia
        if(!searchDots(dots, p->id, edge->getTo())) {
          Utils::WeightedDot dot;
          dot.origin = p->id;
          dot.destination = edge->getTo();
          dot.weight = edge->getWeight();
          dots.push_back(dot);
        }
        edge = edge->getNext();
      }
      p = p->getNext();
    }
    if(graphType == DIRECTED) {
      //Metodo para gerar o dot
      dt->writeOnFile(filePath, dots, true);
    }
    else {
      //Metodo para gerar o dot
      dt->writeOnFile(filePath, dots, false);
    }
  }

  bool Graph::searchDots(std::vector<Utils::WeightedDot>& p, int from, int to) {
    for(auto b : p) {
      if((b.origin == from && b.destination == to) || (b.origin == to && b.destination == from )) {
        return true;
      }
    }
    return false;
  }

  void Graph::directTransitiveClosure(int id) {
    Utils::PerformanceMetrics* metrics = new Utils::PerformanceMetrics();
    metrics->setupOutputFile("metrics.txt");
    Node* n = searchById(id);
    std::string path(outputFilePath.c_str());
    #ifdef LINUX
    struct sysinfo memInfo;
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;
    metrics->setMemoryUsed(physMemUsed);
    #endif
    #ifdef TEST_CASE
    metrics->setupTestSuit("FileSystem");
    metrics->startClock();
    #endif
    dt->startGraph(path);
    Utils::DotType type = getAllNodesConnected(id);
    dt->endGraph();
    #ifdef TEST_CASE
    metrics->endClock();
    #endif
    visited.clear();
  }

  /**
   * @brief Pega todos os nós conectados com o nó selecionado
   * 
   * @param id 
   * @return Utils::DotType 
   */
  Utils::DotType Graph::getAllNodesConnected(int id) {
    //Cria um value para a classe dot gerar o arquivo .dot
    Utils::DotType value;
    value.id = id;
    visited.push_back(id);
    Node* node = searchById(id);
    if(node->beenVisited()) {
      value.active = false;
      return value;
    }
    else {
      node->visited();
    }
    Edge* edge = node->getEdge();
    while(edge != nullptr) {
      value.connected.push_back(getAllNodesConnected(edge->getTo()));
      edge = edge->getNext();
    }
    #ifdef OUTPUTMODE_FILESYSTEM
    dt->outputDotRepresentation(value);
    #else
    dt->consoleDotRepresentation(value);
    #endif
    return value;
  }

  // b) Fecho transitivo indireto:
  void Graph::indirectTransitiveClosure(int id) {
    std::vector<int> indirectClosure;
    Node* vertex = this->node;
    Node* assistant = nullptr;
    Node* target = searchById(id);
    if(target == nullptr) return;
    Edge* edge;

    // Ideia similar ao fecho transitivo direto:
    Utils::Dot* dot = new Utils::Dot();
    std::string path(outputFilePath.c_str());
    std::cout << path << "\n";
    path.append("_indirect_transitive_closure.txt");
    std::cout << path << "\n";
    std::fstream file;
    file.open(path, std::ios::trunc | std::ios::out);
    if(!file.is_open()) {
      std::cout << "NOT OPEN" << "\n";
    }
    
    std::cout << "Fecho transitivo indireto: ";
    while(vertex!=nullptr) {
      this->setAllNodesVisitedFalse();
      deepPath(vertex);

      if(target->beenVisited() && (vertex->id!=target->id)) {
        indirectClosure.push_back(vertex->id);
        std::cout << vertex->id << " ";
      }
      
      vertex = vertex->getNext();
    }

    file << dot->generateDotRepresentation(indirectClosure, id);
    file.close();
  }

  /**
   * @brief Recebe o grafo em um formato vetorial (Util para ordenar)
   * 
   * @return std::vector<Node*> 
   */
  std::vector<Node*> Graph::getGraphInVectorFormat() {
    std::vector<Node*> v;
    Node* p = node;
    while(p != nullptr) {
      if(!p->beenVisited()) 
      v.push_back(p);
      p = p->getNext();
    }
    //Ordena o grafo baseado nos bias criados
    std::sort(v.begin(), v.end(), [](Node* n, Node* b) {
      return n->getClusteringRelativeValue() < b->getClusteringRelativeValue();
    });
    return v;
  }

  /**
   * @brief Função para iniciar cada cluster com 1 vértice
   * 
   */
  void Graph::initClusteringData() {
    std::vector<Node*> viz = getGraphInVectorFormat();
    for(int i = 0; i < clusters.size(); i++) {
      clusters[i].ids.push_back(viz.back()->id);
      clusters[i].currentWeight += viz.back()->getWeight();
      searchById(viz.back()->id)->visited();
      viz.pop_back();
    }
  }

  /**
   * @brief Calcula o impacto de determinado vértice dentro de um cluster
   * 
   * @param p 
   * @param clusterId 
   * @return float 
   */
  float Graph::calculateImpact(Node* p, int clusterId) {
    Cluster cluster = clusters[clusterId];
    float weight = 0.0f;
    for(int ids : cluster.ids) {
      Edge* edge = p->searchEdge(ids);
      if(edge != nullptr)
      weight += edge->getWeight();
      //std::cout << "Impacto somado do node " << p->id << " com o node: " << ids << " e " << edge->getWeight() << std::endl;
    }
    return weight;
  }

  /**
   * @brief Algoritmo guloso
   * 
   * @return float 
   */
  float Graph::greedy() {
    int tNodes;
    initClusteringData();
    std::vector<Node*> viz = getGraphInVectorFormat();
    //Condição para evitar repetição infinita devido a pouca capacidade e quantidade de clusters.
    tNodes = viz.size() >= 30 ? viz.size() : 5;
    int limit = 1;
    int close = 0;
    bool cond = false;
    //Enquanto o tamanho do vetor de candidatos não for zero
    //Executa a verificação de impacto para os proximos T nós
    while(viz.size() != 0) {
      for(int i = 0; i < clusters.size(); i++) {
        close++;
        //Caso close seja igual ao tamanho dos clusters quer dizer que a solução não é viavel
        if(close == clusters.size()) {
          std::cout << "Algoritmo guloso nao pode encontrar solucao viavel" << std::endl;
          return 1.0f;
        }
        std::vector<Impact> impacts;
        if(viz.size() < tNodes) tNodes = viz.size();
        if(viz.size() == 0) break;
        for(int b = 0; b < tNodes; b++) {
          int randomIndex = b;
          impacts.push_back({calculateImpact(viz[randomIndex], i), randomIndex});         
        }

        //Ordena o vetor de impactos para pegar o com maior impacto
        std::sort(impacts.begin(), impacts.end(), [](Impact& impact1, Impact& impact2) {
          return impact1.impact > impact2.impact;
        });
        if(viz[impacts[0].id]->getWeight() + clusters[i].currentWeight > clusters[i].upperLimit) {
          continue;
        }
        clusters[i].ids.push_back(viz[impacts[0].id]->id);
        clusters[i].currentWeight += viz[impacts[0].id]->getWeight();
        viz.erase(std::remove(viz.begin(), viz.end(), viz[impacts[0].id]), viz.end());
        close = 0;
      }
    }
    constructClusterSet();
    float r = getClusteringInfo();
    return r;
  }

  /**
   * @brief Função auxiliar para executar apenas o Algoritmo Guloso
   * 
   */
  void Graph::greedyResult() {
    //Iniciando execução
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy");
    performance->startClock();
    float s = greedy();
    if(s != 1.0f) {
      std::cout << "Resultado para o algoritmo guloso: " << s << std::endl;
    }
    //Nesse caso não encontrou solução viável
    else {
      return;
    }
    float time = performance->_end();
    std::string resultStr = getClustersStateInStringFormat(s, 1);
    std::ofstream off(outputFilePath, std::ios::trunc | std::ios::out);
    off << "Tempo decorrido: " << time << std::endl << resultStr;
    off.close();
    std::cout << std::endl << "Resultado em " << outputFilePath << std::endl;
  }

  /**
   * @brief Algoritmo guloso randomizado
   * 
  */
  void Graph::randomGreedy() {
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy Random");
    int tNodes;
    std::cout << "Indique o alfa: " << std::endl;
    float alpha = 0.05f;
    std::cin >> alpha;
    performance->startClock();
    float solution = this->generateSolution(alpha);
    if(solution == 1.0f) {
      std::cout << "Esse alpha nao possui solucao" << std::endl;
      return;
    }
    std::cout << "Resultado para Guloso randomizado: " << solution << std::endl;
    std::string resultStr = getClustersStateInStringFormat(solution, alpha);
    std::ofstream off(outputFilePath, std::ios::trunc | std::ios::out);
    float time = performance->_end();
    off << "Tempo decorrido: " << time << std::endl << resultStr;
    off.close();
    std::cout << std::endl << "Saida em " << outputFilePath << std::endl;
  }

  /**
   * @brief Guloso randomizado reativo adaptativo
   * 
   */
  void Graph::randomReactiveGreedy() {
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy Random Reactive");
    performance->startClock();

    //Vetor contendo os alfas selecionados
    std::vector<float> alphas;


    alphas.push_back(0.05);
    alphas.push_back(0.07);
    alphas.push_back(0.13);
    alphas.push_back(0.15);
    alphas.push_back(0.21);
    alphas.push_back(0.47);
    alphas.push_back(0.48);
    alphas.push_back(0.49);
    alphas.push_back(0.51);
    alphas.push_back(0.22);
    alphas.push_back(0.27);
    alphas.push_back(0.18);
    alphas.push_back(0.28);
    alphas.push_back(0.34);
    alphas.push_back(0.39);
    alphas.push_back(0.78);
    alphas.push_back(0.42);
    alphas.push_back(0.09);
    alphas.push_back(0.57);
    alphas.push_back(0.81);


    std::vector<float> medians; //Vetor de médias
    std::vector<float> sums; // Vetor de somas (suporte para as médias)
    std::vector<float> bestSolutions; // Vetor para armazenar a melhor solução para cada alfa
    std::vector<float> probabilities; // Vetor de probabilidades de cada alfa
    std::vector<float> q; // Suporte para atualização de probabilidades de cada alfa
    std::vector<int> solutionsAmount;
    std::map<int, std::string> clustersInfo; //Hash contendo a representação em string dos resultados, utilizado para analise e para armazenamento no arquivo de saída


    int sizeT = alphas.size();

    instanceVectors(medians, probabilities, sizeT); //Instancia os vetores de probabilidades e médias


    /*
      Inicia todos os vetores auxiliares com seus valores mínimos.
    */
    for(int i = 0; i < sizeT; i++) {
      bestSolutions.push_back(0.0f);
      q.push_back(0.0f);
      sums.push_back(0.0f);
      solutionsAmount.push_back(0);
    }


    int numInterations; //Número de interações
    int numToUpdate; // Número de interações até a atualização das probabilidades

    std::cout << "Digite o numero de interações: " << std::endl;
    std::cin >> numInterations;
    std::cout << "Digite o numero de blocos para atualizacao: " << std::endl;
    std::cin >> numToUpdate;

    int index = 0;
    while(index < numInterations) {
      //Caso o index atual seja dívisivel pelo numero de blocos, atualiza as probabilidades
      if(index != 0 && index % numToUpdate == 0) {
        /*std::cout << "Probabilities: " << std::endl;
        for(int i = 0; i < probabilities.size(); i++) {
          std::cout << probabilities[i] << ' ';
        }
        std::cout << std::endl;*/
        updateProbabilities(medians, probabilities, bestSolutions, q); //Atualiza as probabilidades
      }
      int selected = selectProbabilities(probabilities);
      float selectedAlpha = alphas[selected];
      float alphaSolution = generateSolution(selectedAlpha);
      //Atualiza as médias das melhores soluções
      updateMedians(medians, 
      alphaSolution, 
      alphas, sums, selectedAlpha, solutionsAmount
      );
      //Caso a melhor solução do alfa atual seja melhor que a melhor solução já encontrada
      //Atualiza o vetor com as melhores soluções
      if(bestSolutions[selected] < alphaSolution) {
        bestSolutions[selected] = alphaSolution;
        clustersInfo[alphaSolution] = getClustersStateInStringFormat(alphaSolution, selectedAlpha);
      }
      //std::cout << "Alpha result: " << alphaSolution << " in interaction: " << index << std::endl;
      std::cout << "Interacao: " << index + 1 << std::endl;
      this->cleanVisited();
      this->cleanClusters();
      index++;
    }
    //Ordena o vetor para obter a melhor solução
    std::sort(bestSolutions.begin(), bestSolutions.end());
    std::ofstream off(outputFilePath, std::ios::trunc | std::ios::out);
    float metrics = performance->_end();
    off << "Tempo de execucao: " << metrics << std::endl << clustersInfo[bestSolutions[bestSolutions.size() - 1]];
    off.close();
    std::cout << std::endl << "Best solution is: " << bestSolutions[bestSolutions.size() - 1] << std::endl;
  }


  /**
   * @brief Limpa o estado de todos os clusters
   * 
   */
  void Graph::cleanClusters() {
    for(int i = 0; i < clusters.size(); i++) {
      clusters[i].ids.clear();
      clusters[i].currentWeight = 0.0f;
      clusters[i].visited = false;
      clusters[i].totalWeight = 0.0f;
    }
  }

  /**
   * @brief Função auxiliar para gerar uma solução viavel para determinado alfa
   * 
   * @param alpha 
   * @return float 
   */
  float Graph::generateSolution(float alpha) {
    srand(time(NULL));
    int tNodes;
    //Inicia todos os clusters com 1 vértice
    initClusteringData();
    std::vector<Node*> viz = getGraphInVectorFormat();
    tNodes = (int)((viz.size() - 1) * alpha) != 0 ? (int)((viz.size() - 1) * alpha) : 1;
    //Caso o resultado do alfa seja 1 executa o guloso
    if(tNodes == 1) {
      return greedy();
    }
    int limit = 1;
    int close = 0;
    bool cond = false;
    //Quantidade de nós analisados por cada interação
    //Enquanto viz não for vazio executa a heuristica
    while(viz.size() != 0) {
      srand(time(NULL));
      for(int i = 0; i < clusters.size(); i++) {
        //Caso close seja igual ao tamanho dos clusters quer dizer que a solução não é viavel
        if(close == clusters.size()) return 1;
        close++;
        //Vetor de impactos para cada vértice atualizado
        std::vector<Impact> impacts;
        //Caso o tamanho de viz seja menor que 5, deixa de ser randomizado, e passa a pegar o de maior impacto
        if(viz.size() < 5) {
          cond = true;
        }
        if(viz.size() == 0) break;
        for(int b = 0; b < viz.size(); b++) {
          impacts.push_back({calculateImpact(viz[viz.size() - 1 - b], i), viz.size() - 1 - b});         
        }
        //Ordena o vetor pelo impacto de cada Node para o cluster i
        std::sort(impacts.begin(), impacts.end(), [](Impact& impact1, Impact& impact2) {
          return impact1.impact > impact2.impact;
        });
        tNodes = (int)((viz.size() - 1) * alpha) != 0 ? (int)((viz.size() - 1) * alpha) : 1;
        int select = rand() % tNodes;
        if(cond) select = 0;
        if(viz[impacts[select].id]->getWeight() + clusters[i].currentWeight > clusters[i].upperLimit) continue;
        clusters[i].ids.push_back(viz[impacts[select].id]->id);
        clusters[i].currentWeight += viz[impacts[select].id]->getWeight();
        viz.erase(std::remove(viz.begin(), viz.end(), viz[impacts[select].id]), viz.end());
        close = 0;
      }
    }
    constructClusterSet();
    return getClusteringInfo();
  }

  /**
   * @brief Função para atualizar as médias das soluções
   * 
   * @param medians 
   * @param solution 
   * @param alphas 
   * @param sums 
   * @param alpha 
   * @param solutionsAmount 
   */
  void Graph::updateMedians(
    std::vector<float>& medians,
    float solution,
    std::vector<float> alphas, 
    std::vector<float> sums,
    float alpha,
    std::vector<int> solutionsAmount
  ) {
    int index;
    for(int i = 0; i < alphas.size(); i++) {
      if(alpha == alphas[i]) {
        index = i;
        break;
      } 
    }
    sums[index] += solution;
    solutionsAmount[index] += solutionsAmount[index] + 1;
    medians[index] = sums[index] / solutionsAmount[index];
  }

  /**
   * @brief Função para instanciar os vetores de médias e probabilidades para o Guloso
   * randomizado reativo adaptativo
   * 
   * @param medians 
   * @param prob 
   * @param size 
   */
  void Graph::instanceVectors(std::vector<float>& medians, std::vector<float>& prob, int size) {
    float baseProbability = 1.0f / size;
    float baseMedian = 1.0f;
    for(int i = 0; i < size; i++) {
      prob.push_back(baseProbability);
      medians.push_back(baseMedian);
    }
  }

  /**
   * @brief Função para gerar a representação em string dos resultados para determinado alfa
   * 
   * @param result 
   * @param alpha 
   * @return std::string 
   */
  std::string Graph::getClustersStateInStringFormat(float result, float alpha) {
    std::string data;
    data.append("Melhor resultado: ");
    data.append(std::to_string(result));
    data.append("\nAlfa: ");
    data.append(std::to_string(alpha));
    int ind = 1;
    for(Cluster cluster : clusters) {
      data.append("\nVertices no cluster ");
      data.append(std::to_string(ind));
      data.append("\n");
      for(int ids : cluster.ids) {
        data.append("  ->");
        data.append(std::to_string(ids));
        data.append("\n");
      }
      data.append("\n");
      ind++;
    }
    return data;
  }

  /**
   * @brief Seleciona um alfa baseado nas probabilidades de cada um deles
   * 
   * @param prob 
   * @return int 
   */
  int Graph::selectProbabilities(std::vector<float>& prob) {
    std::vector<int> indexes;
    for(int i = 0; i < prob.size(); i++) {
      for(int j = 0; j < (int)(prob[i] * 100); j++) {
        indexes.push_back(i);
      }
    }
    int random = rand() % indexes.size();
    return indexes[random];
  }

  /**
   * @brief Atualiza as probabilidades
   * 
   * @param median 
   * @param probabilities 
   * @param bestSolutions 
   * @param q 
   */
  void Graph::updateProbabilities(
    std::vector<float>& median, 
    std::vector<float>& probabilities,
    std::vector<float>& bestSolutions,
    std::vector<float>& q
  ) 
  {
    float somaQ = 0.0f;
    for(int i = 0; i < median.size(); i++) {
      float tmp =  bestSolutions[i] / median[i];
      q[i] = std::pow((tmp), 100);
      somaQ += q[i];
    }
    for(int i = 0; i < median.size(); i++) {
      if(q[i] / somaQ != 0.0f)
      probabilities[i] = q[i] / somaQ;
    }

  }

  void Graph::sortByClusterPreference(int clusterId, std::vector<Node*> viz) {
    std::sort(viz.begin(), viz.end(), [](Node* p, Node* b) {
      return p->getSpecificBiasValue(p->currentCluster) > b->getSpecificBiasValue(b->currentCluster);
    });
  }

  /**
   * @brief Função para pegar a informação dos clusters (Qualidade da solução e desvio padrão)
   * 
   * @param printClusters 
   * @return float 
   */
  float Graph::getClusteringInfo(bool printClusters) {
    float variation = 0;
    float totalWeight = 0.0f;
    float median = 0;
    int ind = 1;
    for(Cluster cluster: clusters) {
      //std::cout << "Cluster: " << ind << " has weight: " << cluster.currentWeight << " and has degree: " << cluster.totalWeight << std::endl;
      totalWeight += cluster.totalWeight;
      ind++;
    }
    median = totalWeight / clusters.size();
    for(Cluster cluster: clusters) {
      variation += (cluster.totalWeight - median) * (cluster.totalWeight - median);
    }
    //variation /= (clusters.size() - 1);
    //float standardDeviation = sqrt(variation);
    //std::cout << std::endl << "Desvio padrao: " << standardDeviation << std::endl;
    if(printClusters) {
      printClustersInfo();
    }
    return totalWeight != 0 ? totalWeight : 1;
  }

  void Graph::printClustersInfo() {
    int ind = 1;
    for(Cluster cluster : clusters) {
      std::cout << "Vertexes in cluster " << ind << std::endl;
      for(int ids : cluster.ids) {
        std::cout << "  " << "->" << ids << std::endl;
      }
      std::cout << std::endl;
      ind++;
    }
  }


  /**
   * @brief Função para calcular o peso total de cada cluster
   * 
   */
  void Graph::constructClusterSet() {
    std::vector<int> reference;
    int index = 0;
    for(Cluster cluster : clusters) {
      cluster.totalWeight = 0;
      //Vetor de referências
      reference = cluster.ids;
      for(int id : cluster.ids) {
        Node* p = searchById(id);
        //Remove o item do vetor de referências ( para ele não ser contabilizado 2 vezes)
        reference.erase(std::remove(reference.begin(), reference.end(), id), reference.end());
        for(int toId : reference) {
          Edge* temp = p->searchEdge(toId);
          if(temp != nullptr) {
            clusters[index].totalWeight += temp->getWeight();
          }
        }
      }
      index++;
    }
  }


  void Graph::setAllNodesVisitedFalse() { 
    Node* node = this->node;
    while(node!=nullptr) {
      node->setVisitFalse();
      node = node->getNext();
    }
  }

  void Graph::deepPath(Node* node) {
    Node* assistant = nullptr;
    node->visited();
    Edge *edge = node->getEdge();

    while(edge != nullptr) {
      assistant = this->searchById(edge->getTo());
      if(!assistant->beenVisited()) {
        this->deepPath(assistant);
      }
      edge = edge->getNext();
    }
  }

  /**
   * @brief Retorna o numero de vizinhos conectados entre si de um vértice
   * 
   * @param id 
   * @param p 
   * @param size 
   * @return int 
   */
  int Graph::neighborsConnected(int id, int* p, int size) {
    Node* s = searchById(id);
    Edge* edge = s->getEdge();
    int count = 0;
    while(edge != nullptr) {
      for(int b = 0; b < size; b++) {
        if(p[b] == edge->getTo() && !(std::find(s->visitedBy.begin(), s->visitedBy.end(), p[b]) != s->visitedBy.end())) {
          count++;
          searchById(p[b])->visitedBy.push_back(id);
          break;
        }
      }
      edge = edge->getNext();
    }
    return count;
  }

  /**
   * @brief Algoritmo para encontrar o coeficiente de agrupamento local de um vértice do grafo
   * 
   * @param id 
   * @return float 
   */
  float Graph::clusteringCoeficient(int id) {
    Node* s = searchById(id);
    Edge* edge = s->getEdge();
    //Array para colocar os ids dos vértices connectados com o nó original
    int* connected = new int[s->getEdgeCount()];
    int clustering = 0;
    int index = 0;
    //Coloca todos os ids dos destinos das arestas em do nó selecinado em connected
    while(edge != nullptr) {
      connected[index] = edge->getTo();
      index++;
      edge = edge->getNext();
    }
    for(int i = 0; i < index; i++) {
      clustering += neighborsConnected(connected[i], connected, index);
    }
    if(s->getEdgeCount() == 0) return 0;
    float clusteringRealValue = (float)((float)clustering / (s->getEdgeCount() * (s->getEdgeCount() - 1) / 2));
    if(graphType == DIRECTED) {
      clusteringRealValue *= 0.5;
    }
    return clusteringRealValue;
  }

  /**
   * @brief Encontra o coeficiente médio do grafo
   * 
   * @return float 
   */
  float Graph::clusteringGlobalCoeficient() {
    //Apenas soma todos os coeficientes locais do grafo em uma váriavel e retorna ela
    float sum = 0.0;
    Node* p = this->node;
    while(p != nullptr) {
      sum += clusteringCoeficient(p->id);
      p = p->getNext();
    }
    return sum / getCount();
  }

  /**
   * @brief Pega a posição de um node baseado no id
   * @deprecated em favor de searchNodeById(id)->getPosition()
   * 
   * @param id 
   * @return int 
   */
  int Graph::getNodeReferenceIndex(int id) {
    Node* p = node;
    int index = 0;
    while(index < count) {
      if(p->id == id) return index;
      index++;
      p = p->getNext();
    }
    std::cout << "Erro, indice invalido: " << id << std::endl;
    exit(0);
  }

  /**
    * Seleciona um vértice baseado na posição dele dentro da lista encadeada
    * 
  */
  Node* Graph::getNodeByPosition(int id) {
    Node* p = node;
    for(int i = 0; i < id; i++) {
      p = p->getNext();
    }
    return p;
  }

  /**
   * @brief Algoritmo de dijkstra para buscar caminho minimo entre todos os vértices
   * @param origin
   * @param destination
  */
  std::list<int> Graph::dijkstra(int origin, int destination) {
    //Inicia todas as distâncias como 'Infinito'
    int* dist = new int[this->count];
    bool* visited = new bool[this->count];
    int* predecessor = new int[this->count];
    for(int i = 0; i < this->count; i++) {
      dist[i] = INT_MAX, visited[i] = false;
    }
    int nodeReference = getNodeReferenceIndex(origin);
    dist[nodeReference] = 0;
    std::priority_queue<std::pair<int, int>, 
    std::vector<std::pair<int, int>>, 
    std::greater<std::pair<int, int>>> priority;
    priority.push(std::make_pair(dist[nodeReference], origin));
    Node* node = nullptr;
    Edge* edge = nullptr;
    while(!priority.empty()) {
      std::pair<int, int> topInQueue = priority.top();
      int selected = topInQueue.second;
      priority.pop();
      int nodeReferenceIndex = getNodeReferenceIndex(selected);
      if(visited[nodeReferenceIndex] == false) {
        visited[nodeReferenceIndex] = true;
        Node* selectedNode = searchById(selected);
        if(selectedNode != nullptr) edge = selectedNode->getEdge();
        while(edge != nullptr) {
          float weight = 1;
          if(edgeType == EdgeType::PONDERED) weight = edge->getWeight();
          int ver = getNodeReferenceIndex(edge->getTo());
          if(dist[ver] > (dist[nodeReferenceIndex] + weight)) {
            predecessor[ver] = selected;
            dist[ver] = (dist[nodeReferenceIndex] + weight);
            priority.push(std::make_pair(dist[ver], edge->getTo()));
          }
          edge = edge->getNext();
        }
      }
    }
    std::cout << "dijkstra finished" << std::endl;
    delete [] dist;
    delete [] visited;
    return shortestPath(origin, destination, predecessor);
  }


  /**
   * @brief Função para gerar uma representação matricial do grafo para floyd
  */
  int** Graph::generateArrayRepresentation() {
    //Inicia as distâncias na matriz como 'Infinito'
    int** dists = new int*[count];
    for(int i = 0; i < count; i++) {
      dists[i] = new int[count];
      for(int j = 0; j < count; j++) {
        if(i == j) {
          dists[i][i] = 0;
          continue;
        }
        dists[i][j] = INT_MAX;
      }
    }

    //Trocando a distância de 'INFINITO' para o peso correspondente das arestas
    Node* p = nullptr;
    Edge* edge = nullptr;
    for(int i = 0; i < count; i++) {
      p = getNodeByPosition(i);
      edge = p->getEdge();
      while(edge != nullptr) {
        Node* s = searchById(edge->getTo());
        dists[i][s->getPosition()] = edge->getWeight();
        edge = edge->getNext();
      }
    }
    return dists;
  }

  std::list<int> Graph::floyd(int origin, int destination) {
    int** dists = generateArrayRepresentation();
    int** pred = initPred();
    int k, i, j;
    std::list<int> reList;
    for (k = 0; k < count; k++) {
      for (i = 0; i < count; i++) {
        for (j = 0; j < count; j++) {
          if(dists[i][k] == INT_MAX || dists[k][j] == INT_MAX) continue;
          if (dists[i][j] > (dists[i][k] + dists[k][j]))
          {
            dists[i][j] = dists[i][k] + dists[k][j];
            pred[i][j] = pred[k][j]; //Atualiza a posição do no antercessor dos nos na posição i e j
          }
        }
      }
    } //Calculando a distância de todas as posições
    Node* n = searchById(origin);
    Node* s = searchById(destination);
    int dist = dists[n->getPosition()][s->getPosition()];
    if(dist < INT_MAX) {
      std::cout << "A distancia e: " << dist << std::endl;
      floydOut(reList, pred, origin, destination);
    }
    else {
      std::cout << "A distancia e infinita, portanto os dois vertices nao sao conexos entre si" << std::endl << std::endl;
    }
    return reList;
  }


  //Função para printar Floyd
  void Graph::floydOut(std::list<int>& p, int** pred, int origin, int destination) {
    int predecessor;
    Node* n = searchById(origin);
    Node* b = searchById(destination);
    predecessor = pred[n->getPosition()][b->getPosition()];
    while(predecessor != n->getPosition()) {
      p.push_front(getNodeByPosition(predecessor)->id);
      predecessor = pred[n->getPosition()][predecessor];
    }
    int index = 0;
    p.push_back(destination);
    p.push_front(origin);
    std::cout << "O caminho e: ";
    for(int b : p) {
      if(index == p.size() - 1) {
        std::cout << b << "\n";
      }
      else {
        std::cout << b << "->";
      }
      index++;
    }
    std::cout << "Finalizado" << std::endl;
  }

  /**
   * @brief Função
   * 
   * @param from 
   * @param to
  */
  int** Graph::initPred() {
    int** temp = new int*[count];
    for(int i = 0; i < count; i++) {
      temp[i] = new int[count];
      for(int j = 0; j < count; j++) {
        temp[i][j] = i;
      }
    }

    return temp;
  }

  /**
   * @brief Algoritmo para gerar uma lista contendo o caminho entre a origem e o destino
   * 
   * @param origin 
   * @param destination
   * @param predecessors
  */
  std::list<int> Graph::shortestPath(int origin, int destination, int* predecessors) {
    std::list<int> path;
    //Adiciona o destino na frente da lista
    path.push_front(destination);
    int ver = getNodeReferenceIndex(destination);
    //Adiciona os antecessores de cada vertice na lista até 1 deles ser igual a origem
    while(predecessors[ver] != origin) {
      path.push_front(predecessors[ver]);
      ver = getNodeReferenceIndex(predecessors[ver]);
    }
    //Adiciona a origem na lista
    path.push_front(origin);
    delete [] predecessors;
    return path;
  }


  /**
   * @brief Algoritmo que gera um subgrafo vértice induzido do grafo original
   * 
   * @param subN
   */
  Graph* Graph::vertexInducedSubgraph(std::vector<int> subN) {
    std::sort(subN.begin(), subN.end());
    std::vector<Relation> relations;
    Graph* graph = new Graph(this->edgeType, this->graphType);
    for(int b : subN) {
      Node* k = searchById(b);
      if(k != nullptr) {
        Node* nNode = graph->instanceNewNode(k->id);
        nNode->setPosition(graph->getCount());
      }
      else {
        continue;
      }
    }
    for(int b : subN) {
      Node* searched = graph->searchById(b);
      for(int h : subN) {
        if(h != b && edgeExists(b, h)) {
          Edge* edge = searchById(b)->searchEdge(h);
          int weight = 0;
          if(edge != nullptr) weight = edge->getWeight();
          searched->makeRelationship(h, weight);
        }
      }
    }
    return graph;
  }

  /**
   * @brief Algoritmo para verificar a existência de uma aresta entre dois vértices
   * 
   * @param from 
   * @param to
   */
  bool Graph::edgeExists(int from, int to) {
    Node* k = searchById(from);
    if(k == nullptr) return false;
    if(k->searchEdge(to) != nullptr) return true;
    return false;
  }

  void Graph::kruskal(Graph* subGraph) {
    int* parent = new int[subGraph->getCount()];
    for(int i = 0; i < subGraph->getCount(); i++) {
      parent[i] = i;
    }
    std::vector<std::pair<int, std::pair<int, int>>> pairs = subGraph->getGraphInPairFormat();
    std::vector<std::pair<int, std::pair<int, int>>> pairsMst;
    std::cout << "Finish 2 part" << "\n";
    std::sort(pairs.begin(), pairs.end());
    for(auto b : pairs) {
      std::cout << "Weight: " << b.first << std::endl;
    }
    int i, uRep, vRep;
    for(int i = 0; i < pairs.size(); i++) {
      uRep = findSet(parent, subGraph->getNodeReferenceIndex(pairs[i].second.first));
      vRep = findSet(parent, subGraph->getNodeReferenceIndex(pairs[i].second.second));
      if(uRep != vRep) {
        pairsMst.push_back(pairs[i]);
        unionSet(parent, uRep, vRep);
      }
    }
    printKruskal(pairsMst, subGraph);
  }

  void Graph::printKruskal(std::vector<std::pair<int, std::pair<int, int>>> &edges, Graph* subGraph) {
    std::cout << "ARVORE GERADORA MINIMA VIA KRUSKAL\n";
    std::string data;
    int weight = 0;
    std::string realFilePath(outputFilePath.c_str());
    realFilePath.append("_kruskal.dot");
    std::ofstream oof(realFilePath, std::ios::out | std::ios::trunc);
    std::string connector;
    if(graphType == DIRECTED) {
      oof << "digraph {\n";
      connector.append("->");
    }
    else {
      oof << "graph {\n";
      connector.append("--");
    }
    for(int i = 0; i < edges.size(); i++) {
      if(edges[i].second.first == edges[i].second.second) {
        oof << " " << edges[i].second.first << std::endl;
      }
      else {
        oof << edges[i].second.first << connector << edges[i].second.second;
        oof << " [weight = " << edges[i].first << "]" << std::endl;
        weight += edges[i].first;
      }
    }
    Node* p = subGraph->getNode();
    while(p != nullptr) {
      oof << p->id << std::endl;
      p = p->getNext();
    }
    oof << "}\n";
    oof.close();
  }

  int Graph::witchSubTree(SubTree subTree[], int n) {
  
  }

  void Graph::joinSubTrees(SubTree subTree[], int u, int v) {
    int subU = witchSubTree(subTree, u);
    int subV = witchSubTree(subTree, v);
    if(subTree[subU].order < subTree[subV].order) {
      subTree[subU].father = subV;
    }
    else if(subTree[subU].order > subTree[subV].order) {
      subTree[subV].father = subU;
    }
    else {
      subTree[subV].father = subU;
      subTree[subU].order += subTree[subV].order;
    }
  }

  std::vector<Edge*> Graph::deepPathTree(int id) {
    // Nó do vértice raiz da árvore
    Node* vertex = searchById(id);
    // arestas de retorno
    std::vector<Edge*> returnEdges;

    // Se o vértice não existir, retornar a lista de arestas vazia
    if(vertex==nullptr) {
      return returnEdges;
    }

    std::string path(outputFilePath.c_str());
    path.append("deepPathTree.dot");
    std::cout << path << "\n";

    std::vector<Utils::WeightedDot> dots;
    // Vetor dos nós que irão compor um caminho:
    // Funcionamento: Se um vértice for visitado ele irá,
    // em um primeiro momento, entrar no vetor nodesInThePath,
    // que guarda todos os vértices que compõem o caminho entre
    // a raiz e o vértice que foi visitado.
    // Quando o laço de repetição da função assistente 
    // deepPathAssistent() se encerra, o id do último vetor
    // visitado é retirado do final do vetor.
    // O vetor é usado para descobrir todas as arestas de retorno:
    // se a aresta do último vértice visitado se conecta com algum
    // vértice do vetor, então esssa é uma aresta de retorno.
    std::vector<int> nodesInThePath;

    this->setAllNodesVisitedFalse();
    deepPathTreeAssistant(vertex, nodesInThePath, dots, returnEdges);

    dt->writeOnFile(path, dots, true);

    return returnEdges;
  }

  void Graph::deepPathTreeAssistant(Node* vertex, std::vector<int>& nodesInThePath, 
  std::vector<Utils::WeightedDot>& dots, std::vector<Edge*>& returnEdges) {
    // O vértice passado como parâmetro é visitado
    vertex->visited();
    // nodesInThePath recebe o id do vértice visitado para
    // colocar no caminho da árvore
    nodesInThePath.push_back(vertex->id);
    // Nó assistente para as operações
    Node* assistant = nullptr;
    // Aresta do vértice passado como parâmetro
    Edge* edge = vertex->getEdge();

    while(edge!=nullptr) {
      // o nó assistente irá receber o vértice apontado pela aresta
      assistant = searchById(edge->getTo());

      // Se o vértice foi visitado, será feita uma verificação
      // no vetor nodesInThePath para saber se algum vértice
      // no caminho da raíz até o último nó visitado corresponde
      // ao vértice assistente.
      // Caso o vértice não tenha sido visitado, será feita uma
      // chamada recursiva passando o vértice assistente como parâmetro
      if(assistant->beenVisited()) {
        for(auto i : nodesInThePath) {
          if(assistant->id == i) {
            // Impressão das arestas de retorno ( Usada em testes)
            //std::cout << vertex->id << " -> " << i << ", ";
            returnEdges.push_back(edge);
          }
        }
      }
      else {
        Utils::WeightedDot dot;
        dot.origin = vertex->id;
        dot.destination = assistant->id;
        dot.weight = edge->getWeight();
        dots.push_back(dot);

        deepPathTreeAssistant(assistant, nodesInThePath, dots, returnEdges);
      }

      edge = edge->getNext();
    }
    // o id do nó assistente é retirado do vetor nodesInThePath
    nodesInThePath.pop_back();
  }

  void Graph::algorithmPrim(Graph* subgraph) {
    
    int order = subgraph->getCount();

    std::vector<int> distance;
    distance.clear();

    //Vector que verifica se o nó já foi visitado;
    std::vector<bool> visited(order, false);

    //O primeiro inicializado com 0 e os demais com INF;
    distance.push_back(0);
    for(int i = 1;i < order;i++) distance.push_back(INF);

    //Vector com os pais de cada nó, -1 caso não tenha pai;
    std::vector<int> parent(order, -1);

    subgraph->setAllNodesVisitedFalse();

    Node* node = subgraph->getNode();
    for(int i = 0;i < order;i++) 
    {

      //Só marcar visitado caso tenha aresta, pq tava dando bug
      if(!node->getEdgeCount()){
        node->visited();
      } else {
        parent[node->getPosition()] = node->id;
      }
      
      //Percorrer cada aresta do nó
      Edge* edge = node->getEdge();
      int j = 0;
      while(j < node->getEdgeCount()) 
      {
        Node* v = subgraph->searchById(edge->getTo());
        int weigth = edge->getWeight();

        if(!(v->beenVisited()) && distance[v->getPosition()] > weigth) {
          distance[v->getPosition()] = weigth;
          //atualizando o pai de cada nó dada a posicao;
          parent[v->getPosition()] = node->id;
        }
        edge = edge->getNext();
        j++;
      }
      node = node->getNext();
    }
    //Usado para testar o pai de cada nó;
    for(int i = 1;i < order;i++) {
      std::cout << parent[i] << std::endl;
    }
    //printPrim(subgraph, parent);
  }


  void Graph::printPrim(Graph* subgraph, std::vector<int>& mgt) {
    int peso = 0;
    std::cout << "\nÁRVORE GERADORA MÍNIMA via Prim\n"
        << std::endl;
    std::cout << "graph {" << std::endl;
    for (int i = 0; i < subgraph->getCount(); i++)
    {
        if (mgt[i] != -1)
        {
            int destination = subgraph->getNodeByPosition(i)->id;
            if (mgt[i] == destination)
                std::cout << "  " << mgt[i] << std::endl;
            else
            {
                std::cout << "  " << mgt[i] << " -- " << destination;
                /*std::cout << " [label = " << subgraph->searchById(mgt[i])->searchEdge(destination)->getWeight() << "]" << std::endl;
                peso += subgraph->searchById(mgt[i])->searchEdge(destination)->getWeight();*/
            }
        }
    }
    std::cout << "}" << std::endl;
  }
  
}