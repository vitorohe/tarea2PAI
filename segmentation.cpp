#include "segmentation.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;

Component::Component(){}

vector<int> Component::getNodes() {
    return nodes;
}

void Component::addNodes(int node) {
    nodes.push_back(node);
}

int Component::getIndex() {
    return index;
}

void Component::setIndex(int ind) {
    index = ind;
}

/*------------------------------------------------------------------------------*/

Segmentation::Segmentation(){}

vector<Component> Segmentation::getComponents(){
	return components;
}

void Segmentation::addComponent(Component c){
    components.push_back(c);
}

bool Segmentation::areDisjoint(Component c1, Component c2, int k){
	bool areDisjoint = false;

	return areDisjoint;
}

Component Segmentation::joinComponents(Component c1, Component c2){
    return c1;
}

int Segmentation::getDiffComponents(Component c1, Component c2, Graph& graph){
	int min = 5000;

    int index2 = c2.getIndex();

    vector<int> c1Nodes = c1.getNodes();
    vector<Node> graphNodes = graph.getNodes();
    vector<Edge> graphEdges = graph.getEdges();

    for(int i = 0; i < c1Nodes.size(); i++){
        Node c1Node = graphNodes[c1Nodes[i]];
        vector<int> c1NodeEdges = c1Node.getEdges();

        for(int j = 0; j < c1NodeEdges.size(); j++){
            Edge c1edg = graphEdges[c1NodeEdges[j]];
            vector<int> nodes  = c1edg.getNodes();
			int index;
            if(c1Node.getIndex() != nodes[0]){
				index = graphNodes[nodes[0]].getIndex();
			}
			else{
				index = graphNodes[nodes[1]].getIndex();
			}

			if(index == index2)
                if(c1edg.getWeight() < min)
                    min = c1edg.getWeight();
		}
	}

	return min;
}
