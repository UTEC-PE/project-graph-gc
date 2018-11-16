#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "graph.h"
#include <iostream>
#include <limits.h>
#include <map>
#include <utility>

template <typename D> struct non_dec_unique {
  inline bool operator()(const D &x1, const D &x2) const {
    return !(x1 == x2) && (x1 < x2);
  }
};

template <typename D> class PriorityQueue {
public:
  typedef typename D::N N;
  typedef typename D::E E;
  typedef typename D::edge edge;
  typedef typename D::node node;

  std::pair<int, node *> *A;
  int length;
  int heap_size;
  const int INF = INT_MAX;

  // CONSTRUCTORS

  PriorityQueue(std::map<N, node *> nodes) {
    (this->length) = 0;
    int size = nodes.size();
    this->A = new std::pair<int, node *>[size];
    for (std::pair<int, node *> p : nodes) {
      this->A[this->length] = std::make_pair(INF, new node(p.second));

      (this->length)++;
    }
    this->heap_size = this->length;

    this->buildHeap();
  }

  PriorityQueue(std::map<N, node *> nodes, node *start) {
    (this->length) = 0;
    int size = nodes.size();
    this->A = new std::pair<int, node *>[size];

    this->A[this->length] = std::make_pair(0, new node(start));

    (this->length)++;
    for (std::pair<int, node *> p : nodes) {
      if (p.second->data != start->data) {
        this->A[this->length] = std::make_pair(INF, new node(p.second));

        (this->length)++;
      }
    }
    this->heap_size = this->length;

    this->buildHeap();
  }
  // UTILITY FUNCTIONS

  inline int parent(int i) const { return ((i - 1) >> 1); }
  inline int left(int i) const { return (i << 1) + 1; }
  inline int right(int i) const { return ((i << 1) + 2); }

  void updateWeight(node *v, int weight) {
    int i;
    for (i = 0; i < this->heap_size; ++i) {
      if (A[i].second->data == v->data) {
        A[i].first = weight;
        break;
      }
    }
    if (heap_size > 1) {

      while (A[parent(i)].first > A[i].first && i > 0) {
        heapify(parent(i));
        i = parent(i);
      }
    }
  }
  N weight(node *v) const {
    for (int i = 0; i < this->length; ++i) {
      if (A[i].second->data == v->data) {
        return A[i].first;
      }
    }
  }

  bool has(node *v) const {
    for (int i = 0; i < this->heap_size; ++i) {
      if (A[i].second->data == v->data) {
        return true;
      }
    }
    return false;
  }

  // PriorityQueue Functions

  node *extractMin() {
    if (this->heap_size < 1) {
      std::cout << "vacío\n";
      return nullptr;
    }

    node *min = this->A[0].second;
    A[0] = A[this->heap_size - 1];

    this->heap_size = this->heap_size - 1;
    this->heapify(0);
    return min;
  }

  // Heap functions
  void buildHeap() {
    this->heap_size = this->length;
    for (int i = (this->length - 1) / 2; i >= 0; --i) {
      this->heapify(i);
    }
  }

  // caution: this heapify implementation assumes that binary trees rooted at
  // left(i) and right(i) are heaps
  void heapify(int i) {
    int l = this->left(i);
    int r = this->right(i);
    int smallest;

    if (l <= this->heap_size - 1 && this->A[l].first < this->A[i].first) {
      smallest = l;
    } else {
      smallest = i;
    }
    if (r <= this->heap_size - 1 &&
        this->A[r].first < this->A[smallest].first) {
      smallest = r;
    }
    if (smallest != i) {
      swap(i, smallest);
      this->heapify(smallest);
    }
  }

  void swap(int pos1, int pos2) {
    std::pair<int, node *> temp = this->A[pos1];
    this->A[pos1] = this->A[pos2];
    this->A[pos2] = temp;
  }
  void print() const {

    std::cout << "printing pq with length: " << this->length << "\n";
    for (int i = 0; i < this->heap_size; i++) {
      std::cout << "{node: " << *(this->A[i].second)
                << " weight: " << (this->A[i].first) << "}\n";
    }
    std::cout << "finished printing\n";
  }
};

template <typename D> class DisjointSet {
public:
  typedef typename D::N N;
  typedef typename D::E E;
  typedef typename D::edge edge;
  typedef typename D::node node;

private:
  std::map<N, node *> nodes;

public:
  DisjointSet(){};
  void makeSet(N data, double x, double y) {
    node *vertex = new node(data, x, y);
    (this->nodes)[data] = vertex;
  }

  bool unionSet(N data1, N data2) {
    node *parent1 = findSet(data1);
    node *parent2 = findSet(data2);

    if (parent1 != parent2) {
      if (parent1->rank >= parent2->rank) {
        parent1->rank = (parent1->rank == parent2->rank) ? parent1->rank + 1
                                                         : parent1->rank;
        parent2->parent = parent1;
      } else {
        parent1->parent = parent2;
      }

      return true;
    }

    return false;
  }
  void unionSet(node *parent1, node *parent2) {
    if (parent1->rank >= parent2->rank) {
      parent1->rank =
          (parent1->rank == parent2->rank) ? parent1->rank + 1 : parent1->rank;
      parent2->parent = parent1;
    } else {
      parent1->parent = parent2;
    }
  }
  node *findSet(N data) const { return findSet(this->nodes[data]); }

  node *findSet(node *vertex) const {
    node *current = vertex;
    while (current != current->parent) {
      current = current->parent;
    }

    vertex->parent = current;
    return current;
  }
  ~DisjointSet(){};
};

#endif
