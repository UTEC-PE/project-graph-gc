#ifndef EDGE_H
#define EDGE_H

#include "node.h"

template <typename G> class Edge {

private:
  E data;
  bool dir;

public:
  typedef typename G::E E;
  typedef typename G::node node;

  node *nodes[2];

  Edge(E data, bool dir, node *nodes[2]) : data(data), dir(dir), nodes(nodes) {}
};

#endif
