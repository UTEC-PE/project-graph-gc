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
    std::cout << "Edge " << *this << " (address: " << this << ")"
              << " is being deleted...   ";

    (this->nodes)[0] = (this->nodes)[1] = nullptr;
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

  inline E getWeight() const { return data; };
  N printV1() const { return (this->nodes)[0]->print(); };
  N printV2() { return (this->nodes)[1]->print(); };
  inline bool getDir() const { return this->dir; };
  bool removeIfHas(N &v_data) {
    if (this->has(v_data)) {
      delete this;
      return true;
    }
    return false;
  }
  bool removeIfGoesTo(N &v_data) {
    if (this->nodes[1]->is(v_data)) {
      delete this;
      return true;
    }
    return false;
  }

  bool removeIfSame(N &nodeFrom, N &nodeTo) {
    if (this->is(nodeFrom, nodeTo)) {
      delete this;
      return true;
    }
    return false;
  }
  inline bool is(N &nodeFrom, N &nodeTo) const {
    return (this->nodes[0]->data == nodeFrom && this->nodes[1]->data == nodeTo);
  }
  inline bool has(N &v_data) const {
    return (this->nodes[0]->data == v_data || this->nodes[1]->data == v_data);
  }
  inline bool hasNULL() const {
    return (this->nodes[0] == nullptr || this->nodes[1] == nullptr);
  }
};

#endif
