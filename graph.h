#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
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

  // Copy constructor to 1) copy vertices, edges or entire graph
  Graph(Graph<Tr> *g, char type_of_copy_constructor) {
    switch (type_of_copy_constructor) {
    case 'v':
      this->nodes = g->vertices();
      this->directed = g->isDirected();
      this->counter = g->lastNodeTag();
      break;
    case 'e':
      // not sure if this is useful
      break;
    default:
      this->nodes = g->adjacencylist();
      this->directed = g->isDirected();
      this->counter = g->lastNodeTag();
      break;
    }
  }
  Graph(int number_of_vertices, bool directed)
      : nodes(), directed(directed), counter(0){};

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
  std::map<int, node *> vertices() {
    std::map<int, node *> vs;
    for (auto &v : this->nodes) {
      vs[v.second->data] = new node(v.second->data, v.second->x, v.second->y);
    }
    return vs;
  }
  std::map<int, node *> adjacencylist() {
    // hope this doesn't pass the adjacency list by reference
    return this->nodes;
  }
  bool isDirected() { return this->directed; }
  int lastNodeTag() { return this->counter; }

  /* ***** ALGORITHMS  ***** */

  void dfs(node *v = nullptr) {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (this->nodes)[0];
    }

    self A(this, 'v');

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
    self A(this, 'v');
    DisjointSet<self> DS;
    std::set<edge, non_dec_unique<edge>> GE;

    for (auto &v : nodes) {
      DS.makeSet((v.second)->data, (v.second)->x, (v.second)->y);
      for (auto e : (v.second)->edges) {
        if (e->nodes)
          GE.insert(*e);
      }
    }

    typename std::set<edge>::iterator it;
    for (it = GE.begin(); it != GE.end(); ++it) {

      std::cout << *it << '\n';
    }
    std::cout << '\n' << "finish\n";

    for (auto &e : GE) {
      node *parent1 = findSet(e->nodes[0]);
      node *parent2 = findSet(e->nodes[1]);

      if (parent1 != parent2) {
        A[(e->nodes[0])->data].addEdge(e);
        DS.unionSet(parent1, parent2);
      }
    }
    return A;
  }
};

typedef Graph<Traits> graph;

#endif
