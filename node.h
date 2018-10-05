#ifndef NODE_H
#define NODE_H

template <typename G> class Node {

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

  void addEdge(edge e) { (this->edges).push_back(e); }
};

#endif
