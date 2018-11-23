#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {

  Read<graph> readGraph("GRAFO1.txt");

  graph *g = readGraph.getGraph();
  g->pprint();

  std::cout << "\n\n************************************\n\n";
  std::cout << g->getWeight(0, 10) << "<<--- wtf\n";

  std::cout << "\n\n************************************\n\n";
  g->analyzeMemory();
  g->bellmanFord(0);

  g->dijkstra(0);

  g->greedyBFS(0, 7);
  delete g;

  return 0;
}
