#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {
  std::cout << "test1\n";
  Read<graph> readGraph("testdirected.txt");
  std::cout << "test2\n";
  graph g = readGraph.getGraph();
  std::cout << "test3\n";
  return 0;
}
