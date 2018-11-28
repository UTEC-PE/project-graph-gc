#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include "priority_queue.h"
#include <climits>
#include <map>

template <typename GH> struct path {

  typedef typename GH::N N;
  typedef typename GH::E E;
  typedef typename GH::node node;
  typedef typename GH::self graph;
  typedef typename GH::edge edge;
};

template <typename GH> class DijkstraHelper {

  typedef typename GH::N N;
  typedef typename GH::E E;
  typedef typename GH::node node;
  typedef typename GH::edge edge;
  typedef typename GH::self graph;

  struct info {
    int cost;
    N parent;
  };

private:
  const graph *G;
  std::map<N, info> table;

public:
  DijkstraHelper(N start, const graph *G_) {

    this->G = G_;

    this->execute(start);
  }
  void execute(N start) {
    this->initialize(start);
    PriorityQueue<GH> Q(G->nodes, start);
    N u;
    while (Q.heap_size != 0) {
      u = Q.extractMin();
      if (this->table[u].cost != INT_MAX) {
        for (auto const &e : G->nodes.at(u)->outEdges) {
          this->relax(u, e->nodes[1]->tag, e->getWeight(), Q);
        }
      }
    }
  }
  void initialize(N start) {
    for (auto const &v : G->nodes) {
      this->table[v.first].cost = INT_MAX;
      this->table[v.first].parent = -1;
    }
    this->table[start].cost = 0;
  }

  bool relax(N nodeFrom, N nodeTo, E weight, PriorityQueue<GH> &Q) {
    if (this->table[nodeTo].cost > (this->table[nodeFrom].cost + weight)) {
      this->table[nodeTo].cost = this->table[nodeFrom].cost + weight;
      this->table[nodeTo].parent = nodeFrom;
      Q.updateWeight(nodeTo, this->table[nodeFrom].cost + weight);
      return true;
    }
    return false;
  }

  void printTable() const {
    std::cout << '\n';

    for (auto const &v : this->table) {
      std::cout << v.first << " | " << v.second.cost << " | " << v.second.parent
                << "\n";
    }
  }

  const std::map<N, info> &getTable() const { return this->table; }
};

#endif
