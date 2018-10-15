#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>

template <typename T> class Heap {
private:
  T *container;
  int containerSize;
  bool non_decreasing;

public:
  Heap() : container(nullptr), containerSize(0), non_decreasing(1) {}
  Heap(T *container_, int containerSize_, bool non_decreasing_ = 1)
      : container(container_), containerSize(containerSize_),
        non_decreasing(non_decreasing_) {}
  T *HeapSort() {
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
    T temp = this->container[pos1];
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

template <typename T> class Heap {
private:
  T *container;
  int containerSize;

public:
  Heap() : container(nullptr), containerSize(0) {}
  Heap(T *container_, int containerSize_)
      : container(container_), containerSize(containerSize_) {}
  T *HeapSort() {
    int n = this->containerSize - 1;
    while (n--) {
      this->heapifyMAX(n + 1);
      swap(0, n + 1);
    }
    return this->container;
  }
  void heapifyMAX(int lastpos) {
    int startpos = ((lastpos) / 2) + 1;
    while (startpos--) {
      this->doHeapMAX(startpos, lastpos);
    }
  }
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
  void swap(int pos1, int pos2) {
    T temp = this->container[pos1];
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
#endif
