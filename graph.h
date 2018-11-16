#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "data_structures.h"
#include "edge.h"
#include "node.h"

using namespace std;

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
  typedef map<N, std::shared_ptr<node>> NodeSeq;
  typedef vector<std::weak_ptr<edge>> wEdgeSeq;
  typedef vector<std::shared_ptr<edge>> EdgeSeq;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;
  typedef typename wEdgeSeq::iterator wEdgeIte;

private:
  NodeSeq nodes;
  NodeIte ni;
  EdgeIte ei;
  bool directed;
  int counter;

public:
  /* ***** CONSTRUCTORS ***** */
  // Empty explicit initialization might be unnecessary

  Graph() : nodes(), counter(0){};

  Graph(bool directed) : nodes(), directed(directed), counter(0){};

  // Copy constructor to 1) copy vertices, edges or entire graph
  Graph(Graph<Tr> *g, char type_of_copy_constructor) {
    switch (type_of_copy_constructor) {
    case 'v':
      this->nodes = g->getVertices();
      this->directed = g->isDirected();
      this->counter = g->lastNodeTag();
      break;
    case 'e':
      // not sure if this is useful
      break;
    default:
      this->nodes = g->adjacencylist();
      this->directed = g->isDirected();
      this->counter = g->lastNodeTag();
      break;
    }
  }
  Graph(int numberOfVertices, bool directed)
      : nodes(), directed(directed), counter(0){};

  ~Graph() {
    std::cout << "number of vertices: " << this->nodes.size() << "\n";
    for (auto &v : this->nodes) {
      this->rawPrint();
      std::cout << "removing vertex " << *(v.second) << std::endl;
      this->removeVertex(v.second);
    }
    std::cout << "Graph deleted" << '\n';
  }

  /* ***** MANIPULATION METHODS ***** */

  // Integer indexed method
  void addVertex(double x, double y) {
    (this->nodes)[this->counter] = std::make_shared<node>(this->counter, x, y);
    (this->counter)++;
  }

  void addVertex(N data, double x, double y) {
    if (!(this->nodes)[data]) {
      (this->nodes)[data] = std::make_shared<node>(data, x, y);
      (this->counter)++;
    } else {
      throw "Illegal node overriding.";
    }
  }
  void addVertex(node *v) {
    (this->nodes)[v->data] = std::make_shared<node>(v->data, v->x, v->y);
    (this->counter)++;
  }
  void addEdge(N v1, N v2, E weight, bool dir) {
    this->addEdge((this->nodes)[v1], (this->nodes)[v2], weight, dir);
  }

  void addEdge(std::shared_ptr<node> &vn1, std::shared_ptr<node> &vn2, E weight,
               bool dir) {

    auto edgeFromTo = std::make_shared<edge>(weight, dir, vn1, vn2);

    if (dir) {
      vn1->addEdge(edgeFromTo);
      vn2->addEdge(edgeFromTo);

    } else {
      auto edgeToFrom = std::make_shared<edge>(weight, dir, vn2, vn1);
      vn1->addEdge(edgeFromTo);
      vn2->addEdge(edgeToFrom);
    }
  }

  void removeVertex(N data) { removeVertex(this->nodes[data]); }
  void removeVertex(std::shared_ptr<node> &v) {
    std::set<int> buffer;

    for (auto &e : v->outEdges) {
      buffer.insert(e->nodes[0]->data == v->data ? e->nodes[1]->data
                                                 : e->nodes[0]->data);
      e.reset();
    }

    if (this->isDirected()) {

      for (auto &e : v->inEdges) {
        buffer.insert((e.lock())->nodes[0]->data == v->data
                          ? (e.lock())->nodes[1]->data
                          : (e.lock())->nodes[0]->data);
        e.reset();
      }
    }
    std::cout << "now removing edge siblings\n";
    if (this->isDirected()) {

      for (auto vb : buffer) {
        removeExpiredEdges(vb);
      }
    }
    for (auto vb : buffer) {
      removeEdgeCoincidences(vb, v->data);
    }
    (this->nodes).erase(v->data);
    std::cout << v->getData() << " use_count: " << v.use_count() << "\n";
    v.reset();
  }
  void removeEdgeCoincidences(N &nodeFrom, N &nodeTo) {
    std::cout << "called removeEdgeCoincidences\n";

    auto tempVectorHolder = &((this->nodes)[nodeFrom]->outEdges);
    tempVectorHolder->erase(
        std::remove_if(tempVectorHolder->begin(), tempVectorHolder->end(),
                       [&](std::shared_ptr<edge> &e) -> bool {
                         return e->removeIfHas(nodeTo);
                       }),
        tempVectorHolder->end());

    std::cout << "succesfully removed edge coincidences\n";
  }

  void removeExpiredEdges(N data) {
    auto tempVectorHolder = &((this->nodes)[data]->inEdges);
    tempVectorHolder->erase(std::remove_if(tempVectorHolder->begin(),
                                           tempVectorHolder->end(),
                                           [](std::weak_ptr<edge> &e) -> bool {
                                             if (e.expired()) {
                                               std::cout << "expired edge\n";
                                               e.reset();
                                               return true;
                                             }
                                             return false;
                                           }),
                            tempVectorHolder->end());
  }
  void removeEdge(N nodeFrom, N nodeTo) {
    if (this->directed) {
      (this->nodes)[nodeFrom]->edges.remove_if(
          [&](edge *&e) -> bool { return e->removeIfSame(nodeTo, nodeFrom); });

      (this->nodes)[nodeTo]->edges.remove_if(
          [&](edge *&e) -> bool { return e->removeIfSame(nodeTo, nodeFrom); });
    } else {

      (this->nodes)[nodeFrom]->edges.remove_if(
          [&](edge *&e) -> bool { return e->removeIfSame(nodeTo, nodeFrom); });

      (this->nodes)[nodeTo]->edges.remove_if(
          [&](edge *&e) -> bool { return e->removeIfSame(nodeFrom, nodeTo); });
    }
  }

  /* ***** UTILITY METHODS ***** */

  inline void setDirected(bool direction) { this->directed = direction; }

  std::map<int, node *> getVertices() {
    std::map<int, node *> vs;
    for (auto &v : this->nodes) {
      vs[v.second->data] = new node(v.second->data, v.second->x, v.second->y);
    }
    return vs;
  }
  std::map<int, node *> adjacencylist() {
    // hope this doesn't pass the adjacency list by reference
    return this->nodes;
  }
  inline bool isDirected() const { return this->directed; }
  inline int lastNodeTag() const { return this->counter; }
  inline int size() const { return (this->nodes).size(); }
  void print() const {
    std::cout << this->size() << ' ' << this->directed << "\n\n";
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      std::cout << (*ni).second->print() << '\n';
    }

    std::cout << '\n';
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      for (EdgeIte it = (*ni).second->outEdges.begin();
           it != (*ni).second->outEdges.end(); it++) {

        if ((*ni).second->print() !=
            ((*it)->printV2())) { // Se verifica q no se imprimas dobles por
                                  // recordar de donde vino
          if (*it) {
            std::cout << (*it)->printV1() << ' ';
            std::cout << (*it)->printV2() << ' ';
            std::cout << (*it)->printWeight() << ' ';
            std::cout << (*it)->printDir() << '\n';
          } else {
            std::cout << "nullptr\n";
          }
        }
      }

      for (wEdgeIte it = (*ni).second->inEdges.begin();
           it != (*ni).second->inEdges.end(); it++) {

        if ((*ni).second->print() !=
            ((*it)->printV2())) { // Se verifica q no se imprimas dobles por
                                  // recordar de donde vino
          if (*it) {
            std::cout << (*it)->printV1() << ' ';
            std::cout << (*it)->printV2() << ' ';
            std::cout << (*it)->printWeight() << ' ';
            std::cout << (*it)->printDir() << '\n';
          } else {
            std::cout << "nullptr\n";
          }
        }
      }
    }
  }

  void rawPrint() const {
    std::cout << "\n\n********** PRINTING NODES ***********\n\n";
    if (this->isDirected()) {
      std::cout << "graph: directed\n\n";
    } else {
      std::cout << "graph: non-directed\n\n";
    }
    for (auto &v : this->nodes) {
      std::cout << *(v.second) << " : ";
      for (auto &e : v.second->outEdges) {
        std::cout << *e << "  |  ";
      }
      for (auto &e : v.second->inEdges) {
        std::cout << *(e.lock()) << "  |  ";
      }
      std::cout << std::endl;
    }

    std::cout << "\n********* FINISHED PRINTING *********\n\n";
  }

  /* ***** ALGORITHMS  ***** */

  self dfs(node *v = nullptr) const {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (this->nodes).begin()->second;
    }

    self ST(this->directed);

    std::vector<node *> output;
    std::vector<node *> visited;
    std::stack<node *> nodes_stack;
    nodes_stack.push(v);

    node *new_v = new node(v);
    ST.addVertex(new_v);

    while (!(nodes_stack.empty())) {
      v = nodes_stack.top();
      nodes_stack.pop();
      if (!(v->in(visited))) {
        visited.push_back(v);
      }

      for (const auto &edge : v->edges) {

        if (edge->nodes[0]->is(*v) && !(edge->nodes[1]->in(visited))) {
          nodes_stack.push(edge->nodes[1]);

          if (!(ST.nodes[edge->nodes[1]->data])) {
            new_v = new node(edge->nodes[1]);
            ST.addVertex(new_v);
            ST.nodes[v->data]->addEdge(new_v, edge->data, this->directed);
          }
        }
      }
    }

    for (auto &v : visited) {
      std::cout << *v << ' ';
    }
    std::cout << "\n";
    return ST;
  }
  self bfs(node *v = nullptr) const {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (this->nodes).begin()->second;
    }

    self ST(this->directed);

    std::vector<node *> visited;
    std::queue<node *> nodes_queue;
    nodes_queue.push(v);
    visited.push_back(v);

    node *new_v = new node(v);
    ST.addVertex(new_v);

    while (!(nodes_queue.empty())) {
      v = nodes_queue.front();
      nodes_queue.pop();

      for (const auto &edge : v->edges) {
        if (edge->nodes[0]->is(*v) && !(edge->nodes[1]->in(visited))) {

          visited.push_back(edge->nodes[1]);
          nodes_queue.push(edge->nodes[1]);
          new_v = new node(edge->nodes[1]);
          ST.addVertex(new_v);
          ST.nodes[v->data]->addEdge(new_v, edge->data, this->directed);
        }
      }
    }

    for (auto &v : visited) {
      std::cout << *v << ' ';
    }
    std::cout << "\n";
    return ST;
  }

  self prim(N startpos = -1) const {
    // check if graph is connected
    self MST(this->directed, 'v');
    std::map<int, node *> parent;
    node *start = nullptr;
    if (startpos == -1) {
      start = ((this->nodes).begin()->second);
    } else {
      if (this->nodes[startpos]) {
        start = (this->nodes[startpos]);
      }
    }

    PriorityQueue<self> Q(this->nodes, start);
    // Q.print();
    parent[start->data] = nullptr;
    node *u = nullptr;

    while (Q.heap_size != 0) {

      u = Q.extractMin();
      for (auto &e : this->nodes[u->data]->edges) {

        if (Q.has(e->nodes[1]) && e->data < Q.weight(e->nodes[1])) {

          parent[e->nodes[1]->data] = u;
          Q.updateWeight(e->nodes[1], e->data);
        }
      }
    }
    for (std::pair<int, node *> p : parent) {
      if (p.second) {

        std::cout << "parent of " << p.first << " is " << p.second->data
                  << "\n";
      } else {
        std::cout << "parent of " << p.first << " is "
                  << "self\n";
      }
    }
    return MST;
  }
  self *kruskal() const {
    self MST(this, 'v');
    DisjointSet<self> DS;
    std::set<edge, non_dec_unique<edge>> GE;

    for (auto &v : nodes) {
      DS.makeSet((v.second)->data, (v.second)->x, (v.second)->y);
      for (auto e : (v.second)->edges) {
        if (e->nodes)
          GE.insert(*e);
      }
    }

    typename std::set<edge>::iterator it;
    for (it = GE.begin(); it != GE.end(); ++it) {

      std::cout << *it << '\n';
    }
    std::cout << '\n' << "finish\n";

    for (auto &e : GE) {
      node *parent1 = findSet(e->nodes[0]);
      node *parent2 = findSet(e->nodes[1]);

      if (parent1 != parent2) {
        MST[(e->nodes[0])->data].addEdge(e);
        DS.unionSet(parent1, parent2);
      }
    }
    return MST;
  }

  float density() const {
    float numberEdges = 0;
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      for (EdgeIte it = (*ni).second->edges.begin();
           it != (*ni).second->edges.end(); it++) {
        numberEdges++;
      }
    }
    return ((numberEdges) / (this->size() * (this->size() - 1)));
  }

  bool isDense(float criteria = 0.6) const {
    if (criteria < this->density())
      return true;
    else
      return false;
  }

  int nodeGrade(N tag) const {
    if (!this->isDirected()) {
      node *pnode = nodes[tag];
      int grade = 0;
      for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end();
           it++) {
        grade++;
      }
      return grade;
    } else {
      return this->nodeInGrade(tag) + this->nodeOutGrade(tag);
    }
  }

  int nodeInGrade(N tag) const {
    if (!this->isDirected()) {
      return this->nodeGrade(tag);
    } else {
      node *pnode = nodes[tag];
      int grade = 0;
      for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end();
           it++) {
        if ((*it)->printV2() == tag) {
          grade++;
        }
      }
      return grade;
    }
  }

  int nodeOutGrade(N tag) const {
    if (!this->isDirected()) {
      return this->nodeGrade(tag);
    } else {
      node *pnode = nodes[tag];
      int grade = 0;
      for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end();
           it++) {
        if ((*it)->printV1() == tag) {
          grade++;
        }
      }
      return grade;
    }
  }

  bool isFontNode(N tag) const {
    if (!this->isDirected()) {
      return false;
    }
    if (this->nodeOutGrade(tag) != 0 && this->nodeInGrade(tag) == 0) {
      return true;
    } else
      return false;
  }

  bool isSunkenNode(N tag) const {
    if (!this->isDirected()) {
      return false;
    }
    if (this->nodeOutGrade(tag) == 0 && this->nodeInGrade(tag) != 0) {
      return true;
    } else
      return false;
  }

  void nodeInfo(E tag) const {
    std::cout << "Grado: " << this->nodeGrade(tag) << '\n';
    std::cout << "Grado de entrada: " << this->nodeInGrade(tag) << '\n';
    std::cout << "Grado de salida: " << this->nodeOutGrade(tag) << '\n';
    std::cout << "Nodo hundido: " << this->isSunkenNode(tag) << '\n';
    std::cout << "Nodo fuente: " << this->isFontNode(tag) << '\n';
  }

  bool isConnected() const {
    if (this->isDirected()) {
      return false;
    } else {
      self dfsGraph = this->dfs();
      if (this->size() == dfsGraph.size()) {
        return true;
      } else
        return false;
    }
  }

  bool isStronglyConnected() const {
    if (!this->isDirected()) {
      return false;
    } else {
      for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end();
           ++ni) {
        self dfsGraph = this->dfs((*ni).second);
        if (this->size() != dfsGraph.size()) {
          return false;
        }
      }
      return true;
    }
  }

  bool isBipartite() const {
    typedef map<E, bool> coloredMap;
    coloredMap redBlueMap;
    queue<node *> unColored;
    redBlueMap[(*((this->nodes).begin())).first] = true;
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      if (redBlueMap.find((*ni).first) != redBlueMap.end()) {
        bool color = !((*((redBlueMap.find((*ni).first)))).second);
        for (EdgeIte it = (*ni).second->edges.begin();
             it != (*ni).second->edges.end(); it++) {
          if ((*ni).first == (*it)->printV1()) {
            if (redBlueMap.find((*it)->printV2()) != redBlueMap.end()) {
              if (redBlueMap[(*it)->printV2()] != color) {
                return false;
              }
            } else {
              redBlueMap[(*it)->printV2()] = color;
            }
          }
        }
      } else {
        unColored.push((*ni).second);
      }
    }
    while (!unColored.empty()) {
      if (redBlueMap.find((unColored.front())->print()) != redBlueMap.end()) {
        bool color = !(redBlueMap[(unColored.front())->print()]);
        for (EdgeIte it = (unColored.front())->edges.begin();
             it != (unColored.front())->edges.end(); it++) {
          if (unColored.front()->print() == (*it)->printV1()) {
            if (redBlueMap.find((*it)->printV2()) != redBlueMap.end()) {
              if (redBlueMap[(*it)->printV2()] != color) {
                return false;
              }
            } else {
              redBlueMap[(*it)->printV2()] = color;
            }
          }
        }
        unColored.pop();
      } else {
        redBlueMap[(unColored.front())->print()] = true;
        bool color = !(redBlueMap[(unColored.front())->print()]);
        for (EdgeIte it = (unColored.front())->edges.begin();
             it != (unColored.front())->edges.end(); it++) {
          if ((unColored.front())->print() == (*it)->printV1()) {
            if (redBlueMap.find((*it)->printV2()) != redBlueMap.end()) {
              if (redBlueMap[(*it)->printV2()] != color) {
                return false;
              }
            } else {
              redBlueMap[(*it)->printV2()] = color;
            }
          }
        }
        unColored.pop();
      }
    }
    return true;
  }

  node *findNode(N tag) const {
    if (nodes.find(tag) != nodes.end()) {
      return nodes[tag];
    } else
      return nullptr;
  }

  edge *findEdge(N v1, N v2) const {
    if (nodes.find(v1) != nodes.end()) {
      for (EdgeIte it = nodes[v1]->edges.begin(); it != nodes[v1]->edges.end();
           it++) {
        if ((*it)->printV1() == v1 && (*it)->printV2() == v2) {
          return (*it);
        }
      }
    } else
      return nullptr;
  }
};

typedef Graph<Traits> graph;

#endif
