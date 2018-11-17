#ifndef EDGE_H
#define EDGE_H

#include "graph.h"
#include <iostream>

template <typename G> class Edge {

public:
  typedef typename G::E E;
  typedef typename G::node node;
  typedef typename G::N N;

  struct pairV {
    const node *start;
    const node *end;
  };

private:
  E weight;
  bool dir;
  const node *nodes[2];

public:
  Edge(E weight_, bool dir_, const node *v1, const node *v2)
      : weight(weight_), dir(dir_) {
    this->nodes[0] = v1;
    this->nodes[1] = v2;
  }
  inline E getWeight() const { return this->weight; }
  inline bool getDirection() const { return this->dir; }
  inline pairV getNodes() const { return this->nodes; }

  friend std::ostream &operator<<(std::ostream &os, const Edge<G> &e) {
    os << "(edge: (" << e.nodes[0]->tag << ", " << e.nodes[1]->tag
       << "), w: " << e.weight << ')';
    return os;
  }
};

#endif
