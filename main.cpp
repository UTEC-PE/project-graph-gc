#include <GL/glut.h>
#include <iostream>

#include "graph.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[]) {
  Read<graph> readGraph("graph1.txt");
  graph mygraph =readGraph.getGraph();
  return 0;
}
