#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
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
  typedef map<N, node *> NodeSeq;
  typedef list<edge *> EdgeSeq;
  typedef typename NodeSeq::iterator NodeIte;
  typedef typename EdgeSeq::iterator EdgeIte;

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
      this->nodes = g->vertices();
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
  Graph(int number_of_vertices, bool directed)
      : nodes(), directed(directed), counter(0){};

  /* ***** MANIPULATION METHODS ***** */

  // Integer indexed method
  void addVertex(double x, double y) {
    (this->nodes)[this->counter] = new node(this->counter, x, y);
    (this->counter)++;
  }

  void addVertex(N data, double x, double y) {
    if ((this->nodes).find(data) == (this->nodes).end()) {
      (this->nodes)[data] = new node(data, x, y);
      (this->counter)++;
    } else {
      cout << "you're trying to override a node" << endl;
    }
  }

  void addVertex(node *v) {
    (this->nodes)[v->data] = v;
    (this->counter)++;
  }

  void addEdge(N v1, N v2, E weight, bool dir) {
    node *vn1 = (this->nodes)[v1];
    node *vn2 = (this->nodes)[v2];

    edge *e1 = new edge(weight, dir, vn1, vn2);

    if (dir) {
      vn1->addEdge(e1);
      vn2->addEdge(e1);
    } else {
      edge *e2 = new edge(weight, dir, vn2, vn1);
      vn1->addEdge(e1);
      vn2->addEdge(e2);
    }
  }

  void addEdge(node *vn1, node *vn2, E weight, bool dir) {
    edge *e1 = new edge(weight, dir, vn1, vn2);
    if (dir) {
      vn1->addEdge(e1);
      vn2->addEdge(e1);
    } else {
      edge *e2 = new edge(weight, dir, vn2, vn1);
      vn1->addEdge(e1);
      vn2->addEdge(e2);
    }
  }

  void removeVertex(N data) { removeVertex(this->nodes[data]); }
  void removeVertex(node *&v) {
    std::map<int, int> buffer;
    for (auto &e : v->edges) {
      // In theory buffer[value]++ will first check if key exists and if not,
      // initialize it with 0 and then sum 1.
      buffer[e->nodes[0]->data == v->data ? e->nodes[1]->data
                                          : e->nodes[0]->data]++;
      delete e;
      e = nullptr;
    }
    if (this->directed) {
      for (std::pair<int, int> vb : buffer) {

        removeEmptyEdges(vb.first);
      }
    } else {
      for (std::pair<int, int> vb : buffer) {
        removeEdgeCoincidences(v->data, vb.first);
      }
    }
    (this->nodes).erase(v->data);
    delete v;
  }
  void removeEdgeCoincidences(N &data1, N &data2) {
    (this->nodes)[data2]->edges.remove_if(
        [&](edge *&e) -> bool { return e->remove_if_has(data1); });
  }
  void removeEmptyEdges(N &data) {
    std::cout << "Removing empty edges\n";
    (this->nodes)[data]->edges.remove_if([](edge *&e) -> bool {
      bool null = e->nodes[0] == nullptr;
      if (null) {
        e = nullptr;
      }
      return (null);
    });
  }
  void removeEdge() {
    // TODO: @dgcnz
  }

  /* ***** UTILITY METHODS ***** */

  std::map<int, node *> vertices() {
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
  inline bool isDirected() { return this->directed; }
  inline int lastNodeTag() { return this->counter; }
  inline int size() { return (this->nodes).size(); }
  void print() {
    std::cout << this->size() << ' ' << this->directed << "\n\n";
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      std::cout << (*ni).second->print() << '\n';
    }

    std::cout << '\n';
    for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
      for (EdgeIte it = (*ni).second->edges.begin();
           it != (*ni).second->edges.end(); it++) {

				if ((*ni).second->print()!=((*it)->printV2())) { //Se verifica q no se imprimas dobles por recordar de donde vino
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
  };

  /* ***** ALGORITHMS  ***** */

  self dfs(node *v = nullptr) {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (*((this->nodes).begin())).second;
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
          std::cout << "creating node " << *(edge->nodes[1]) << std::endl;

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
  self bfs(node *v = nullptr) {
    // If starting node is null, initialize starting node with first node in
    // graph
    if (!v) {
      v = (*((this->nodes).begin())).second;
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

  void prim() {}
  self *kruskal() {
    self A(this, 'v');
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
        A[(e->nodes[0])->data].addEdge(e);
        DS.unionSet(parent1, parent2);
      }
    }
    return A;
  }

	float density() {
		float numberEdges = 0;
		for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
			for (EdgeIte it = (*ni).second->edges.begin(); it != (*ni).second->edges.end(); it++) {
				numberEdges++;
			}
		}
		return ((numberEdges)/(this->size()*(this->size()-1)));
	}

	bool isDense(float criteria = 0.6) {
		if (criteria < this->density()) return true;
		else return false;
	}

	int nodeGrade(N tag) {
		if (!this->isDirected()) {
			node *pnode = nodes[tag];
			int grade = 0;
			for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end(); it++) {
				grade++;
			}
			return grade;
		}else{
			return this->nodeInGrade(tag) + this->nodeOutGrade(tag);
		}
	}

	int nodeInGrade(N tag) {
		if (!this->isDirected()) {
			return this->nodeGrade(tag);
		}else{
			node *pnode = nodes[tag];
			int grade = 0;
			for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end(); it++) {
				if ((*it)->printV2()==tag) {
					grade++;
				}
			}
			return grade;
		}
	}

	int nodeOutGrade(N tag) {
		if (!this->isDirected()) {
			return this->nodeGrade(tag);
		}else{
			node *pnode = nodes[tag];
			int grade = 0;
			for (EdgeIte it = (pnode)->edges.begin(); it != (pnode)->edges.end(); it++) {
				if ((*it)->printV1()==tag) {
					grade++;
				}
			}
			return grade;
		}
	}

	bool isFontNode(N tag) {
		if (!this->isDirected()) {
			return false;
		}
		if (this->nodeOutGrade(tag) != 0 && this->nodeInGrade(tag) == 0) {
			return true;
		}else return false;
	}

	bool isSunkenNode(N tag){
		if (!this->isDirected()) {
			return false;
		}
		if (this->nodeOutGrade(tag) == 0 && this->nodeInGrade(tag) != 0) {
			return true;
		}else return false;
	}

	void nodeInfo(E tag) {
		std::cout << "Grado: " << this->nodeGrade(tag) << '\n';
		std::cout << "Grado de entrada: " << this->nodeInGrade(tag) << '\n';
		std::cout << "Grado de salida: " << this->nodeOutGrade(tag) << '\n';
		std::cout << "Nodo hundido: " << this->isSunkenNode(tag) << '\n';
		std::cout << "Nodo fuente: " << this->isFontNode(tag) << '\n';
	}

	bool isConnected() {
		if (this->isDirected()) {
			return false;
		} else {
			self dfsGraph = this->dfs();
			if (this->size()==dfsGraph.size()) {
				return true;
			} else return false;
		}
	}

	bool isStronglyConnected() {
		if (!this->isDirected()) {
			return false;
		}else{
			for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
				self dfsGraph = this->dfs((*ni).second);
				if (this->size()!=dfsGraph.size()) {
					return false;
				}
			}
			return true;
		}
	}

	bool isBipartite(){
		typedef map<E, bool> coloredMap;
		coloredMap redBlueMap;
		queue <node *> unColored;
		redBlueMap[(*((this->nodes).begin())).first]=true;
		for (NodeIte ni = (this->nodes).begin(); ni != (this->nodes).end(); ++ni) {
			if (redBlueMap.find((*ni).first)!=redBlueMap.end()) {
				bool color = !((*((redBlueMap.find((*ni).first)))).second);
				for (EdgeIte it = (*ni).second->edges.begin();it != (*ni).second->edges.end(); it++) {
					if ((*ni).first == (*it)->printV1()) {
						if (redBlueMap.find((*it)->printV2())!=redBlueMap.end()) {
							if (redBlueMap[(*it)->printV2()]!=color) {
								return false;
							}
						}else{
							redBlueMap[(*it)->printV2()]=color;
						}
					}
				}
			}else{
				unColored.push((*ni).second);
			}
		}
		while (!unColored.empty()) {
			if (redBlueMap.find((unColored.front())->print())!=redBlueMap.end()) {
				bool color = !(redBlueMap[(unColored.front())->print()]);
				for (EdgeIte it = (unColored.front())->edges.begin();it != (unColored.front())->edges.end(); it++) {
					if (unColored.front()->print() == (*it)->printV1()) {
						if (redBlueMap.find((*it)->printV2())!=redBlueMap.end()) {
							if (redBlueMap[(*it)->printV2()]!=color) {
								return false;
							}
						}else{
							redBlueMap[(*it)->printV2()]=color;
						}
					}
				}
				unColored.pop();
			}else{
				redBlueMap[(unColored.front())->print()]=true;
				bool color = !(redBlueMap[(unColored.front())->print()]);
				for (EdgeIte it = (unColored.front())->edges.begin();it != (unColored.front())->edges.end(); it++) {
					if ((unColored.front())->print() == (*it)->printV1()) {
						if (redBlueMap.find((*it)->printV2())!=redBlueMap.end()) {
							if (redBlueMap[(*it)->printV2()]!=color) {
								return false;
							}
						}else{
							redBlueMap[(*it)->printV2()]=color;
						}
					}
				}
				unColored.pop();
			}
		}
		return true;
	}

	node* findNode(N tag) {
		if (nodes.find(tag)!=nodes.end()) {
			return nodes[tag];
		} else return nullptr;
	}

	edge* findEdge(N v1, N v2) {
		if (nodes.find(v1)!=nodes.end()) {
			for (EdgeIte it = nodes[v1]->edges.begin(); it != nodes[v1]->edges.end(); it++) {
				if ((*it)->printV1()==v1 && (*it)->printV2()==v2) {
					return (*it);
				}
			}
		} else return nullptr;
	}
};

typedef Graph<Traits> graph;

#endif
