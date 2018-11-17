#ifndef NODE_H
#define NODE_H
#include "graph.h"
#include <iostream>
#include <memory>
#include <vector>

template <typename G> class Node {

public:
  typedef typename G::N N;
  typedef typename G::E E;
  typedef typename G::edge edge;
  typedef typename G::EdgeSeq EdgeSeq;
  typedef typename G::wEdgeSeq wEdgeSeq;

  struct pairN {
    double x;
    double y;
  };

  N tag;

  pairN coordinates;
  wEdgeSeq inEdges;
  EdgeSeq outEdges;

  Node(N tag_, double x, double y) : tag(tag_) {
    this->coordinates = pairN{x, y};
  }
  inline pairN getCoordinates() const { return this->coordinates; }
  inline N getTag() const { return this->tag; }
  void addEdge(std::shared_ptr<edge> &e) {
    (this->outEdges).push_back(std::move(e));
  }
  void addEdge(std::weak_ptr<edge> &e) {
    (this->inEdges).push_back(std::move(e));
  }
};

#endif
