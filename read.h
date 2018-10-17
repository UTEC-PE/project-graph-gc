#ifndef READ_H
#define READ_H

#include "graph.h"
#include <fstream>
using namespace std;

/**
 * Clase de ayuda para leer el grafo desde un archivo,
 * no es necesario que la utilicen, podrían implementar su lector
 * desde su grafo o algún otro lado
 **/
template <typename G>
class Read {
	typedef typename G::N N;
	typedef typename G::E E;

private:
	graph readGraph;
	public:
		Read(char* file) {
			ifstream graphFile;
		  graphFile.open(file);
		  string lineGraphFile;
		  if (graphFile.is_open()) {
		    getline(graphFile,lineGraphFile);
		    int numberNodes = 0;
				bool dirigedGraph = false;
				readGraphInformation(numberNodes, dirigedGraph, lineGraphFile);
				graph tempGraph(numberNodes, dirigedGraph);
				this->readGraph = tempGraph;
		    for (int i = 0; i < numberNodes; i++) {
					N tag=i;
					float x=0, y=0;
					getline(graphFile,lineGraphFile);
		      readCoordinateTag(x,y,tag,lineGraphFile);
					this->readGraph.addVertex(tag,x,y);
		    }
		    getline(graphFile,lineGraphFile);
				while(getline(graphFile,lineGraphFile)) {
						N node1=0;
						N node2=0;
						E edgeWeight=0;
						bool dirigedEdge=false;
						readNodeInformation(node1,node2,edgeWeight,dirigedEdge,lineGraphFile);
						this->readGraph.addEdge(node1, node2, edgeWeight, dirigedEdge);
				}
		    graphFile.close();
		  }else throw("Unable to open the graph file");
    }

		void readCoordinateTag (float &xCoordinate, float &yCoordinate, N &tag, string line) {
			xCoordinate = stof(line.substr(0,line.find(" ")));
			line = line.substr(line.find(" ")+1);
			if (string::npos!=line.find(" ")) {
				yCoordinate = stof(line.substr(0,line.find(" ")));
				line = line.substr(line.find(" ")+1);
				tag = stoi(line);
			}else{
				yCoordinate = stof(line);
			}
		};

		void readNodeInformation (N &node1, N &node2, E &edgeWeight, bool &dirigedEdge, string lineGraphFile) {
			node1 = stoi(lineGraphFile.substr(0,lineGraphFile.find(" ")));
			lineGraphFile = lineGraphFile.substr(lineGraphFile.find(" ")+1);
			node2 = stoi(lineGraphFile.substr(0,lineGraphFile.find(" ")));
			lineGraphFile = lineGraphFile.substr(lineGraphFile.find(" ")+1);
			edgeWeight = stoi(lineGraphFile.substr(0,lineGraphFile.find(" ")));
			lineGraphFile = lineGraphFile.substr(lineGraphFile.find(" ")+1);
			dirigedEdge = stoi(lineGraphFile);
		};

		void readGraphInformation (int &numberNodes, bool &dirigedGraph, string lineGraphFile) {
			numberNodes = stoi(lineGraphFile.substr(0,lineGraphFile.find(" ")));
			lineGraphFile = lineGraphFile.substr(lineGraphFile.find(" ")+1);
			dirigedGraph = stoi(lineGraphFile);
		};

		graph getGraph() {
			return this->readGraph;
    }
};

#endif
