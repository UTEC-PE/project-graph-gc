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
private:
  NodeSeq nodes;
  NodeIte ni;
  EdgeIte ei;

public:
  typedef Graph<Tr> self;
  typedef Node<self> node;
  typedef Edge<self> edge;
  typedef vector<node *> NodeSeq;
  typedef list<edge *> EdgeSeq;
  typedef typename Tr::N N;
  typedef typename Tr::E E;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;

  Graph();

  addVertex(node *vertex) { nodes.push_back(vertex); }
  addEdge(N v1, N v2, E weight, bool dir) {
    node *n1 = nullptr;
    node *n2 = nullptr;
    if (dir) {
      for (auto v : nodes) {
        if (v->data == v1) {
          n1 = v;
        } else if (v->data == v2) {
          n2 = v;
        }
        if (n1 && n2) {
          break;
        }
      }
    }
  }
};

typedef Graph<Traits> graph;

#endif
