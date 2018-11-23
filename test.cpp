#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {

  Read<graph> readGraph("dijkstra_dir_test.txt");

  graph *g = readGraph.getGraph();
  g->pprint();

  std::cout << '\n';

  std::cout << std::endl;
  g->pprint();

  g->analyzeMemory();
  /*
  g->bellmanFord(0);

  g->dijkstra(0);

  g->greedyBFS(0, 7);
  */
  delete g;

  return 0;
}
