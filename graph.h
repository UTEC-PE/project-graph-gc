#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include "node.h"
#include <map>
#include <memory>
#include <utility>
#include <vector>
template <typename G> class Read;

class Traits {
public:
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
  typedef std::map<N, node *> NodeSeq;
  typedef std::vector<std::weak_ptr<edge>> wEdgeSeq;
  typedef std::vector<std::shared_ptr<edge>> EdgeSeq;

private:
  bool directed;
  NodeSeq nodes;
  int counter;

public:
  Graph(){};
  Graph(bool directed_) : nodes(), directed(directed_) {}

  void addVertex(N tag, double x, double y) {
    this->nodes[tag] = new node(tag, x, y);
  }
  void addEdge(E weight, bool directed, N vFrom, N vTo) {
    this->addEdge(weight, directed,
                  const_cast<const node *>(this->nodes[vFrom]),
                  const_cast<const node *>(this->nodes[vTo]));
  }
  void addEdge(E weight, bool directed, const node *vFrom, const node *vTo) {

    auto e12 = std::make_shared<edge>(weight, directed, vFrom, vTo);
    auto e21 = std::make_shared<edge>(weight, directed, vFrom, vTo);
    auto e12w = std::weak_ptr<edge>(e12);

    if (directed) {
      this->nodes[vFrom->tag]->addEdge(e12);
      this->nodes[vTo->tag]->addEdge(e12w);
    } else {
      this->nodes[vTo->tag]->addEdge(e12);
      this->nodes[vFrom->tag]->addEdge(e21);
    }
  }

  void pprint() const {

    for (auto const &v : this->nodes) {
      std::cout << (v.first) << " : ";
      for (auto const &e : v.second->outEdges) {
        std::cout << " | " << *e << " ";
      }
      for (auto const &e : v.second->inEdges) {
        std::cout << " | " << *(e.lock()) << " ";
      }
      std::cout << '\n';
    }
  }
  void analyzeMemory() const {
    for (auto const &v : this->nodes) {
      std::cout << (v.first) << " : ";
      for (auto const &e : v.second->outEdges) {
        std::cout << "(" << *e << "," << e.use_count() << ")"
                  << " ";
      }
      std::cout << " <-> inEdges: ";
      for (auto const &e : v.second->inEdges) {
        std::cout << "(" << *(e.lock()) << "," << e.use_count() << ")"
                  << " ";
      }
    }
  }

  friend class Read<self>;
};
typedef Graph<Traits> graph;

#endif
