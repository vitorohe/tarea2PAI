#include "graph.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;


Node::Node(){
	setSelected(false);
}

vector<int> Node::getNeighbors(){
	return neighbors;
}

void Node::addNeighbor(int neighbor){
	neighbors.push_back(neighbor);
}

int Node::getIndex(){
	return index;
}

void Node::setIndex(int ind){
	index = ind;
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

void Node::addEdge(int index,int weight) {
	pair<int,int> p;
	p.first = index;
	p.second = weight;
	edges.push_back(p);
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
	return index;
}

void Edge::setIndex(int ind){
	index = ind;
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

bool Edge::compareEdges(Edge edge1, Edge edge2){
	return edge1.getWeight() < edge2.getWeight();
}

void Edge::setSelected(bool sel){
	selected = sel;
}

bool Edge::isSelected(){
	return selected;
}

Graph::Graph(){}

void Graph::createGraph(Mat input) {
	int width = input.size().width;
	int height = input.size().height;

	int k = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Node node = Node();
			node.setIndex(k++);
			node.setValue((int)input.at<uchar>(i,j));
			nodes.push_back(node);
		}
	}

	k = 0;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int vertex_a = (int)input.at<uchar>(i,j);
			int index_a = j+width*i;
			Node& node_a = nodes[index_a];
			int vertex_b;
			int index_b;
			int weight;
			//right
			if(j < width-1) {
				Edge edge1 = Edge();
				vertex_b = (int)input.at<uchar>(i,j+1);
				weight = abs(vertex_a - vertex_b);
				edge1.setWeight(weight);
				index_b = (j + 1) + i*width;
				edge1.addNode(index_a);
				edge1.addNode(index_b);

				edge1.setIndex(k++);
				edges.push_back(edge1);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edge1.getIndex(),weight);
				node_b.addEdge(edge1.getIndex(),weight);

			}

			//right and down
			if(j < width-1 && i < height-1) {
				Edge edge2 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j+1);
				weight = abs(vertex_a - vertex_b);
				edge2.setWeight(weight);
				index_b = (j + 1) + (i + 1)*width;
				edge2.addNode(index_a);
				edge2.addNode(index_b);

				edge2.setIndex(k++);
				edges.push_back(edge2);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edge2.getIndex(),weight);
				node_b.addEdge(edge2.getIndex(),weight);

			}

			//down
			if(i < height-1) {
				Edge edge3 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j);
				weight = abs(vertex_a - vertex_b);
				edge3.setWeight(weight);
				index_b = j + (i + 1)*width;
				edge3.addNode(index_a);
				edge3.addNode(index_b);

				edge3.setIndex(k++);
				edges.push_back(edge3);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edge3.getIndex(),weight);
				node_b.addEdge(edge3.getIndex(),weight);

			}

			//left and down
			if(j > 0 && i < height-1) {
				Edge edge4 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j-1);
				weight = abs(vertex_a - vertex_b);
				edge4.setWeight(weight);
				index_b = (j - 1) + (i + 1)*width;
				edge4.addNode(index_a);
				edge4.addNode(index_b);

				edge4.setIndex(k++);
				edges.push_back(edge4);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edge4.getIndex(),weight);
				node_b.addEdge(edge4.getIndex(),weight);

			}
		}
	}

	for (int i = 0; i < nodes.size(); ++i)
	{
		Node& node = nodes[i];
		vector<pair<int,int> > nEdges = node.getEdges();
		sort(nEdges.begin(),nEdges.end(),compareEdges1);
		node.setEdges(nEdges);
	}

}

void Graph::printGraph() {
	Node node;
	vector<pair<int,int> > edgesNode;
	for (int i = 0; i < nodes.size(); ++i)
	{
		node = nodes[i];
		cout<<"Node "<<node.getIndex()<<endl;

		edgesNode = node.getEdges();
		for (int j = 0; j < edgesNode.size(); ++j)
		{
			Edge edge = edges[edgesNode[j].first];
			vector<int> nodesEdge = edge.getNodes();
			if(nodesEdge[0] != i) {
				cout<<"\t"<<"Neighbor "<<nodesEdge[0]<<" weight "<<edge.getWeight()<<endl;
			} else {
				cout<<"\t"<<"Neighbor "<<nodesEdge[1]<<" weight "<<edge.getWeight()<<endl;
			}
		}
	}
}

vector<Node> Graph::getNodes(){
	return nodes;
}

vector<Edge> Graph::getEdges(){
	return edges;
}

int Graph::getIndex(){
	return index;
}

void Graph::setIndex(int ind){
	index = ind;
}

void Graph::setNodeIndexComponent(int n, int k){
	Node& node = nodes[n];
	node.setIndexComponent(k);
}

bool Graph::compareEdges1( const pair<int, int>& i, const pair<int, int>& j ) {
	return i.second < j.second;
}