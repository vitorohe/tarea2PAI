#include <opencv2/core/core.hpp>

using namespace cv;

class Node
{
vector<int> neighbors;
vector<int> edges;
int index;
int indexComponent;
bool selected;
int value;
public:
	Node();
	vector<int> getNeighbors();
	void addNeighbor(int neighbor);
	int getIndex();
	void setIndex(int ind);
	int getIndexComponent();
	void setIndexComponent(int ind);
	void addEdge(int edge);
	vector<int> getEdges();
	void setSelected(bool sel);
	bool isSelected();
	void setValue(int val);
	int getValue();
};


class Edge
{
int index;
int weight;
vector<int> nodes;
bool selected;
public:
	Edge();
	int getIndex();
	void setIndex(int ind);
	int getWeight();
	void setWeight(int w);
	void addNode(int node);
	vector<int> getNodes();
	static bool compareEdges(Edge edge1, Edge edge2);
	void setSelected(bool sel);
	bool isSelected();
};


class Graph
{

vector<Node> nodes;
vector<Edge> edges;
int index;

public:
	Graph();
	void createGraph(Mat input);
	void printGraph();
	vector<Node> getNodes();
	vector<Edge> getEdges();
	int getIndex();
	void setIndex(int ind);
	void setNodeIndexComponent(int node, int k);
	
};