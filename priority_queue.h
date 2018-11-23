#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "graph.h"
#include <climits>
#include <iostream>
#include <map>
#include <utility>

template <typename D> class PriorityQueue {
public:
  typedef typename D::N N;
  typedef typename D::E E;
  typedef typename D::edge edge;
  typedef typename D::node node;

  std::pair<E, N> *A;
  int length;
  int heap_size;
  const int INF = INT_MAX;

  // CONSTRUCTORS

  PriorityQueue(const std::map<N, node *> &nodes, N start,
                E defaultWeight = 0) {
    (this->length) = 0;
    int size = nodes.size();
    this->A = new std::pair<E, N>[size];

    this->A[this->length] = std::make_pair(defaultWeight, start);

    (this->length)++;
    for (auto const &p : nodes) {
      if (p.second->tag != start) {
        this->A[this->length] = std::make_pair(INF, p.first);
        (this->length)++;
      }
    }
    this->heap_size = this->length;

    this->buildHeap();
  }

  PriorityQueue() {
    (this->length) = 0;
    (this->heap_size) = 0;
  }

  // UTILITY FUNCTIONS

  inline int parent(int i) const { return ((i - 1) >> 1); }
  inline int left(int i) const { return (i << 1) + 1; }
  inline int right(int i) const { return ((i << 1) + 2); }

  void updateWeight(N v, int weight) {
    int i;
    for (i = 0; i < this->heap_size; ++i) {
      if (A[i].second == v) {
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

  void insert(N node, E weight) {
    this->length++;
    this->heap_size++;
    int i = this->heap_size;
    while (i > 0 && A[parent(i)] < node) {
      A[i] = A[parent(i)];
      i = parent(i);
    }
    A[i] = node;
  }

  N weight(N v) const {
    for (int i = 0; i < this->length; ++i) {
      if (A[i].second == v) {
        return A[i].first;
      }
    }
  }

  bool has(N v) const {
    for (int i = 0; i < this->heap_size; ++i) {
      if (A[i].second == v) {
        return true;
      }
    }
    return false;
  }

  // PriorityQueue Functions

  N extractMin() {
    if (this->heap_size < 1) {
      std::cout << "vacío\n";
      throw("empty");
    }

    N min = this->A[0].second;
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
    auto temp = this->A[pos1];
    this->A[pos1] = this->A[pos2];
    this->A[pos2] = temp;
  }
  void print() const {

    std::cout << "printing pq with length: " << this->length << "\n";
    for (int i = 0; i < this->heap_size; i++) {
      std::cout << "{node: " << (this->A[i].second)
                << " weight: " << (this->A[i].first) << "}\n";
    }
    std::cout << "finished printing\n";
  }
};
#endif
