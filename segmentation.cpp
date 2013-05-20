#include "segmentation.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;


Segmentation::Segmentation(){}

vector<Graph> Segmentation::getComponents(){
	return components;
}

void Segmentation::addComponent(Graph g){
	components.push_back(g);
}

bool Segmentation::areDisjoint(Graph g1, Graph g2, int k){
	bool areDisjoint = false;

	return areDisjoint;
}

Graph Segmentation::joinComponents(Graph g1, Graph g2){

}

int Segmentation::getDiffComponents(Graph g1, Graph g2, Graph& graph){
	int min = 5000;

	int index2 = g2.getIndex();

	vector<Node>& g1Nodes = g1.getNodes();
	vector<Node>& graphNodes = graph.getNodes();
	vector<Edge>& graphEdges = graph.getEdges();

	for(int i = 0; i < g1Nodes.size(); i++){
		vector<char>& g1NodeEdges = g1Nodes[i].getEdges();

		for(int j = 0; j < g1NodeEdges; j++){
			Edge g1edg = g1NodeEdges[graphEdges[j]];
			vector<int> nodes  = g1edg.getNodes();
			int index;
			if(g1Nodes[i].getIndex() != nodes[0]){
				index = graphNodes[nodes[0]].getIndex();
			}
			else{
				index = graphNodes[nodes[1]].getIndex();
			}

			if(index == index2)
				if(g1edg.getWeight() < min)
					min = g1edg.getWeight();
		}
	}

	return min;
}