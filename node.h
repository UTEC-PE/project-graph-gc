#ifndef NODE_H
#define NODE_H
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

template <typename G> class Node {

public:
  typedef typename G::N N;
  typedef typename G::E E;
  typedef typename G::edge edge;
  typedef typename G::EdgeSeq EdgeSeq;
  typedef typename G::wEdgeSeq wEdgeSeq;

  N data;
  double x;
  double y;

  wEdgeSeq inEdges;
  EdgeSeq outEdges;

  Node(N data, double x, double y)
      : data(data), x(x), y(y), outEdges(), inEdges() {}
  Node(Node<G> *&v) : inEdges(), outEdges() {
    this->data = v->data;
    this->x = v->x;
    this->y = v->y;
  }
  ~Node() {
    std::cout << "Node " << (this->data) << " is being deleted...   ";
    (this->inEdges).clear();
    (this->outEdges).clear();
    std::cout << "Done.\n";
  }
  void addEdge(std::shared_ptr<edge> &e) {
    if (this->is(e->nodes[0])) {
      (this->outEdges).push_back(e);
    } else {
      (this->inEdges).push_back(e);
    }
  }
  void addEdge(Node<G> *&v, E data, bool dir) {
    (this->outEdges).push_back(std::make_shared<edge>(data, dir, this, v));
  }

  bool findEdge(N nodeFrom, N nodeTo, edge *&edgeRef) const {

    // TODO: custom comparator to use std::find with 2 N nodes_values (aka int),
    // instead of linear search.
    if (nodeFrom == this->data) {
      this->findOutEdge(nodeFrom, nodeTo, edgeRef);
    } else {
      this->findInEdge(nodeFrom, nodeTo, edgeRef);
    }
    edgeRef = nullptr;
    return false;
  }
  bool findOutEdge(N &nodeFrom, N &nodeTo, edge *&edgeRef) const {
    for (const auto &e : this->outEdges) {
      if (e->is(nodeFrom, nodeTo)) {
        edgeRef = e;
        return true;
      }
    }
  }

  bool findInEdge(N nodeFrom, N nodeTo, edge *&edgeRef) const {
    for (const auto &e : this->inEdges) {
      if (e->is(nodeTo, nodeFrom)) {
        edgeRef = e;
        return true;
      }
    }
  }

  inline bool is(int node_tag) const { return ((this->data) == node_tag); }

  inline bool is(const Node<G> &v) const { return ((this->data) == v.data); }

  bool in(const std::vector<Node<G> *> &v) const {
    return std::find(v.begin(), v.end(), this) != v.end();
  }
  inline N getData() const { return data; }
  inline N print() const { return data; }
  inline int numberOfEdges() const {
    return this->inEdges.size() + this->outEdges.size();
  }
  friend std::ostream &operator<<(std::ostream &os, const Node<G> &v) {
    os << "v" << v.getData() << " ($" << &v << ", " << v.numberOfEdges()
       << "e)";
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
