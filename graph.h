#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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
  typedef typename Tr::N N;
  typedef typename Tr::E E;
  typedef Graph<Tr> self;
  typedef Node<self> node;
  typedef Edge<self> edge;
  typedef map<N, node *> NodeSeq;
  typedef list<edge *> EdgeSeq;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;

private:
  NodeSeq nodes;
  NodeIte ni;
  EdgeIte ei;
  bool directed;
  int counter;

public:
  /* ***** CONSTRUCTORS ***** */
  // Empty explicit initialization might be unnecessary

  Graph() : nodes(), counter(0){};
  Graph(int number_of_vertices) : nodes(), counter(0){};

  /* ***** MANIPULATION METHODS ***** */

  // Integer indexed method
  void addVertex(double x, double y) {
    (this->nodes)[this->counter] = new node(this->counter, x, y);
    (this->counter)++;
  }

  void addVertex(N data, double x, double y) {
    if ((this->nodes).find(data) == (this->nodes).end()) {
      (this->nodes)[data] = new node(data, x, y);
      (this->counter)++;
    } else {
      cout << "you're trying to override a node" << endl;
    }
  }

  void addEdge(N v1, N v2, E weight, bool dir) {
    node *vn1 = (this->nodes)[v1];
    node *vn2 = (this->nodes)[v2];

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
