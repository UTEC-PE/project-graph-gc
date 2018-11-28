#include <iostream>
#include <thread>
#include <mutex>
#include "graph.h"
#include "read.h"

using namespace std;

#define NUM_THREADS 4
mutex mtx;

void printA_estrella(graph* readGraph, int tagInicio, int tagFinal, string hilo){
  mtx.lock();
  std::cout << "Hilo: " << hilo << "----------------------------------------" << '\n';
  try{
    readGraph->aEstrella(tagInicio, tagFinal).pprint();
  }
  catch(char const*) {std::cout << "No existe ruta posible-------------------\n" << '\n';}
	//std::cout << '\n';
  mtx.unlock();
}

int main(int argc, char *argv[]) {
	thread threads[NUM_THREADS];
  Read<graph> readGraph("GRAFO3.txt");

  graph *g = readGraph.getGraph();
  //g->pprint();

	threads[0] = thread(printA_estrella,g,0,3, "0");
  threads[1] = thread(printA_estrella,g,0,5, "1");
  threads[2] = thread(printA_estrella,g,1,3, "2");
  threads[3] = thread(printA_estrella,g,1,4, "3");
	for(int i = 0; i < NUM_THREADS;i++){threads[i].join();}

  delete g;

	return 0;
}
