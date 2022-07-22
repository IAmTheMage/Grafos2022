#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "stack" // revisar
#include "algorithm"
#include "config.h"

#ifdef LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif



namespace Graph {
  Graph::Graph(char** args) {
    dt = new Utils::Dot();
    std::cout << "Instance new graph" << std::endl;
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
    std::string out(args[2]);
    setFiles(in, out);
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

  void Graph::setFiles(std::string in, std::string out) {
    std::string path(ROOT_DIR);
    path.append(in);
    std::fstream ip(path, std::ios::in);
    if(ip.is_open()) {
      std::string b;
      std::getline(ip, b);
      int numberOfNodes = std::atoi(b.c_str());
      std::vector<int> values;
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
      std::fstream of;
      std::string path(ROOT_DIR);
      path.append("graph.dot");
      of.open(path, std::ios::trunc | std::ios::out);
      Utils::Dot* dot = new Utils::Dot();
      //of << dot->generateDotRepresentation(generateDotTypeVector());
      of.close();
    }
    else {
      std::cout << "Is close" << "\n";
    }
  }

  void Graph::generateGraphVizRepresentation() {
    std::string filePath(ROOT_DIR);
    filePath.append("graph.dot");
    std::vector<Utils::WeightedDot> dots;
    Node* p = node;
    Edge* edge = nullptr;
    while(p != nullptr) {
      edge = p->getEdge();
      while(edge != nullptr) {
        Utils::WeightedDot dot;
        dot.origin = p->id;
        dot.destination = edge->getTo();
        dot.weight = edge->getWeight();
        dots.push_back(dot);
        edge = edge->getNext();
      }
      p = p->getNext();
    }
    dt->writeOnFile(filePath, dots, true);
  }

  void Graph::directTransitiveClosure(int id) {
    Utils::PerformanceMetrics* metrics = new Utils::PerformanceMetrics();
    metrics->setupOutputFile("metrics.txt");
    Node* n = searchById(id);
    std::string path(ROOT_DIR);
    #ifdef LINUX
    struct sysinfo memInfo;
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;
    metrics->setMemoryUsed(physMemUsed);
    #endif
    path.append("out.dot");
    std::cout << path << "\n";
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

  Utils::DotType Graph::getAllNodesConnected(int id) {
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
  std::vector<int> Graph::indirectTransitiveClosure(int id) {
    // vértice o qual se procura o seu fecho transitivo indireto
    Node* target = searchById(id);

    // Se o vértice não existir, retornar o vetor vazio
    std::vector<int> indirectClosure;
    if(target == nullptr) {
      std::cout << "O vértice " << id << " não existe!\n";
      return indirectClosure;
    }

    // Primeiro nó do grafo
    Node* vertex = this->node;
    // Nó assistente para as operações
    Node* assistant = nullptr;
    Edge* edge = nullptr;
    
    //std::cout << "Fecho transitivo indireto: ";

    // O algoritmo funciona da sequinte forma: 
    // Todos os vértices do grafo farão o caminhamento em 
    // profundidade, marcando todos os vértices ao seu alcance
    // como visitados. Se o vértice alvo for visitado e não
    // for o próprio vértice alvo, então esse vértice faz parte
    // do fecho transitivo indireto (entra para o vetor indirectClosure)
    while(vertex!=nullptr) {
      // Todos os vértices do grafo serão marcados como não visitados
      // a cada vez que um novo vértice realizar o caminhamento em profundidade
      this->setAllNodesVisitedFalse();
      deepPath(vertex);

      if(target->beenVisited() && (vertex->id!=target->id)) {
        indirectClosure.push_back(vertex->id);
        //std::cout << vertex->id << " ";
      }
      
      // próximo vértice do grafo
      vertex = vertex->getNext();
    }

    return indirectClosure;
  }

  void Graph::setAllNodesVisitedFalse() {
    // "node" recebe o primeiro vértice do grafo
    Node* node = this->node;
    // enquanto todos os vértices não forem visitados, marcar o vértice/nó atual
    // como não visitado 
    while(node!=nullptr) {
      node->setVisitFalse();
      node = node->getNext();
    }
  }

  // caminhamento em profundidade
  void Graph::deepPath(Node* node) {
    // Nó assistente para as operações
    Node* assistant = nullptr;

    // nó node é marcado como visitado
    node->visited();

    // primeira aresta do nó "node" passado como parâmetro
    Edge *edge = node->getEdge();

    // Enquanto todas as arestas do vértice "node" não forem usadas/"visitadas",
    // Realizar o caminhamento em profundidade do nó destino
    while(edge != nullptr) {
      assistant = this->searchById(edge->getTo());
      if(!assistant->beenVisited()) {
        this->deepPath(assistant);
      }

      // próxima aresta do nó node
      edge = edge->getNext();
    }
  }

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

  float Graph::clusteringCoeficient(int id) {
    Node* s = searchById(id);
    Edge* edge = s->getEdge();
    int* connected = new int[s->getEdgeCount()];
    int clustering = 0;
    int index = 0;
    while(edge != nullptr) {
      connected[index] = edge->getTo();
      index++;
      edge = edge->getNext();
    }
    for(int i = 0; i < index; i++) {
      clustering += neighborsConnected(connected[i], connected, index);
    }
    if(s->getEdgeCount() == 0) return 0;
    float clusteringRealValue = (float)((float)clustering/s->getEdgeCount());
    return clusteringRealValue;
  }

  float Graph::clusteringGlobalCoeficient() {
    float sum = 0.0;
    Node* p = this->node;
    while(p != nullptr) {
      sum += clusteringCoeficient(p->id);
      p = p->getNext();
    }
    return sum;
  }

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

  Node* Graph::getNodeByPosition(int id) {
    Node* p = node;
    for(int i = 0; i < id; i++) {
      p = p->getNext();
    }
    return p;
  }

  std::list<int> Graph::dijkstra(int origin, int destination) {
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
          int weight = 1;
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
    return shortestPath(origin, destination, predecessor);
  }

  int** Graph::generateArrayRepresentation() {
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
    for(int i = 0; i < count; i++) {
      for(int j = 0; j < count; j++) {
        if(dists[i][j] == INT_MAX) {
          std::cout << "I" << " ";
          continue;
        }
        else {
          std::cout << dists[i][j] << " ";
        }
      }
      std::cout << std::endl;
    }
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

  std::list<int> Graph::shortestPath(int origin, int destination, int* predecessors) {
    std::list<int> path;
    path.push_front(destination);
    int ver = getNodeReferenceIndex(destination);
    while(predecessors[ver] != origin) {
      path.push_front(predecessors[ver]);
      ver = getNodeReferenceIndex(predecessors[ver]);
    }
    path.push_front(origin);
    return path;
  }

  // i) Árvore dada pelo caminhamento em profundidade:


  std::vector<Edge*> Graph::deepPathTree(int id) {
    // Nó do vértice raiz da árvore
    Node* vertex = searchById(id);
    // arestas de retorno
    std::vector<Edge*> returnEdges;

    // Se o vértice não existir, retornar a lista de arestas vazia
    if(vertex==nullptr) {
      return returnEdges;
    }

    std::string path(ROOT_DIR);
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

  void Graph::deepPathTreeAssistant(Node* vertex, std::vector<int>& nodesInThePath, std::vector<Utils::WeightedDot>& dots, std::vector<Edge*>& returnEdges) {
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
}