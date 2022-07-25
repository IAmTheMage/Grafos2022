#include "../include/Graph.h" 
#include "string.h"
#include "vector"
#include "algorithm"
#include "config.h"

#ifdef LINUX
#include "sys/types.h"
#include "sys/sysinfo.h"
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
      ip.close();
    }
    else {
      std::cout << "Is close" << "\n";
    }
  }

  /**
   * @brief Função para gerar a representação em .dot do grafo
   * 
   */
  void Graph::generateGraphVizRepresentation() {
    std::string filePath(ROOT_DIR);
    filePath.append("graph.dot");
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
        std::cout << "Ja processado: " << from << ":" << to << std::endl;
        return true;
      }
    }
    return false;
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
    std::string path(ROOT_DIR);
    std::cout << path << "\n";
    path.append("indirectTransitiveClosure.dot");
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
    if(s->getEdgeCount() == 0 || clustering == 0) return 0;
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
    for(auto pair : pairs) {
      std::cout << pair.second.first << ":" << pair.second.second << std::endl;
    }
    std::sort(pairs.begin(), pairs.end());
    int i, uRep, vRep;
    for(int i = 0; i < pairs.size(); i++) {
      uRep = findSet(parent, pairs[i].second.first);
      vRep = findSet(parent, pairs[i].second.second);
      if(uRep != vRep) {
        pairsMst.push_back(pairs[i]);
        unionSet(parent, uRep, vRep);
      }
    }
    std::cout << "EDGES: " << pairsMst.size() << std::endl;
    for(auto kp : pairsMst) {
      std::cout << kp.second.first << "->" << kp.second.second << std::endl;
    }
    printKruskal(pairsMst);
  }

  void Graph::printKruskal(std::vector<std::pair<int, std::pair<int, int>>> &edges) {
    std::cout << "ARVORE GERADORA MINIMA VIA KRUSKAL\n";
    std::string data;
    int weight = 0;
    std::string realFilePath(ROOT_DIR);
    realFilePath.append("kruskal.dot");
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
    oof << "}\n";
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