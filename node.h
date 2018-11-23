#ifndef NODE_H
#define NODE_H
#include "graph.h"
#include <climits>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <typename G> class Node {

public:
  typedef typename G::N N;
  typedef typename G::E E;
  typedef typename G::edge edge;
  typedef typename G::EdgeSeq EdgeSeq;
  typedef typename G::wEdgeSeq wEdgeSeq;

  struct pairN {
    double x;
    double y;
  };

  N tag;

  pairN coordinates;
  wEdgeSeq inEdges;
  EdgeSeq outEdges;

  Node(N tag_, double x, double y) : tag(tag_) {
    this->coordinates = pairN{x, y};
  }
  inline pairN getCoordinates() const { return this->coordinates; }
  inline N getTag() const { return this->tag; }
  E getWeight(N vTo) const {
    auto A = std::find_if(outEdges.begin(), outEdges.end(),
                          [&](auto const &e) {
                            if (e->is(this->tag, vTo)) {
                              return true;
                            } else {
                              return false;
                            }
                          }

    );
    if (A == outEdges.end()) {
      return INT_MAX;
    } else {
      return (*A)->getWeight();
    }
  }
  inline bool operator==(N v) const { return this->tag == v; }
  void addEdge(std::shared_ptr<edge> &e) {
    (this->outEdges).push_back(std::move(e));
  }
  void addEdge(std::weak_ptr<edge> &e) {
    (this->inEdges).push_back(std::move(e));
  }
  void removeEdgeIfGoesTo(N tag) {
    // this method be used, by definition, in outEdges.
    this->outEdges.erase(std::remove_if(this->outEdges.begin(),
                                        this->outEdges.end(),
                                        [&](auto &e) -> bool {
                                          if (e->nodes[1]->tag == tag) {
                                            e.reset();
                                            return true;
                                          } else {
                                            return false;
                                          }
                                        }),
                         this->outEdges.end());
  }
  void removeExpiredEdge() {
    this->inEdges.erase(std::remove_if(this->inEdges.begin(),
                                       this->inEdges.end(),
                                       [](auto &e) -> bool {
                                         if (e.expired()) {
                                           e.reset();
                                           return true;
                                         } else {

                                           return false;
                                         }
                                       }),
                        this->inEdges.end());
  }
  void removeEdge(N vFrom, N vTo) {
    if (vFrom == this->tag) {
      this->outEdges.erase(std::remove_if(this->outEdges.begin(),
                                          this->outEdges.end(),
                                          [&](auto &e) -> bool {
                                            if (e->is(vFrom, vTo)) {
                                              e.reset();
                                              return true;
                                            } else {
                                              return false;
                                            }
                                          }),
                           this->outEdges.end());
    } else {
      this->inEdges.erase(std::remove_if(this->inEdges.begin(),
                                         this->inEdges.end(),
                                         [&](auto &e) -> bool {
                                           if ((e.lock())->is(vFrom, vTo)) {
                                             e.reset();
                                             return true;
                                           } else {
                                             return false;
                                           }
                                         }),
                          this->inEdges.end());
    }
  }
  ~Node() {
    // This destructor does NOT free the memory occupied by the edges, use
    // Graph->removeVertex() for that.
    std::cout << "Destroying node " << this->tag << "... ";
    std::cout << "Done.\n";
  }
};

#endif
