#include "graph.h"
#include <opencv2/core/core.hpp>

using namespace cv;

class Component
{

vector<int> nodes;
int index;

public:
	Component();
	vector<int> getNodes();
	void addNodes(int node);
	void setIndex(int ind);
	int getIndex();
};

class Segmentation
{

vector<Component> components;

public:
	Segmentation();
	vector<Component> getComponents();
	void addComponent(Component c);
	bool areDisjoint(Component& c1, Component& c2, float k, Graph& graph);
	Component joinComponents(Component c1, Component c2);
	static float getMInt(Component& c1, Component& c2, float k, Graph& graph);
	static int getDiffComponents(Component& c1, Component& c2, Graph& graph);
	static int getMaxEdgeMST(Component& c, Graph& graph);
};