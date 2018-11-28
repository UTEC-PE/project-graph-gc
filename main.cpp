#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {

  Read<graph> readGraph("GRAFO3.txt");

  graph *g = readGraph.getGraph();
  //g->pprint();

  g->aEstrella(0, 6).pprint();
  delete g;

  return 0;
}
