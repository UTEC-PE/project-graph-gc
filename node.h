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

private:
  N data;
  double x;
  double y;

public:
  EdgeSeq edges;

  Node(N data, double x, double y) : data(data), x(x), y(y), edges() {}
  void addEdge(edge *e) { (this->edges).push_back(e); }
  bool is(int node_tag) { return ((this->data) == node_tag); }
  bool in(std::vector<Node<G> *> v) {
    return std::find(v.begin(), v.end(), this) != v.end();
  }
  N print() { return data; }
};

#endif
