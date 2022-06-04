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
    Edge* p = NULL;
    while(t != NULL) {
      p = t;
      t = t->getNext();
    }
    return p;
  }

  void Node::makeRelationship(int to, int weight = 0) {
    if(edgeCount == 0) {
      this->edge = new Edge(this->id, to, weight);
    }
    else {
      Edge* last = getLastEdgeBeforeNull();
      last->setNext(new Edge(this->id, to, weight));
    }
    edgeCount++;
  }
}