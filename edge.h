#ifndef EDGE_H
#define EDGE_H

#include "graph.h"
#include <iostream>
#include <utility>

template <typename G> class Edge {

public:
  typedef typename G::E E;
  typedef typename G::node node;
  typedef typename G::N N;

  struct pairV {
    const node *start;
    const node *end;
  };

  const node *nodes[2];

private:
  E weight;
  bool dir;

public:
  Edge(E weight_, bool dir_, const node *v1, const node *v2)
      : weight(weight_), dir(dir_) {
    this->nodes[0] = v1;
    this->nodes[1] = v2;
  }
  inline E getWeight() const { return this->weight; }
  inline bool getDirection() const { return this->dir; }
  inline pairV getNodes() const { return this->nodes; }
  inline bool operator==(const Edge<G> &e) {
    return this->nodes[0]->tag == e.nodes[0]->tag &&
           this->nodes[1]->tag == e.nodes[1]->tag;
  }
  bool is(N vFrom, N vTo) const {
    return (this->nodes[0]->tag == vFrom && this->nodes[1]->tag == vTo);
  }

  friend std::ostream &operator<<(std::ostream &os, const Edge<G> &e) {
    os << "(edge: (" << e.nodes[0]->tag << ", " << e.nodes[1]->tag
       << "), w: " << e.weight << ')';
    return os;
  }
  ~Edge() {
    std::cout << "deleting " << *(this);
    this->nodes[0] = this->nodes[1] = nullptr;
    std::cout << "...    done.\n";
  }
};

#endif
