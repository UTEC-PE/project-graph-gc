#ifndef EDGE_H
#define EDGE_H

#include "node.h"

template <typename G> class Edge {
public:
  typedef typename G::E E;
  typedef typename G::node node;
  typedef typename G::N N;

private:
  E data;
  bool dir;

public:
  node *nodes[2];

  Edge(E data, bool dir, node *v1, node *v2) : data(data), dir(dir) {
    (this->nodes)[0] = v1;
    (this->nodes)[1] = v2;
  }
  E printWeight() {return data;};
  N printV1() {return (this->nodes)[0]->print();};
  N printV2() {return (this->nodes)[1]->print();};
  bool printDir() {return this->dir;};
};

#endif
