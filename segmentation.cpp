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

bool Segmentation::areDisjoint(Component& c1, Component& c2, float k, Graph& graph){
	int v1 = getDiffComponents(c1,c2,graph);
	float v2 = getMInt(c1,c2,k, graph);
	// cout<<"diff "<<v1<<", mint "<<v2<<endl;
	return v1 > v2;
}

Component Segmentation::joinComponents(Component c1, Component c2){
	vector<int> nodes = c2.getNodes();

	for(int i = 0; i < nodes.size(); i++) {
		c1.addNodes(nodes[i]);
	}

    return c1;
}

float Segmentation::getMInt(Component& c1, Component& c2, float k, Graph& graph){
	float t1 = (float)k/c1.getNodes().size();
	float t2 = (float)k/c2.getNodes().size();
	int maxEdgeMST1 = getMaxEdgeMST(c1, graph);
	int maxEdgeMST2 = getMaxEdgeMST(c2, graph);
	// cout<<"maxEdgeMST1 "<<maxEdgeMST1<<", maxEdgeMST2 "<<maxEdgeMST2<<", t1 "<<t1<<", t2 "<<t2<<endl;
	return min(maxEdgeMST1 + t1, maxEdgeMST2 + t2);
}

int Segmentation::getDiffComponents(Component& c1, Component& c2, Graph& graph){
	int min = 5000;

    int index2 = c2.getIndex();
    vector<int> c1Nodes = c1.getNodes();
    vector<Node> graphNodes = graph.getNodes();
    vector<Edge> graphEdges = graph.getEdges();
    // cout<<"Components "<<c1.getIndex()<<", "<<c2.getIndex()<<endl;
    for(int i = 0; i < c1Nodes.size(); i++){
        Node c1Node = graphNodes[c1Nodes[i]];
        vector<int> c1NodeEdges = c1Node.getEdges();

        for(int j = 0; j < c1NodeEdges.size(); j++){
            Edge c1edg = graphEdges[c1NodeEdges[j]];
            // cout<<"\tedge_index> "<<c1edg.getIndex()<<" weight "<<c1edg.getWeight()<<endl;
            vector<int> nodes  = c1edg.getNodes();
            int indexC;
            if(c1Node.getIndex() != nodes[0]){
				indexC = graphNodes[nodes[0]].getIndexComponent();
			}
			else{
				indexC = graphNodes[nodes[1]].getIndexComponent();
			}
			// cout<<"\tindexC "<<indexC<<endl;
			if(indexC == index2){
				// cout<<"\t\tjoin components"<<endl;
                if(c1edg.getWeight() < min)
                    min = c1edg.getWeight();
            }
		}
	}
	// cout<<"min diff"<<min<<endl;
	return min;
}


int Segmentation::getMaxEdgeMST(Component& c, Graph& graph1){

	Graph graph = graph1;

	int maxComponent = 0;

	vector<int> cNodes = c.getNodes();

	if(cNodes.size() == 1)
		return maxComponent;

	vector<Node> graphNodes = graph.getNodes();
	vector<Edge> graphEdges = graph.getEdges();

	for(int i = 0; i < cNodes.size(); i++){
		Node node = graphNodes[cNodes[i]];

		vector<int> nEdges = node.getEdges();

		int minEdgeIndex = 0;
		int minWeightEdge = 5000;

		for(int j = 0; j < nEdges.size(); j++){
			Edge edge = graphEdges[nEdges[j]];

			if(!edge.isSelected())
				if(edge.getWeight() < minWeightEdge){
					minWeightEdge = edge.getWeight();
					minEdgeIndex = edge.getIndex();
				}

		}

		graphEdges[minEdgeIndex].setSelected(true);

		if(minWeightEdge > maxComponent)
			maxComponent = minWeightEdge;
	}

	return maxComponent;
}