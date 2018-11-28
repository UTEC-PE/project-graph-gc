#ifndef BELLMAN_H
#define BELLMAN_H

#include "graph.h"
#include <climits>
#include <iostream>
#include <map>
#include <stdexcept>

template <typename GH> class BellmanHelper {

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
  BellmanHelper(N start, const graph *G_) {
    this->G = G_;
    bool hasNegCycle = !(this->execute(start));
    if (hasNegCycle) {
      throw std::invalid_argument(
          "Graph has a negative cycle, Bellman Ford can't provide exact "
          "solution.");
    }
  }
  bool execute(N start) {
    this->initialize(start);
    for (int it = 0, iterations = G->getNumberOfNodes(); it < iterations;
         ++it) {
      for (auto const &v : G->nodes) {
        for (auto const &e : v.second->outEdges) {
          if (this->table[v.first].cost != INT_MAX) {
            this->relax(e->nodes[0]->tag, e->nodes[1]->tag, e->getWeight());
          }
        }
      }
    }

    for (auto const &v : G->nodes) {
      for (auto const &e : v.second->outEdges) {
        if (this->table[v.first].cost != INT_MAX) {
          if (this->table[e->nodes[1]->tag].cost >
              this->table[e->nodes[0]->tag].cost + e->getWeight()) {
            return false;
          }
        }
      }
    }
    return true;
  }
  void initialize(N start) {
    for (auto const &v : G->nodes) {
      this->table[v.first].cost = INT_MAX;
      this->table[v.first].parent = -1;
    }
    this->table[start].cost = 0;
  }

  bool relax(N nodeFrom, N nodeTo, E weight) {
    if (this->table[nodeTo].cost > (this->table[nodeFrom].cost + weight)) {
      this->table[nodeTo].cost = this->table[nodeFrom].cost + weight;
      this->table[nodeTo].parent = nodeFrom;
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
