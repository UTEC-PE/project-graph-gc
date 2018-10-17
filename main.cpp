#include <GL/glut.h>
#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {

  graph g(4, true);
  g.addVertex(13.0, 6.0);
  g.addVertex(22.0, 6.0);

  g.addVertex(23.0, 2.0);
  g.addVertex(42.0, 1.0);

  g.addEdge(0, 1, 1, 1);
  g.addEdge(1, 0, 2, 1);
  g.addEdge(1, 3, 5, 1);
  g.addEdge(1, 2, 4, 1);
  g.kruskal();
  return 0;
}
