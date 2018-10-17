#ifndef EDGE_H
#define EDGE_H

#include "graph.h"
#include "node.h"
#include <iostream>
#include <ostream>

template <typename G> class Edge {
public:
  typedef typename G::E E;
  typedef typename G::node node;

  E data;
  bool dir;

  node *nodes[2];

  Edge(E data, bool dir, node *v1, node *v2) : data(data), dir(dir) {
    (this->nodes)[0] = v1;
    (this->nodes)[1] = v2;
  }
  friend std::ostream &operator<<(std::ostream &os, const Edge<G> &e) {

    if (e.dir) {
      os << "(edge: (" << e.nodes[0]->data << ", " << e.nodes[1]->data
         << "), w: " << e.data << ')';
    } else {

      os << "(edge: {" << e.nodes[0]->data << ", " << e.nodes[1]->data
         << "}, w: " << e.data << ')';
    }
    return os;
  }

  bool operator==(const Edge<G> edge) const {
    return ((this->nodes) == edge.nodes);
  }
  inline bool operator<(const Edge<G> edge) const {
    return this->data < edge.data;
  }
};

#endif
