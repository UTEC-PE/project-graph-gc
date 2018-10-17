#ifndef NODE_H
#define NODE_H
#include <algorithm>
#include <iostream>
#include <vector>

template <typename G> class Node {

public:
  typedef typename G::N N;
  typedef typename G::E E;
  typedef typename G::edge edge;
  typedef typename G::EdgeSeq EdgeSeq;

  N data;
  double x;
  double y;

  EdgeSeq edges;

  Node(N data, double x, double y) : data(data), x(x), y(y), edges() {}
  void addEdge(edge *e) {
    if ((e->nodes[0])->data == this->data) {
      (this->edges).push_front(e);
    } else {
      (this->edges).push_back(e);
    }
  }
  bool is(int node_tag) { return ((this->data) == node_tag); }
  bool in(std::vector<Node<G> *> v) {
    return std::find(v.begin(), v.end(), this) != v.end();
  }
  N print() { return data; }

  friend std::ostream &operator<<(std::ostream &os, const Node<G> &v) {
    os << v.data;
    return os;
  }
  bool operator<(const Node<G> node) const { return this->data < node.data; }
};

#endif
