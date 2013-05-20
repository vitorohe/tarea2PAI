#include "graph.h"
#include <opencv2/core/core.hpp>

using namespace cv;

class Segmentation
{

vector<Graph> components;

public:
	Segmentation();
	vector<Graph> getComponents();
	void addComponent(Graph g);
	bool areDisjoint(Graph g1, Graph g2);
	Graph joinComponents(Graph g1, Graph g2);

};