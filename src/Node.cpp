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
}