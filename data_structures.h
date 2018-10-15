#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <map>

#include "node.h"

template <typename D> class Heap {
public:
  typedef Heap<D> self;
  typedef Node<self> node;

private:
  D *container;
  int containerSize;
  bool non_decreasing;

public:
  Heap() : container(nullptr), containerSize(0), non_decreasing(1) {}
  Heap(D *container_, int containerSize_, bool non_decreasing_ = 1)
      : container(container_), containerSize(containerSize_),
        non_decreasing(non_decreasing_) {}
  D *HeapSort() {
    int n = this->containerSize - 1;
    if (this->non_decreasing) {
      while (n--) {
        this->heapifyMAX(n + 1);
        swap(0, n + 1);
      }
    } else {
      while (n--) {
        this->heapifyMIN(n + 1);
        swap(0, n + 1);
      }
    }
    return this->container;
  }

  // SUBTREE FUNCTIONS

  void heapifyMAX(int lastpos = -1) {
    if (lastpos == -1) {
      lastpos = this->containerSize - 1;
    }
    int startpos = ((lastpos) / 2) + 1;
    while (startpos--) {
      this->doHeapMAX(startpos, lastpos);
    }
  }
  void heapifyMIN(int lastpos = -1) {
    if (lastpos == -1) {
      lastpos = this->containerSize - 1;
    }
    int startpos = ((lastpos) / 2) + 1;
    while (startpos--) {
      this->doHeapMIN(startpos, lastpos);
    }
  }

  // 3-NODE TREE FUNCTIONS
  void doHeapMAX(int pos, int lastpos) {
    if ((pos * 2 + 1) <= lastpos) {
      if (this->container[pos * 2 + 1] > this->container[pos]) {
        swap(pos * 2 + 1, pos);
      }
    }
    if ((pos * 2 + 2) <= lastpos) {
      if (this->container[pos * 2 + 2] > this->container[pos]) {
        swap(pos * 2 + 2, pos);
      }
    }
  }

  void doHeapMIN(int pos, int lastpos) {
    if ((pos * 2 + 1) <= lastpos) {
      if (this->container[pos * 2 + 1] < this->container[pos]) {
        swap(pos * 2 + 1, pos);
      }
    }
    if ((pos * 2 + 2) <= lastpos) {
      if (this->container[pos * 2 + 2] < this->container[pos]) {
        swap(pos * 2 + 2, pos);
      }
    }
  }
  void swap(int pos1, int pos2) {
    D temp = this->container[pos1];
    this->container[pos1] = this->container[pos2];
    this->container[pos2] = temp;
  }
  void printArray() {
    for (int i = 0; i < this->containerSize; i++) {
      std::cout << this->container[i] << " ";
    }
    std::cout << std::endl;
  }
};

template <typename D> class DisjointSet {
public:
  typedef DisjointSet<D> self;
  typedef Node<self> node;

private:
  std::map<int, node *> nodes;

public:
  DisjointSet(){};
  void makeSet(int data) {
    node *vertex = new node(data);
    this->nodes[data] = vertex;
  }

  bool unionSet(int data1, int data2) {
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
  node *findSet(int data) { return findSet(this->nodes[data]); }

  node *findSet(node *vertex) {
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
