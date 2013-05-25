#include <opencv2/core/core.hpp>
#include <list>

using namespace cv;
using namespace std;

class Pair{
public:
	int first;
	int second;
	Pair();
};

class Edge
{
int indexE;
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
	static bool compareEdges(Edge *edge1, Edge *edge2);
	void setSelected(bool sel);
	bool isSelected();
};

class Node
{
public:
	Node *parent;
	vector<Node *> *children;

	vector<pair<int,int> > edges;
	int indexN;
	int rank;
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
	void addEdge(int i, int j);
	void setEdges(vector<pair<int,int> > edgesN);
	vector<pair<int,int> > getEdges();
	void setSelected(bool sel);
	bool isSelected();
	void setValue(int val);
	int getValue();
	Node(int i);
	Node(Node *p, int i);
	Node *findN(Node *node);
};


class Graph
{

vector<Node *> *nodes;
vector<Edge *> *edges;

public:
	Graph(int height, int width);
	void createGraph(Mat input);
	void printGraph();
	vector<Node *>* getNodes();
	vector<Edge *>* getEdges();
	void setNodeIndexComponent(int node, int k);
	static bool compareEdges1( pair<int,int> i, pair<int,int> j );
	template <class C>
	static void freeClear( C *cntr );
	void freeGraph();
	
};