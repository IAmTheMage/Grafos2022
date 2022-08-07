#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"
#include "math.h"
#include "time.h"
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
    if(strcmp(args[4], "1") == 0) {
      edgeType = EdgeType::PONDERED;
    }
    else {
      edgeType = EdgeType::NONPONDERED;
    }
    if(strcmp(args[3], "1") == 0) {
      graphType = GraphType::DIRECTED;
    }
    else {
      graphType = GraphType::NONDIRECTED;
    }
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
      int numberOfNodes = std::atoi(b.c_str());
      std::vector<int> values;
      if(b.size() == 1) {
        while(getline(ip, b)) {
          std::string s;
          std::stringstream temp(b);
          if(edgeType == PONDERED) {
            while(getline(temp, s, ' ')) {
              values.push_back(std::atoi(s.c_str()));
            }
            Node* p = this->searchById(values[0]);
            if(p == nullptr) {
              p = instanceNewNode(values[0]);
              p->setPosition(this->count - 1);
            }
            Node* s = this->searchById(values[1]);
            if(s == nullptr) {
              s = instanceNewNode(values[1]);
              s->setPosition(this->count - 1);
            }
            p->makeRelationship(values[1], values[2]);
            
            if(graphType == GraphType::NONDIRECTED) {
              s->makeRelationship(values[0], values[2]);
              
            }
            values.clear();
          }
          else {
            while(getline(temp, s, ' ')) {
              values.push_back(std::atoi(s.c_str()));
            }
            Node* p = this->searchById(values[0]);
            if(p == nullptr) {
              p = instanceNewNode(values[0]);
              p->setPosition(this->count - 1);
            }
            p->makeRelationship(values[1], 1);
            Node* s = this->searchById(values[1]);
            if(s == nullptr) {
              s = instanceNewNode(values[1]);
              s->setPosition(this->count - 1);
            }
            if(graphType == GraphType::NONDIRECTED) {
              s->makeRelationship(values[0], 1);
            }
            values.clear();
          }
        }
        ip.close();
      }
      else {
        std::cout << "Clustering file" << std::endl;
        readClusteringFile(in);
      }
    }
    else {
      std::cout << "Is close" << "\n";
    }
    std::cout << "Grafo gerado" << std::endl;
  }

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

  std::vector<Node*> Graph::getGraphInVectorFormat() {
    std::vector<Node*> v;
    Node* p = node;
    while(p != nullptr) {
      if(!p->beenVisited()) 
      v.push_back(p);
      p = p->getNext();
    }
    std::sort(v.begin(), v.end(), [](Node* n, Node* b) {
      return n->getClusteringRelativeValue() < b->getClusteringRelativeValue();
    });
    return v;
  }

  void Graph::initClusteringData() {
    std::vector<Node*> viz = getGraphInVectorFormat();
    for(int i = 0; i < clusters.size(); i++) {
      clusters[i].ids.push_back(viz.back()->id);
      clusters[i].currentWeight += viz.back()->getWeight();
      searchById(viz.back()->id)->visited();
      viz.pop_back();
    }
  }

  void Graph::greedy() {
    std::cout << "Count is: " << count << std::endl;
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy");
    performance->startClock();
    initClusteringData();
    std::vector<Node*> viz = getGraphInVectorFormat();
    int interactions = 0;
    while(!viz.empty()) {
      for(int i = 0; i < clusters.size(); i++) {
        interactions++;
        if(viz.empty()) break;
        Node* b = searchById(clusters[i].ids[0]);
        float max = INT_MIN;
        int addedId;
        Edge* edge = b->getEdge();
        
        while(edge != nullptr) {
          if(max < edge->getWeight() && !(searchById(edge->getTo())->beenVisited())) {
            max = edge->getWeight();
            addedId = edge->getTo();
          }
          edge = edge->getNext();
        }
        if(max != INT_MIN) {
          Node* k = searchById(addedId);
          if(k != nullptr) {
            if(clusters[i].currentWeight + k->getWeight() < clusters[i].upperLimit) {
              k->visited();
              clusters[i].ids.push_back(addedId);
              clusters[i].currentWeight += k->getWeight();
              viz.erase(std::remove(viz.begin(), viz.end(), k), viz.end());
            }
          }
        }
      }
    }
    constructClusterSet();
    getClusteringInfo();
    performance->_end();
    
  }

  void Graph::randomGreedy() {
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy Random");
    performance->startClock();
    initClusteringData();
    std::vector<Node*> viz = getGraphInVectorFormat();
    int limit = 1;
    int close = 0;
    while(!viz.empty()) {
      srand(time(NULL));
      int randomIndex = rand() % (count % clusters.size());
      Node* tpNode = viz[randomIndex];
      float globalContributedWeight = -9999999.0f;
      int addId;
      for(int i = 0; i < clusters.size(); i++) {
        if(clusters[i].visited) continue;
        float contributedWeight = 0.0f;
        for(int ids : clusters[i].ids) {
          Node* temp = searchById(ids);
          Edge* edge = temp->searchEdge(tpNode->id);
          if(edge != nullptr) contributedWeight += edge->getWeight();
        }
        if(contributedWeight > globalContributedWeight && clusters[i].currentWeight + tpNode->getWeight() < clusters[i].upperLimit) {
          globalContributedWeight = contributedWeight;
          addId = i;
        } 
      }
      clusters[addId].ids.push_back(tpNode->id);
      clusters[addId].visited = true;
      clusters[addId].currentWeight += tpNode->getWeight();
      close++;
      if(close == clusters.size()) {
        for(int i = 0; i < clusters.size(); i++) {
          clusters[i].visited = false;
        }
        close = 0;
      }
      viz.erase(std::remove(viz.begin(), viz.end(), tpNode), viz.end());
    }
    constructClusterSet();
    getClusteringInfo();
    performance->_end();
  }

  void Graph::randomReactiveGreedy() {
    Utils::PerformanceMetrics* performance = new Utils::PerformanceMetrics();
    performance->setupTestSuit("Greedy Random Reactive");
    performance->startClock();
    initClusteringData();
    std::vector<Node*> viz = getGraphInVectorFormat();
    int limit = 1;
    int close = 0;
    while(!viz.empty()) {
      srand(time(NULL));
      int randomSeed = clusters.size() / 2;
      if(viz.size() < clusters.size() / 2) randomSeed = viz.size();
      int randomIndex = rand() % randomSeed;
      Node* tpNode = viz[randomIndex];
      int highPriorityCluster;
      float max = -999999.0f;
      for(int i = 0; i < clusters.size(); i++) {
        if(max < tpNode->getSpecificBiasValue(i) && 
        !(clusters[i].currentWeight + tpNode->getWeight() > clusters[i].upperLimit)
        ) {
          max = tpNode->getSpecificBiasValue(i);
          highPriorityCluster = i;
        }
      }
      clusters[highPriorityCluster].ids.push_back(tpNode->id);
      clusters[highPriorityCluster].currentWeight += tpNode->getWeight();
      Edge* edge = tpNode->getEdge();
      while(edge != nullptr) {
        searchById(edge->getTo())->updateSpecificBias(highPriorityCluster, edge->getWeight());
        edge = edge->getNext();
      }
      viz.erase(std::remove(viz.begin(), viz.end(), tpNode), viz.end());
    }
    constructClusterSet();
    getClusteringInfo();
    performance->_end();
  }

  void Graph::sortByClusterPreference(int clusterId, std::vector<Node*> viz) {
    std::sort(viz.begin(), viz.end(), [](Node* p, Node* b) {
      return p->getSpecificBiasValue(p->currentCluster) > b->getSpecificBiasValue(b->currentCluster);
    });
  }

  void Graph::getClusteringInfo() {
    float variation = 0;
    float totalWeight = 0;
    float median = 0;
    int ind = 1;
    for(Cluster cluster: clusters) {
      std::cout << "Cluster: " << ind << " has weight: " << cluster.currentWeight << " and has degree: " << cluster.totalWeight << std::endl;
      totalWeight += cluster.totalWeight;
      ind++;
    }
    median = totalWeight / clusters.size();
    for(Cluster cluster: clusters) {
      variation += (cluster.totalWeight - median) * (cluster.totalWeight - median);
    }
    variation /= (clusters.size() - 1);
    float standardDeviation = sqrt(variation);
    std::cout << "Cluster sum:" << totalWeight << std::endl;
    std::cout << "Cluster media: " << median << std::endl;
    std::cout << "Desvio padrao: " << standardDeviation << std::endl;
  }


  void Graph::constructClusterSet() {
    std::vector<int> reference;
    int index = 0;
    for(Cluster cluster : clusters) {
      cluster.totalWeight = 0;
      reference = cluster.ids;
      for(int id : cluster.ids) {
        Node* p = searchById(id);
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