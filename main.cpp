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
  graph g2dfs = g2.dfs();

  std::cout << "g2 dfs\n";

  g2dfs.print();

  std::cout << "finish\n";
  std::cout << "g2 bfs\n";
  graph g2bfs = g2.bfs();

  g2bfs.print();
  std::cout << "finish\n";
  Read<graph> readGraph("graph1.txt");
  graph mygraph = readGraph.getGraph();
  mygraph.print();

  mygraph.removeVertex(0);
  mygraph.print();

  Read<graph> readGraph2("graph2.txt");
  graph mygraph2 = readGraph2.getGraph();
  mygraph2.print();

  mygraph2.removeVertex(0);
  mygraph2.print();

  std::cout << mygraph2.isConnected() << '\n';
  std::cout << mygraph2.isBipartite() << '\n';

  mygraph2.findNode(1);
  mygraph2.findEdge(1,3);
  mygraph2.removeEdge(1,3);
  mygraph2.print();

  return 0;
}
