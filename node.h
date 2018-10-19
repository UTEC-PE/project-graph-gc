#ifndef NODE_H
#define NODE_H
#include <algorithm>
#include <iostream>
#include <sstream>
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
  Node(Node<G> *&v) {
    this->data = v->data;
    this->x = v->x;
    this->y = v->y;
  }
  ~Node() {
    std::cout << "Node " << (this->data) << " is being deleted...   ";
    (this->edges).clear();
    std::cout << "Done.\n";
  }
  void addEdge(edge *e) {
    if ((e->nodes[0])->data == this->data) {
      (this->edges).push_front(e);
    } else {
      (this->edges).push_back(e);
    }
  }
  void addEdge(Node<G> *v, E data, bool dir) {
    (this->edges).push_front(new edge(data, dir, this, v));
  }
  inline bool is(int node_tag) { return ((this->data) == node_tag); }
  inline bool is(const Node<G> &v) { return ((this->data) == v.data); }

  bool in(std::vector<Node<G> *> v) {
    return std::find(v.begin(), v.end(), this) != v.end();
  }
  N print() { return data; }

  friend std::ostream &operator<<(std::ostream &os, const Node<G> &v) {
    os << v.data;
    return os;
  }
  inline bool operator==(const Node<G> node) const {
    return this->data == node.data;
  }
  inline bool operator<(const Node<G> node) const {
    return this->data < node.data;
  }
};

#endif
