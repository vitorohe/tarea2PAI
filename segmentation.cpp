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
	size = 0;
	totalVal = 0;
}

int Component::getIndex() {
    return indexC;
}

void Component::setIndex(int ind) {
    indexC = ind;
}

void Component::setMaxEdgeMST(int val){
	maxEdgeMST = val;
}

int Component::getMaxEdgeMST(){
	return maxEdgeMST;
}

void Component::unionC(Node *x) {
	Node *y = root;
	if (x->rank > y->rank) {
		y->parent = x;  
		x->children->push_back(y); // adding y to x's children  
		root = x;  
	} else {
		x->parent = y;
		y->children->push_back(x); // adding y to x's children  
		root = y;  
	}

	if (x->rank == y->rank) {  
		(y->rank)++;
	}
}

int Component::getSize() {
	return size;
}

void Component::setSize(int val) {
	size = val;
}

int Component::getTotalVal(){
	return totalVal;
}

void Component::addToTotalVal(int val){
	totalVal += val;
}

void Component::printComponent(Node *root){
	cout<<"Node index "<<root->getIndex()<<endl;
	for(int i = 0; i < root->children->size(); i++){
		printComponent((*root->children).at(i));
	}
}

/*------------------------------------------------------------------------------*/

Segmentation::Segmentation(){}

vector<Component *> Segmentation::getComponents(){
	return components;
}

void Segmentation::addComponent(Component *c){
    components.push_back(c);
}

bool Segmentation::areDisjoint(Component *c1, Component *c2, float k, Graph graph){
	// cout<<"diff between "<<c1->getIndex()<<" y "<<c2->getIndex()<<endl;
	int v1 = getDiffComponents(c1,c2,graph);
	// cout<<"diff between sñwkirghvor"<<endl;
	float v2 = getMInt(c1,c2,k, graph);
	// cout<<"diff "<<v1<<", mint "<<v2<<endl;
	return (v1 - v2) > 20;
}

float Segmentation::getMInt(Component *c1, Component *c2, float k, Graph graph){
	float t1 = (float)k/c1->getSize();
	float t2 = (float)k/c2->getSize();
	int maxEdgeMST1 = 0;
	// cout<<"mint 1"<<endl;
	// cout<<"mst1 c1 "<<c1.getMaxEdgeMST()<<endl;
	// cout<<"mst2 c2 "<<c2.getMaxEdgeMST()<<endl;
	vector<Node *>* graphNodes = graph.getNodes();
    vector<Edge *>* graphEdges = graph.getEdges();
    // cout<<"mint 2"<<endl;
	if(c1->getMaxEdgeMST() == -1){
		// cout<<"mint 2.1"<<endl;
		maxEdgeMST1 = getMaxEdgeMST(c1->root, graphNodes, graphEdges, c1->getIndex());
		// cout<<"mint 2.2"<<endl;
		c1->setMaxEdgeMST(maxEdgeMST1);
	}
	else{
		// cout<<"no recalculando c1"<<endl;
		maxEdgeMST1 = c1->getMaxEdgeMST();
	}
	// cout<<"mint 3"<<endl;
	int maxEdgeMST2 = 0;
	if(c2->getMaxEdgeMST() == -1){
		maxEdgeMST2 = getMaxEdgeMST(c2->root, graphNodes, graphEdges, c2->getIndex());
		c2->setMaxEdgeMST(maxEdgeMST2);
	}
	else{
		// cout<<"no recalculando c2"<<endl;
		maxEdgeMST2 = c2->getMaxEdgeMST();
	}
	// cout<<"mint 4"<<endl;
	// cout<<"maxEdgeMST1 "<<maxEdgeMST1<<", maxEdgeMST2 "<<maxEdgeMST2<<", t1 "<<t1<<", t2 "<<t2<<endl;
	return min(maxEdgeMST1 + t1, maxEdgeMST2 + t2);
}

