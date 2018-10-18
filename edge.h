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
  typedef typename G::N N;

  E data;
  bool dir;

  node *nodes[2];

  Edge(E data, bool dir, node *v1, node *v2) : data(data), dir(dir) {
    (this->nodes)[0] = v1;
    (this->nodes)[1] = v2;
  }
  ~Edge() {

    std::cout << "Edge " << *this << " is being deleted...   ";
    nodes[0] = nodes[1] = nullptr;
    std::cout << "Done.\n";
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

  inline bool operator==(const Edge<G> edge) const {
    return ((this->nodes) == edge.nodes);
  }
  inline bool operator<(const Edge<G> edge) const {
    return this->data < edge.data;
  }

  E printWeight() { return data; };
  N printV1() { return (this->nodes)[0]->print(); };
  N printV2() { return (this->nodes)[1]->print(); };
  bool printDir() { return this->dir; };
  bool remove_if_has(N &v_data) {
    if (this->has(v_data)) {
      std::cout << "this edge: " << *this << "has " << v_data << std::endl;
      delete this;
      return true;
    }
    return false;
  }
  inline bool has(N &v_data) {
    return (this->nodes[0]->data == v_data || this->nodes[1]->data == v_data);
  }
};

#endif
