#include "graph.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <typeinfo>

using namespace std;
using namespace cv;


Node::Node(){}

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

int Node::getIndexGraph(){
	return indexGraph;
}

void Node::setIndexGraph(int ind){
	indexGraph = ind;
}

vector<int> Node::getEdges() {
	return edges;
}

void Node::addEdge(int edge) {
	edges.push_back(edge);
}



Edge::Edge(){}

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
			nodes.push_back(node);
		}
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			int vertex_a = (int)input.at<uchar>(i,j);
			int index_a = j+width*i;
			Node& node_a = nodes[index_a];
			int vertex_b;
			int index_b;
			//right
			if(j < width-1) {
				Edge edge1 = Edge();
				vertex_b = (int)input.at<uchar>(i,j+1);
				edge1.setWeight(abs(vertex_a - vertex_b));
				index_b = (j + 1) + i*width;
				edge1.addNode(index_a);
				edge1.addNode(index_b);

				edges.push_back(edge1);
				edge1.setIndex(edges.size()-1);

				Node& node_b = nodes[index_b];
				node_a.addEdge(edges.size()-1);
				node_b.addEdge(edges.size()-1);
				node_a.addNeighbor(index_b);
				node_b.addNeighbor(index_a);

			}

			//right and down
			if(j < width-1 && i < height-1) {
				Edge edge2 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j+1);
				edge2.setWeight(abs(vertex_a - vertex_b));
				index_b = (j + 1) + (i + 1)*width;
				edge2.addNode(index_a);
				edge2.addNode(index_b);

				edges.push_back(edge2);
				edge2.setIndex(edges.size()-1);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edges.size()-1);
				node_b.addEdge(edges.size()-1);
				node_a.addNeighbor(index_b);
				node_b.addNeighbor(index_a);

			}

			//down
			if(i < height-1) {
				Edge edge3 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j);
				edge3.setWeight(abs(vertex_a - vertex_b));
				index_b = j + (i + 1)*width;
				edge3.addNode(index_a);
				edge3.addNode(index_b);

				edges.push_back(edge3);
				edge3.setIndex(edges.size()-1);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edges.size()-1);
				node_b.addEdge(edges.size()-1);
				node_a.addNeighbor(index_b);
				node_b.addNeighbor(index_a);

			}

			//left and down
			if(j > 0 && i < height-1) {
				Edge edge4 = Edge();
				vertex_b = (int)input.at<uchar>(i+1,j-1);
				edge4.setWeight(abs(vertex_a - vertex_b));
				index_b = (j - 1) + (i + 1)*width;
				edge4.addNode(index_a);
				edge4.addNode(index_b);

				edges.push_back(edge4);
				edge4.setIndex(edges.size()-1);
				Node& node_b = nodes[index_b];
				node_a.addEdge(edges.size()-1);
				node_b.addEdge(edges.size()-1);
				node_a.addNeighbor(index_b);
				node_b.addNeighbor(index_a);

			}

		}
	}
}

void Graph::printGraph() {
	Node node;
	vector<int> edgesNode;
	for (int i = 0; i < nodes.size(); ++i)
	{
		node = nodes[i];
		cout<<"Node "<<node.getIndex()<<endl;

		edgesNode = node.getEdges();
		for (int j = 0; j < edgesNode.size(); ++j)
		{
			Edge edge = edges[edgesNode[j]];
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