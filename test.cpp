#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {

  Read<graph> readGraph("testdirected.txt");
  graph g = readGraph.getGraph();
  g.pprint();
  return 0;
}
