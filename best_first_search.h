#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "graph.h"
#include "priority_queue.h"
#include <iostream>
#include <unordered_map>

template <typename GH> class BFSHelper {

  typedef typename GH::N N;
  typedef typename GH::E E;
  typedef typename GH::node node;
  typedef typename GH::edge edge;
  typedef typename GH::self graph;

  struct info {
    int cost;
    N parent;
    info(int cost_, N parent_) : cost(cost_), parent(parent_){};
  };

private:
  const graph *G;
  std::vector<std::pair<N, info>> output;
  N start;
  N goal;

public:
  BFSHelper(N start_, N goal_, const graph *G_) {
    this->start = start_;
    this->goal = goal_;

    this->G = G_;

    if (!this->execute(start_, goal_)) {
      std::cout << "couldn't reach destination\n";
    }
  }

  bool execute(N start, N goal) {
    PriorityQueue<graph> Q(G->nodes, start);
    N u;
    N temp;
    output.push_back(std::make_pair(start, info(0, -1)));
    while (Q.heap_size != 0) {
      u = Q.extractMin();
      if (u != start) {
        output.push_back(std::make_pair(u, info(G->getWeight(temp, u), temp)));
      }
      temp = u;
      if (u == this->goal) {
        return true;
      }

      for (auto &e : G->nodes.at(u)->outEdges) {
        if (Q.has(e->nodes[1]->tag) &&
            e->getWeight() < Q.weight(e->nodes[1]->tag)) {
          Q.updateWeight(e->nodes[1]->tag, e->getWeight());
        }
      }
    }
    return false;
  }

  void initialize(N start) {
    for (auto const &v : G->nodes) {
      this->table[v.first].cost = INT_MAX;
      this->table[v.first].parent = -1;
    }
    this->table[start].cost = 0;
  }

  void printTable() const {
    std::cout << '\n';
    for (auto const &v : this->output) {
      std::cout << v.first << " | " << v.second.cost << " | " << v.second.parent
                << "\n";
    }
  }

  const std::vector<std::pair<N, info>> &getTable() const {
    return this->table;
  }
};

#endif
