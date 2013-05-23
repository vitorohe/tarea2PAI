#include "segmentation.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;

Component::Component(){
	setMaxEdgeMST(-1);
}

vector<int> Component::getNodes() {
    return nodes;
}

void Component::addNode(int node) {
    nodes.push_back(node);
}

int Component::getIndex() {
    return index;
}

void Component::setIndex(int ind) {
    index = ind;
}

void Component::setMaxEdgeMST(int val){
	maxEdgeMST = val;
}

int Component::getMaxEdgeMST(){
	return maxEdgeMST;
}

void Component::setNodes(vector<int> nodesC){
	nodes = nodesC;
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
	vector<int> nodes2 = c2.getNodes();

	// for(int i = 0; i < nodes.size(); i++) {
	// 	c1.addNodes(nodes[i]);
	// }
	vector<int> nodes1 = c1.getNodes();
	nodes1.reserve(nodes1.size() + distance(nodes2.begin(),nodes2.end()));
	nodes1.insert(nodes1.end(),nodes2.begin(),nodes2.end());
	c1.setNodes(nodes1);
    c1.setMaxEdgeMST(-1);
    return c1;
}

float Segmentation::getMInt(Component& c1, Component& c2, float k, Graph& graph){
	float t1 = (float)k/c1.getNodes().size();
	float t2 = (float)k/c2.getNodes().size();
	int maxEdgeMST1 = 0;
	// cout<<"mst1 c1 "<<c1.getMaxEdgeMST()<<endl;
	// cout<<"mst2 c2 "<<c2.getMaxEdgeMST()<<endl;
	if(c1.getMaxEdgeMST() == -1)
		maxEdgeMST1 = getMaxEdgeMST(c1, graph);
	else{
		// cout<<"no recalculando c1"<<endl;
		maxEdgeMST1 = c1.getMaxEdgeMST();
	}

	int maxEdgeMST2 = 0;
	if(c2.getMaxEdgeMST() == -1)
		maxEdgeMST2 = getMaxEdgeMST(c2, graph);
	else{
		// cout<<"no recalculando c2"<<endl;
		maxEdgeMST2 = c2.getMaxEdgeMST();
	}
	// cout<<"maxEdgeMST1 "<<maxEdgeMST1<<", maxEdgeMST2 "<<maxEdgeMST2<<", t1 "<<t1<<", t2 "<<t2<<endl;
	return min(maxEdgeMST1 + t1, maxEdgeMST2 + t2);
}

int Segmentation::getDiffComponents(Component& c1, Component& c2, Graph graph){
	int min = 5000;
    int index2;
    int indexEdge = -1;
    vector<int> c1Nodes;

	if(c1.getNodes().size() > c2.getNodes().size()){
		index2 = c1.getIndex();
		c1Nodes = c2.getNodes();
	}else{
		index2 = c2.getIndex();
		c1Nodes = c1.getNodes();
	}


    vector<Node> graphNodes = graph.getNodes();
    vector<Edge> graphEdges = graph.getEdges();
    // cout<<"Components "<<c1.getIndex()<<", "<<c2.getIndex()<<endl;
    for(int i = 0; i < c1Nodes.size(); i++){
        Node c1Node = graphNodes[c1Nodes[i]];
        vector<pair<int,int> > c1NodeEdges = c1Node.getEdges();

        for(int j = 0; j < c1NodeEdges.size(); j++){
            Edge& c1edg = graphEdges[c1NodeEdges[j].first];
            if(c1edg.isSelected())
            	continue;
            else{
            	c1edg.setSelected(true);
            }
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
                if(c1edg.getWeight() < min) {
                	indexEdge = c1edg.getIndex();
                    min = c1edg.getWeight();
                }
                break;
            }
		}
	}
	// cout<<"index Edge "<<graphEdges[indexEdge].getNodes()[0]<<","<<graphEdges[indexEdge].getNodes()[1]<<endl;
	// cout<<"min diff"<<min<<endl;
	return min;
}


int Segmentation::getMaxEdgeMST(Component& c, Graph graph1){

	Graph graph = graph1;

	int maxComponent = 0;

	vector<int> cNodes = c.getNodes();

	if(cNodes.size() == 1)
		return maxComponent;

	vector<Node> graphNodes = graph.getNodes();
	vector<Edge> graphEdges = graph.getEdges();

	for(int i = 0; i < cNodes.size(); i++){
		Node node = graphNodes[cNodes[i]];
		vector<pair<int,int> > nEdges = node.getEdges();

		int minEdgeIndex = 0;
		int minWeightEdge = 5000;

		for(int j = 0; j < nEdges.size(); j++){
			Edge& edge = graphEdges[nEdges[j].first];

            vector<int> nodes  = edge.getNodes();
            int indexC;
            if(node.getIndex() != nodes[0]){
				indexC = graphNodes[nodes[0]].getIndexComponent();
			}
			else{
				indexC = graphNodes[nodes[1]].getIndexComponent();
			}

			if(!edge.isSelected() && indexC == c.getIndex()) {
				minWeightEdge = edge.getWeight();
				minEdgeIndex = edge.getIndex();
				edge.setSelected(true);
				break;
			}
		}

		if(minWeightEdge > maxComponent)
			maxComponent = minWeightEdge;
	}
	c.setMaxEdgeMST(maxComponent);
	return maxComponent;
}