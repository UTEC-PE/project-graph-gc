//#include <GL/glut.h>
#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {
  graph g2(4, false);
  g2.addVertex(13.0, 6.0);
  g2.addVertex(22.0, 6.0);
  g2.addVertex(23.0, 2.0);
  g2.addVertex(62.0, 8.0);
  g2.addVertex(22.0, 3.0);
  g2.addVertex(41.0, 2.0);
  g2.addVertex(41.0, 2.0);

  g2.addEdge(0, 1, 1, 0);
  g2.addEdge(0, 3, 8, 0);
  g2.addEdge(0, 2, 4, 0);
  g2.addEdge(2, 4, 7, 0);
  g2.addEdge(1, 5, 2, 0);
  g2.addEdge(1, 2, 2, 0);
  g2.addEdge(3, 2, 5, 0);
  g2.addEdge(3, 4, 1, 0);
  g2.addEdge(5, 6, 5, 0);

  std::cout << "g2\n";
  g2.print();
  g2.prim(2);
  return 0;
}
