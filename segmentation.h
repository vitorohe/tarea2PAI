#include "graph.h"
#include <opencv2/core/core.hpp>

using namespace cv;

class Component
{

public:
	Node *root;
	int indexC;
	int maxEdgeMST;
	int size;
	int totalVal;

public:
	Component();
	void unionC(Node *x) ;
	vector<int> getNodes();
	void setIndex(int ind);
	int getIndex();
	void setMaxEdgeMST(int val);
	int getMaxEdgeMST();
	int getSize();
	void setSize(int val);
	int getTotalVal();
	void addToTotalVal(int val);
	static void printComponent(Node *root);

};

class Segmentation
{

vector<Component *> components;

public:
	Segmentation();
	vector<Component *> getComponents();
	void addComponent(Component *c);
	bool areDisjoint(Component *c1, Component *c2, float k, Graph graph);
	static float getMInt(Component *c1, Component *c2, float k, Graph graph);
	static int getDiffComponents(Component *c1, Component *c2, Graph graph);
	static int minChildren(Node *root, int index2, vector<Node *>* graphNodes, vector<Edge *>* graphEdges);
	static int getMaxEdgeMST(Node *root, vector<Node *>* graphNodes, vector<Edge *>* graphEdges, int index);
	void freeSegmentation();
};