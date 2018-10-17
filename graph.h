#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <stack>
#include <vector>

#include "data_structures.h"
#include "edge.h"
#include "node.h"

using namespace std;

class Traits {
public:
  // typedef char *N;
  typedef int N;
  typedef int E;
};

template <typename Tr> class Graph {

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

private:
  NodeSeq nodes;
  NodeIte ni;
  EdgeIte ei;
  bool i_indexed;
  int counter;
  int graph_size;
  bool directed;

public:
  /* ***** CONSTRUCTORS ***** */
  // Empty explicit initialization might be unnecessary

  Graph(bool indexation = 1)
      : nodes(), i_indexed(indexation), counter(0), graph_size(0){};
  Graph(int number_of_vertices, bool indexation = 1)
      : nodes(), i_indexed(indexation), counter(0),
        graph_size(number_of_vertices) {
    (this->nodes).resize(number_of_vertices);
  }

  /* ***** MANIPULATION METHODS ***** */

  // Integer indexed method
  void addVertex(double x, double y) {
    if (this->counter < this->graph_size) {
      (this->nodes)[this->counter] = new node(this->counter, x, y);
    } else {
      (this->nodes).push_back(new node(this->counter, x, y));
      (this->graph_size)++;
    }
    (this->counter)++;
  }

  void addEdge(N v1, N v2, E weight, bool dir) {
    node *vn1 = nullptr;
    node *vn2 = nullptr;

    if (this->i_indexed) {
      vn1 = (this->nodes)[v1];
      vn2 = (this->nodes)[v2];
    } else {
      for (auto const &v : this->nodes) {
        if (v->is(v1)) {
          vn1 = v;
        } else if (v->is(v2)) {
          vn2 = v;
        }
        if (vn1 && vn1) {
          break;
        }
      }
    }

    edge *e1 = new edge(weight, dir, vn1, vn2);

    if (dir) {
      vn1->addEdge(e1);
      vn2->addEdge(e1);
    } else {
      edge *e2 = new edge(weight, dir, vn2, vn1);
      vn1->addEdge(e1);
      vn2->addEdge(e2);
    }
  }
  void removeVertex() {}
  void removeEdge() {}

  /* ***** UTILITY METHODS ***** */

  void print() {}

  /* ***** ALGORITHMS  ***** */

  void dfs(node *v = nullptr) {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (this->nodes)[0];
    }

    std::vector<node *> output;
    std::vector<node *> visited;
    std::stack<node *> nodes_stack;
    nodes_stack.push(v);

    while (!(nodes_stack.empty())) {
      v = nodes_stack.top();
      nodes_stack.pop();

      if (!(v->in(visited))) {
        std::cout << v->print() << " ";
        visited.push_back(v);
      }

      for (const auto &edge : v->edges) {
        if (edge->nodes[0] == v && !(v->in(visited))) {
          nodes_stack.push(edge->nodes[1]);
        }
      }
    }
  }
  void bfs() {}

  void prim() {}
  self *kruskal() {
    self A();
    DisjointSet<N> DS();
    std::set<edge, non_dec<N>> GE;

    for (auto v : nodes) {
      DS.makeSet(v.data, v.x, v.y);
      for (auto e : v.edges) {
        GE.push_back(e);
      }
    }

    std::set<edge>::iterator it;
    for (it = GE.begin(); it != GE.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
};

typedef Graph<Traits> graph;

#endif
