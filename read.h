#ifndef READ_H
#define READ_H

#include "graph.h"
#include <fstream>

/**
 * Clase de ayuda para leer el grafo desde un archivo,
 * no es necesario que la utilicen, podrían implementar su lector
 * desde su grafo o algún otro lado
 **/
template <typename G> class Read {

  typedef typename G::N N;
  typedef typename G::E E;

private:
  graph *readGraph;

public:
  Read(const char *file) {
    std::ifstream graphFile;
    graphFile.open(file);
    if (graphFile.is_open()) {
      int numberNodes;
      bool isDirected;
      graphFile >> numberNodes >> isDirected;

      this->readGraph = new graph(isDirected);
      for (int tag = 0; tag < numberNodes; ++tag) {
        double x, y;
        graphFile >> x >> y;
        this->readGraph->addVertex(tag, x, y);
      }
      while (!graphFile.eof()) {
        N vFrom, vTo;
        E weight;
        bool directed;
        graphFile >> vFrom >> vTo >> weight >> directed;
        if (graphFile.eof())
          break;
        this->readGraph->addEdge(weight, directed, vFrom, vTo);
      }

      graphFile.close();
    } else
      throw("Unable to open the graph file");
  }

  graph *&getGraph() { return this->readGraph; }
};

#endif
