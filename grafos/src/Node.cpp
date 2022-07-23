#include "../include/Node.h"

namespace Graph {
  Node::Node() {
    edge = NULL;
    next = NULL;
  }

  Node::~Node() {

  }

  int Node::getNumberOfNodes() {
    Node* t = this;
    int i = 0;
    while(t != NULL) {
      i++;
      t = t->getNext();
    }
    return i;
  }

  Node* Node::getLastBeforeNull() {
    Node* t = this;
    Node* p;
    while(t != NULL) {
      p = t;
      t = t->getNext();
    }
    return p;
  }

  Node* Node::getNext() {
    return this->next;
  }

  Edge* Node::getLastEdgeBeforeNull() {
    Edge* t = edge;
    for(int i = 0; i < edgeCount - 1; i++) {
      t = t->getNext();
    }
    return t;
  }

  void Node::makeRelationship(int to, int weight = 0) {
    if(edgeCount == 0) {
//      std::cout << "ID: " << this->id << " TO: " << to << " Weight: " << weight << std::endl;
      this->edge = new Edge(this->id, to, weight);
    }
    else {
      Edge* last = getLastEdgeBeforeNull();
//      std::cout << "ID: " << this->id << " TO: " << to << " Weight: " << weight << std::endl;
      last->setNext(new Edge(this->id, to, weight));
    }
    edgeCount++;
  }

}