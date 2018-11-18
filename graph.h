#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include "node.h"
#include <map>
#include <memory>
#include <stack>
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

  struct bools {
    bool IS_DIRECTED = false;
    bool HAS_NEG_EDGE = false;
  };

private:
  NodeSeq nodes;
  int counter;
  bools properties;

public:
  Graph(){};
  Graph(bool directed_) : nodes() { this->properties.IS_DIRECTED = directed_; }

  ~Graph() {
    std::cout << "Deleting graph...\n\n";
    int n_nodes = nodes.size();
    while (n_nodes--) {
      this->removeVertex((this->nodes.begin())->first);
      this->pprint();
    }
    std::cout << "\n\nDone.\n";
  }
  void addVertex(N tag, double x, double y) {
    this->nodes[tag] = new node(tag, x, y);
  }
  void addEdge(E weight, bool directed, N vFrom, N vTo) {
    if (weight < 0) {
      this->properties.HAS_NEG_EDGE = true;
    }
    this->addEdge(weight, directed,
                  const_cast<const node *>(this->nodes[vFrom]),
                  const_cast<const node *>(this->nodes[vTo]));
  }
  void addEdge(E weight, bool directed, const node *vFrom, const node *vTo) {

    auto e12 = std::make_shared<edge>(weight, directed, vFrom, vTo);
    auto e21 = std::make_shared<edge>(weight, directed, vTo, vFrom);
    auto e12w = std::weak_ptr<edge>(e12);

    if (directed) {
      this->nodes[vFrom->tag]->addEdge(e12);
      this->nodes[vTo->tag]->addEdge(e12w);
    } else {

      this->nodes[vFrom->tag]->addEdge(e12);
      this->nodes[vTo->tag]->addEdge(e21);
    }
  }

  void removeVertex(N tag) { this->removeVertex(this->nodes[tag]); }

  void removeVertex(node *&v) {
    if (this->properties.IS_DIRECTED) {
      std::stack<N> inEdgesToBeDeleted;
      std::stack<N> outEdgesToBeDeleted;
      //      std::cout << "\n******** removing outedges of v **************\n";
      for (auto &e : v->outEdges) {
        inEdgesToBeDeleted.push(e->nodes[1]->tag);
        e.reset();
      }

      //     std::cout << "\n******** removing inedges of v **************\n";
      for (auto &e : v->inEdges) {
        /*      std::cout << "to the outEdges stack: " <<
           e.lock()->nodes[0]->tag
                        << '\n';*/
        outEdgesToBeDeleted.push(e.lock()->nodes[0]->tag);
        e.reset();
      }

      N tag;
      // std::cout << "\n******** removing coincidences **************\n";
      while (!outEdgesToBeDeleted.empty()) {
        tag = outEdgesToBeDeleted.top();
        outEdgesToBeDeleted.pop();
        this->nodes[tag]->removeEdgeIfGoesTo(v->tag);
      }

      // std::cout << "\n******** clearing expired edges **************\n";
      while (!inEdgesToBeDeleted.empty()) {
        tag = inEdgesToBeDeleted.top();
        this->nodes[tag]->removeExpiredEdge();
        inEdgesToBeDeleted.pop();
      }

    } else {
      std::stack<N> outEdgesToBeDeleted;
      for (auto &e : v->outEdges) {
        outEdgesToBeDeleted.push(e->nodes[1]->tag);
        e.reset();
      }
      N tag;
      while (!outEdgesToBeDeleted.empty()) {
        tag = outEdgesToBeDeleted.top();
        this->nodes[tag]->removeEdgeIfGoesTo(v->tag);
        outEdgesToBeDeleted.pop();
      }
    }

    this->nodes.erase(v->tag);
    delete v;
  }

  void removeEdge(N vFrom, N vTo) {
    this->removeEdge(this->nodes[vFrom], this->nodes[vTo]);
  }
  void removeEdge(node *vFrom, node *vTo) {
    if (this->properties.IS_DIRECTED) {
      vFrom->removeEdgeIfGoesTo(vTo->tag);
      vTo->removeExpiredEdge();
    } else {
      vFrom->removeEdgeIfGoesTo(vTo->tag);
      vTo->removeEdgeIfGoesTo(vFrom->tag);
    }
  }

  void pprint() const {
    for (auto const &v : this->nodes) {
      std::cout << "   <<<<<<  " << (v.first) << " : ";
      for (auto const &e : v.second->outEdges) {
        std::cout << *e << "  |  ";
      }
      for (auto const &e : v.second->inEdges) {
        std::cout << *(e.lock()) << "  |  ";
      }
      std::cout << '\n';
    }
  }
  void analyzeMemory() const {
    for (auto const &v : this->nodes) {
      std::cout << (v.first) << " : ";
      for (auto const &e : v.second->outEdges) {
        std::cout << " " << *e << " <" << e.use_count() << ">  | ";
      }
      std::cout << " <---->  ";
      for (auto const &e : v.second->inEdges) {
        std::cout << " " << *(e.lock()) << " <" << e.use_count() << ">  | ";
      }
      std::cout << "\n";
    }
  }

  friend class Read<self>;
};
typedef Graph<Traits> graph;

#endif
