#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>

#include "edge.h"
#include "node.h"

using namespace std;

class Traits {
public:
  typedef char *N;
  typedef int E;
};

template <typename Tr> class Graph {

  typedef Graph<Tr> self;
  typedef Node<self> node;
  typedef Edge<self> edge;
  typedef vector<node *> NodeSeq;
  typedef list<edge *> EdgeSeq;
  typedef typename Tr::N N;
  typedef typename Tr::E E;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;

private:
  NodeSeq nodes;
  NodeIte ni;
  EdgeIte ei;

public:
  Graph();

  void addVertex(double x, double y, N data) {
    node vertex(x, y, data);
    nodes.push_back(vertex);
  }
  void addEdge(N v1, N v2, E weight, bool dir) {
    node *vn1[2] = {nullptr, nullptr};
    node *vn2[2] = {nullptr, nullptr};
    for (auto const &v : this->nodes) {
      if (v->data == v1) {
        vn1[0] = v;
        vn2[1] = v;
      } else if (v->data == v2) {
        vn1[1] = v;
        vn2[0] = v;
      }
      if (vn1[0] && vn1[1]) {
        break;
      }
    }
    edge e1(weight, dir, vn1);
    edge e2(weight, dir, vn2);

    if (dir) {
      vn1[0]->addEdge(e1);
      vn1[1]->addEdge(e1);
    } else {
      vn1[0]->addEdge(e1);
      vn1[1]->addEdge(e2);
    }
  }
};

typedef Graph<Traits> graph;

#endif
