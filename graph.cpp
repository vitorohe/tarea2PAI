#include "graph.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;

Node::Node(int i) {
	rank = 0;
	parent = NULL;
	indexN = i;
	setSelected(false);
	children = new vector<Node *>;
	// edges.reserve(8);
}
Node::Node(Node *p, int i) {
	rank = 0;
	parent = p;
	indexN = i;
	setSelected(false);
	children = new vector<Node *>;
	// edges.reserve(8);
}
Node *Node::findN(Node *node) {
	// Node *parent;
	// cout<<"find "<<node->indexN<<endl;
	if (node->parent != node) {  
		// cout<<"find if"<<endl;
		// cout<<"find p"<<node->parent->indexN<<endl;
		node->parent = findN(node->parent);  
	}
	return node->parent;
}

int Node::getIndex(){
	return indexN;
}

void Node::setIndex(int ind){
	indexN = ind;
}

int Node::getIndexComponent(){
    return indexComponent;
}

void Node::setIndexComponent(int ind){
    indexComponent = ind;
}

vector<pair<int,int> > Node::getEdges() {
	return edges;
}

void Node::addEdge(int ind,int weight) {
	pair<int,int> p;
	// Pair p = Pair();
	p.first = ind;
	// cout<<"\t"<<p.first<<", "<<ind<<endl;
	p.second = weight;
	// cout<<"\t"<<p.second<<endl;
	// if(index < 0)
	// 	cout<<"negative index"<<endl;
	edges.push_back(p);
	// cout<<"\tgugh "<<edges.back().first<<endl;
}

void Node::setEdges(vector<pair<int,int> > edgesN){
	edges = edgesN;
}

void Node::setSelected(bool sel){
	selected = sel;
}

bool Node::isSelected(){
	return selected;
}

void Node::setValue(int val){
	value = val;
}

int Node::getValue(){
	return value;
}

/*-----------------------------------------------------------------*/

Edge::Edge(){
	setSelected(false);
}

int Edge::getIndex(){
	return indexE;
}

void Edge::setIndex(int ind){
	indexE = ind;
}

int Edge::getWeight(){
	return weight;
}

void Edge::setWeight(int w){
	weight = w;
}

void Edge::addNode(int node){
	nodes.push_back(node);
}

vector<int> Edge::getNodes(){
	return nodes;
}

bool Edge::compareEdges(Edge *edge1, Edge *edge2){
	return edge1->getWeight() < edge2->getWeight();
}

void Edge::setSelected(bool sel){
	selected = sel;
}

bool Edge::isSelected(){
	return selected;
}

/*---------------------------------------------------------------------------*/


Graph::Graph(int h, int w){
	nodes = new vector<Node *> ;//(h*w);
	edges = new vector<Edge *> ;//(4*h*w - 3*(w+h) + 2);
	nodes->reserve(h*w);
	edges->reserve(4*h*w - 3*(w+h) + 2);
	// cout<<edges->size()<<endl;
	cout<<"Graph initialized"<<endl;
}

