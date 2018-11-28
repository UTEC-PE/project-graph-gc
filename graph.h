#ifndef GRAPH_H
#define GRAPH_H

#include "bellman_ford.h"
#include "best_first_search.h"
#include "dijkstra.h"
#include "edge.h"
#include "node.h"
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename G> class Read;
template <typename G> class DijkstraHelper;
template <typename G> class BellmanHelper;
template <typename G> class BFSHelper;

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

    if (directed) {

      auto e12w = std::weak_ptr<edge>(e12);
      this->nodes[vFrom->tag]->addEdge(e12);
      this->nodes[vTo->tag]->addEdge(e12w);
    } else {

      auto e21 = std::make_shared<edge>(weight, directed, vTo, vFrom);
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

  E getWeight(N vFrom, N vTo) const {
    return (this->nodes.at(vFrom)->getWeight(vTo));
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

  // ********************* ALGORITHMS *******************

  void dijkstra(N start) {
    // validating

    if (!this->nodes[start])
      throw std::invalid_argument("node out of range");
    if (this->properties.HAS_NEG_EDGE) {
      throw std::invalid_argument(
          "illegal action: tried to run dijkstra with a negative edge.");
    }

    // std::cout << "creating dijkstra helper\n";
    DijkstraHelper<self> DH(start, this);

    // std::cout << "printing dijkstra helper\n";
    DH.printTable();
  }

  void bellmanFord(N start) {
    // validating

    if (!this->nodes[start])
      throw std::invalid_argument("node out of range");
    // std::cout << "creating bellman ford helper\n";
    BellmanHelper<self> BH(start, this);

    // std::cout << "printing bellman ford helper\n";
    BH.printTable();
  }

  void greedyBFS(N start, N goal) {

    if (!this->nodes[start])
      throw std::invalid_argument("node out of range");

    // std::cout << "creating greedy BFS helper\n";
    BFSHelper<self> BFS(start, goal, this);

    // std::cout << "printing greedy BFS helper\n";
    BFS.printTable();
  }

  self aEstrella(N tagStart, N tagEnd) {
    self ruta(true);
    const node *currentNode = this->findNode(tagStart);
    node *endNode = this->findNode(tagEnd);
    if (currentNode == nullptr || this->findNode(tagEnd) == nullptr) {
      throw("No existen los nodos solicitados");
    }
    ruta.addVertex(currentNode->getTag(), (currentNode->getCoordinates()).x,
                   (currentNode->getCoordinates()).y);
    int pathValue = 0;
    while (currentNode->getTag() != tagEnd) {
      int moveValue = INT_MAX;
      const node *nextNode = nullptr;
      int weight = 0;
      for (auto itEdge = (currentNode->outEdges).begin();
           itEdge != (currentNode->outEdges).end(); itEdge++) {
        if (ruta.findNode(((*itEdge)->nodes[1])->getTag()) != nullptr) {
          continue;
        }
        if (moveValue >
            ((pathValue + (*itEdge)->getWeight()) +
             pow(pow((endNode->getCoordinates()).x -
                         (((*itEdge)->nodes[1])->getCoordinates()).x,
                     2) +
                     pow((endNode->getCoordinates()).y -
                             (((*itEdge)->nodes[1])->getCoordinates()).y,
                         2),
                 0.5))) {
          nextNode = (*itEdge)->nodes[1];
          moveValue =
              ((pathValue + (*itEdge)->getWeight()) +
               pow(pow((endNode->getCoordinates()).x -
                           (((*itEdge)->nodes[1])->getCoordinates()).x,
                       2) +
                       pow((endNode->getCoordinates()).y -
                               (((*itEdge)->nodes[1])->getCoordinates()).y,
                           2),
                   0.5));
          weight = (*itEdge)->getWeight();
        }
      }
      if (nextNode == nullptr) {
        throw("No existe un camino entre los nodos");
      }
      pathValue += weight;
      ruta.addVertex(nextNode->getTag(), (nextNode->getCoordinates()).x,
                     (nextNode->getCoordinates()).y);
      ruta.addEdge(weight, true, currentNode, nextNode);
      currentNode = nextNode;
    }
    return ruta;
  }

  int ***floydWarshall() {
    int biMatriz[2][(this->nodes).size()][(this->nodes).size()];
    for (int i = 0; i < (this->nodes).size(); i++) {
      for (int j = 0; j < (this->nodes).size(); j++) {
        if (i == j) {
          biMatriz[0][i][j] = 0;
          biMatriz[1][i][j] = 0;
        } else {
          biMatriz[0][i][j] = this->getWeight(i, j);
          biMatriz[1][i][j] = j;
        }
      }
    }

    for (int i = 0; i < (this->nodes).size(); i++) {
      for (int j = 0; j < (this->nodes).size(); j++) {
        for (int k = 0; k < (this->nodes).size(); k++) {
          if (biMatriz[0][i][k] != INT_MAX && biMatriz[0][j][i] != INT_MAX) {
            if ((biMatriz[0][i][k] + biMatriz[0][j][i]) < biMatriz[0][j][k]) {
              biMatriz[0][j][k] = biMatriz[0][i][k] + biMatriz[0][j][i];
              biMatriz[1][j][k] = j;
            }
          }
        }
      }
    }

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < (this->nodes).size(); j++) {
        for (int k = 0; k < (this->nodes).size(); k++) {
          if (biMatriz[i][j][k] == INT_MAX) {
            std::cout << "INF" << '\t';
          } else
            std::cout << biMatriz[i][j][k] << '\t';
        }
        std::cout << '\n';
      }
      std::cout << "----------------------------------------------" << '\n';
    }
    std::cout << "----------------------------------------------" << '\n';
  }

  // ********************* UTILITY *******************

  node *findNode(N tag) {
    if (nodes.find(tag) != nodes.end()) {
      return nodes[tag];
    } else
      return nullptr;
  }

  int getNumberOfNodes() const { return this->nodes.size(); }

  friend class Read<self>;
  friend class DijkstraHelper<self>;
  friend class BellmanHelper<self>;
  friend class BFSHelper<self>;
};
typedef Graph<Traits> graph;

#endif