int Segmentation::getDiffComponents(Component *c1, Component *c2, Graph graph){
	int min = 2000;
    int index2;
    int indexEdge = -1;
    vector<int> c1Nodes;
	vector<Node *>* graphNodes = graph.getNodes();
    vector<Edge *>* graphEdges = graph.getEdges();
    Node *root1 = c1->root;
    Node *root2 = c2->root;
    Node *root;
	if(c1->getSize() > c2->getSize()){
		index2 = c1->getIndex();
		root = root2;
	}else{
		index2 = c2->getIndex();
		root = root1;
	}
	// cout<<"Antes de minChildren"<<endl;
	min = minChildren(root,index2,graphNodes,graphEdges);
	return min;
}
int Segmentation::minChildren(Node *root, int index2, vector<Node *>* graphNodes, vector<Edge *>* graphEdges){

    int min = 3000;
    vector<pair<int,int> > rEdges = root->getEdges();

    for(int j = 0; j < rEdges.size(); j++){
        Edge *c1edg = (*graphEdges)[rEdges[j].first];
        // cout<<"edge index in minchildren"<<c1edg->getIndex()<<endl;
        // if(c1edg->isSelected())
        // 	continue;
        // else{
        // 	c1edg->setSelected(true);
        // }
        // cout<<"\tedge_index> "<<c1edg.getIndex()<<" weight "<<c1edg.getWeight()<<endl;
        vector<int> nodes  = c1edg->getNodes();
        int indexC;
        if(root->getIndex() != nodes[0]){
			indexC = (*graphNodes)[nodes[0]]->findN((*graphNodes)[nodes[0]])->getIndexComponent();
		}
		else{
			indexC = (*graphNodes)[nodes[1]]->findN((*graphNodes)[nodes[1]])->getIndexComponent();
		}
		// cout<<"\tindexC "<<indexC<<endl;
		if(indexC == index2){
			// cout<<"\t\tjoin components"<<endl;
            if(c1edg->getWeight() < min) {
            	// indexEdge = c1edg->getIndex();
            	min = c1edg->getWeight();
            }
            break;
        }
	}
	// if(root->children == NULL)
	// 	cout<<"es nulli"<<endl;
	// cout<<"revisar hijos"<<endl;
	int minChildren_ = 4000;
	// if(root->children != NULL){
		for(int i = 0; i < root->children->size(); i++) {
			// cout<<"revisar hijo "<<i<<endl;
			Node *child = (*(root->children)).at(i);
    		minChildren_ = minChildren(child, index2, graphNodes, graphEdges);
    		if(minChildren_ < min)
    			min = minChildren_;
		}
	// }
	// cout<<"index Edge "<<graphEdges[indexEdge].getNodes()[0]<<","<<graphEdges[indexEdge].getNodes()[1]<<endl;
	// cout<<"min diff"<<min<<endl;
	return min;
}


int Segmentation::getMaxEdgeMST(Node *root, vector<Node *>* graphNodes, vector<Edge *>* graphEdges, int index){

	int maxComponent = 0;

	// if(root->children->size() == 0)
	// 	return maxComponent;
	// cout<<"mst 1"<<endl;
	vector<pair<int,int> > nEdges = root->getEdges();

	int minEdgeIndex = 0;
	int minWeightEdge = 5000;
	// cout<<"mst 2"<<endl;
	for(int j = 0; j < nEdges.size(); j++){
		Edge *edge = (*graphEdges)[nEdges[j].first];

        vector<int> nodes = edge->getNodes();
        int indexC;
        if(root->getIndex() != nodes[0]){
			indexC = (*graphNodes)[nodes[0]]->findN((*graphNodes)[nodes[0]])->getIndexComponent();
		}
		else{
			indexC = (*graphNodes)[nodes[1]]->findN((*graphNodes)[nodes[1]])->getIndexComponent();
		}

		if(!edge->isSelected() && indexC == index) {
			minWeightEdge = edge->getWeight();
			minEdgeIndex = edge->getIndex();
			edge->setSelected(true);
			break;
		}
	}
	// cout<<"mst 3"<<endl;
	if(minWeightEdge != 5000 && minWeightEdge > maxComponent)
		maxComponent = minWeightEdge;
	// cout<<"node index "<<root->getIndex()<<", maxComponent "<<maxComponent<<endl;
	//revisando hijos
	int minChildren;
	// if(root->children != NULL){
		for(int i = 0; i < root->children->size(); i++) {
			Node *child = (*(root->children)).at(i);
			// cout<<"\tchild index mst "<<child->getIndex()<<endl;
    		minChildren = getMaxEdgeMST(child, graphNodes, graphEdges, index);
    		if(minChildren > maxComponent)
    			maxComponent = minChildren;
		}
	// }
	
	return maxComponent;
}

void Segmentation::freeSegmentation() {
	vector<Component *>::iterator Iterator;
	for(Iterator = components.begin(); Iterator != components.end(); Iterator++)
	{
		delete *Iterator;
	}
	components.clear();
}

Pair::Pair(){}