void Graph::createGraph(Mat input, int type) {
	int width = input.size().width;
	int height = input.size().height;

	int k = 0;
	int val = -1;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Node *node = new Node(k++);
			if(type == 1){
				Vec3b vecColor = input.at<Vec3b>(i,j);
				int val0 = (int)vecColor[0];
				int val1 = (int)vecColor[1];
				int val2 = (int)vecColor[2];
				Mat vals = (Mat_<int>(1,3)<<val0,val1,val2);
				val = (int)norm(vals,NORM_L2);
			}
			else if(type == 2){
				val = (int)input.at<uchar>(i,j);
			}
			node->setValue(val);
			nodes->push_back(node);
		}
	}
	cout<<"Nodes initialized"<<endl;
	k = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int vertex_a = (int)input.at<uchar>(i,j);
			Vec3b vA = input.at<Vec3b>(i,j);
			int index_a = j+width*i;
			Node *node_a = (*nodes)[index_a];
			int vertex_b;
			int index_b;
			int weight;

			// cout<<"\tEdge right"<<endl;
			//right
			if(j < width-1) {
				index_b = (j + 1) + i*width;
				Node *node_b = (*nodes)[index_b];
				vertex_b = node_b->getValue();
				if(type == 1){
					weight = abs(vertex_a - vertex_b);
				}
				else if(type == 2){
					Vec3b vB = input.at<Vec3b>(i,j+1);
					int val0 = (int)vA[0] - (int)vB[0];
					int val1 = (int)vA[1] - (int)vB[1];
					int val2 = (int)vA[2] - (int)vB[2];
					Mat vals = (Mat_<int>(1,3)<<val0,val1,val2);
					int val = (int)norm(vals,NORM_L2);
					weight = val;
				}

				Edge *edge1 = new Edge();
				edge1->setWeight(weight);

				edge1->addNode(index_a);
				edge1->addNode(index_b);
				edge1->setIndex(k++);
				// cout<<"edge k "<<k-1<<" index "<<edge.getIndex()<<endl;
				node_a->addEdge(k-1,weight);
				node_b->addEdge(k-1,weight);
				edges->push_back(edge1);
			}

			//right and down
			if(j < width-1 && i < height-1) {
				index_b = (j + 1) + (i + 1)*width;
				Node *node_b = (*nodes)[index_b];
				vertex_b = node_b->getValue();
				if(type == 1){
					weight = abs(vertex_a - vertex_b);
				}
				else if(type == 2){
					Vec3b vB = input.at<Vec3b>(i+1,j+1);
					int val0 = (int)vA[0] - (int)vB[0];
					int val1 = (int)vA[1] - (int)vB[1];
					int val2 = (int)vA[2] - (int)vB[2];
					Mat vals = (Mat_<int>(1,3)<<val0,val1,val2);
					int val = (int)norm(vals,NORM_L2);
					weight = val;
				}
				
				Edge *edge2 = new Edge();
				edge2->setWeight(weight);
				
				edge2->addNode(index_a);
				edge2->addNode(index_b);
				edge2->setIndex(k++);
				// cout<<"edge k "<<k-1<<" index "<<edge.getIndex()<<endl;
				node_a->addEdge(k-1,weight);
				node_b->addEdge(k-1,weight);
				edges->push_back(edge2);
			}

			//down
			if(i < height-1) {
				
				index_b = j + (i + 1)*width;
				Node *node_b = (*nodes)[index_b];
				vertex_b = node_b->getValue();
				if(type == 1){
					weight = abs(vertex_a - vertex_b);
				}
				else if(type == 2){
					Vec3b vB = input.at<Vec3b>(i+1,j);
					int val0 = (int)vA[0] - (int)vB[0];
					int val1 = (int)vA[1] - (int)vB[1];
					int val2 = (int)vA[2] - (int)vB[2];
					Mat vals = (Mat_<int>(1,3)<<val0,val1,val2);
					int val = (int)norm(vals,NORM_L2);
					weight = val;
				}
				
				Edge *edge3 = new Edge();
				edge3->setWeight(weight);
				
				edge3->addNode(index_a);
				edge3->addNode(index_b);
				edge3->setIndex(k++);
				// cout<<"edge k "<<k-1<<" index "<<edge.getIndex()<<endl;
				node_a->addEdge(k-1,weight);
				node_b->addEdge(k-1,weight);
				edges->push_back(edge3);
			}

			//left and down
			if(j > 0 && i < height-1) {
				index_b = (j - 1) + (i + 1)*width;
				Node *node_b = (*nodes)[index_b];
				vertex_b = node_b->getValue();
				if(type == 1){
					weight = abs(vertex_a - vertex_b);
				}
				else if(type == 2){
					Vec3b vB = input.at<Vec3b>(i+1,j-1);
					int val0 = (int)vA[0] - (int)vB[0];
					int val1 = (int)vA[1] - (int)vB[1];
					int val2 = (int)vA[2] - (int)vB[2];
					Mat vals = (Mat_<int>(1,3)<<val0,val1,val2);
					int val = (int)norm(vals,NORM_L2);
					weight = val;
				}
				
				Edge *edge4 = new Edge();
				edge4->setWeight(weight);
				
				edge4->addNode(index_a);
				edge4->addNode(index_b);
				edge4->setIndex(k++);
				// cout<<"edge k "<<k-1<<" index "<<edge.getIndex()<<endl;
				node_a->addEdge(k-1,weight);
				node_b->addEdge(k-1,weight);
				edges->push_back(edge4);
			}
		
			// for (int h = 0; h<edges->size(); h++)
		 //    {
		 //        int j = (*edges).at(h)->getIndex();
		 //        cout<<"\t\tedge "<<j<<endl;
		 //        // cout<<"edge "<<(*edgesG).at(i)->getIndex()<<" w "<<(*edgesG).at(i)->getWeight()<<endl;
		 //    }
		 //    cout<<"-----------------------------------------------------------------------------"<<endl;
			// break;
		}
		// break;
	}
	// cout<<edges->size()<<endl;
	// for (int i = 0; i<edges->size(); i++)
 //    {
 //        int j = (*edges).at(i)->getIndex();
 //        cout<<"edge "<<j<<endl;
 //        // cout<<"edge "<<(*edgesG).at(i)->getIndex()<<" w "<<(*edgesG).at(i)->getWeight()<<endl;
 //    }
	// cout<<edges->size()<<endl;
	for (int i = 0; i < nodes->size(); ++i)
	{
		Node *node = (*nodes)[i];
		vector<pair<int,int> > nEdges = node->getEdges();
		sort(nEdges.begin(),nEdges.end(),compareEdges1);
		node->setEdges(nEdges);
	}
	cout<<"Graph created with nodes"<<endl;
}

void Graph::printGraph() {
	Node *node;
	vector<pair<int,int> > edgesNode;
	for (int i = 0; i < nodes->size(); ++i)
	{
		node = (*nodes)[i];
		cout<<"Node "<<node->getIndex()<<endl;

		edgesNode = node->getEdges();
		for (int j = 0; j < edgesNode.size(); ++j)
		{
			Edge *edge = (*edges)[edgesNode[j].first];
			vector<int> nodesEdge = edge->getNodes();
			if(nodesEdge[0] != i) {
				cout<<"\t"<<"Neighbor "<<nodesEdge[0]<<" weight "<<edge->getWeight()<<endl;
			} else {
				cout<<"\t"<<"Neighbor "<<nodesEdge[1]<<" weight "<<edge->getWeight()<<endl;
			}
		}
	}
}

vector<Node *>* Graph::getNodes(){
	return nodes;
}

vector<Edge *>* Graph::getEdges(){
	return edges;
}

void Graph::setNodeIndexComponent(int n, int k){
	Node *node = (*nodes)[n];
	node->setIndexComponent(k);
}

bool Graph::compareEdges1( pair<int,int> i, pair<int,int> j ) {
	return i.second < j.second;
}

template <class C> 
void Graph::freeClear( C *cntr ) {
    for ( typename C::iterator it = cntr->begin(); it != cntr->end(); ++it ) {
    	delete *it;
    }
    cntr->clear();
}

void Graph::freeGraph(){
	freeClear(nodes);
	delete nodes;
	freeClear(edges);
	delete edges;
}