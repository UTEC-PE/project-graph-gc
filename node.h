#ifndef NODE_H
#define NODE_H

template <typename G> class Node {

private:
  N data;
  double x;
  double y;

public:
  typedef typename G::N N;
  typedef typename G::E E;
  typedef typename G::edge edge;
  typedef typename G::EdgeSeq EdgeSeq;

  EdgeSeq edges;

  Node(N data, double x, double y, EdgeSeq edges)
      : data(data), x(x), y(y), edges(edges) {}
};

#endif
