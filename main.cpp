#include <GL/glut.h>
#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char *argv[]) {

  graph g(2, true);
  g.addVertex(13.0, 6.0);
  g.addVertex(22.0, 6.0);

  g.addEdge(0, 1, 5, 1);
  g.addEdge(1, 0, 5, 1);
  return 0;
}
