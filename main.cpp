#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <limits>
#include <iostream>
#include <cstdlib>
#include <string>

#include "funciones.h"
//#include "graph.h"
#include "segmentation.h"

using namespace std;
using namespace cv;

void segmentarManos(string filename, int type){
	Mat imagen = imread(filename,-1);

	if(imagen.empty()){
		cout<<"ERROR: la imagen"<<filename<<" no pudo ser encontrada"<<endl;
		exit(EXIT_FAILURE);
	}

	vector<Mat> channels(3);

	split(imagen,channels);

    Graph graph = Graph();
    graph.createGraph(channels[0]);

    vector<Edge> edgesG = graph.getEdges();
    sort(edgesG.begin(),edgesG.end(),Edge::compareEdges);

    Segmentation S0 = Segmentation();

    int height = imagen.size().height;
    int width = imagen.size().width;

    //creation of segmentation 0 with theirs components
    int k = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Component ck = Component();
            ck.setIndex(k);
            ck.addNodes(k);
            S0.addComponent(ck);
            k++;
        }
    }

    vector<Component> components = S0.getComponents();
    for (int q = 0; q < edgesG.size(); q++) {
        Edge edge = edgesG[q];
        vector<int> nodesEdge = edge.getNodes();

        Node node1 = nodesEdge[0];
        Node node2 = nodesEdge[1];

        if(nodeA.getIndexComponent() != nodeB.getIndexComponent()) {

            Component c1 = components[node1.getIndexComponent()];
            Component c2 = components[node2.getIndexComponent()];

            if(!S0.areDisjoint(c1,c2)) {
                Component new_c1 = S0.joinComponents(c1,c2);
            }
        }
    }

}

int main(int argc, char const *argv[])
{
	if(argc != 3){
		cout<<"ERROR: debe especificar un nombre de archivo a procesar"<<endl;
		exit(EXIT_FAILURE);
	}

	int type = (int)argv[2];

    segmentarManos(argv[1],type);

	// Mat input = Mat::zeros(3,3,CV_8UC1);
	// int k = 0;
	// for (int i = 0; i < 3; ++i)
	// {
	// 	for (int j = 0; j < 3; ++j)
	// 	{
	// 		input.at<uchar>(i,j) = k++;
	// 	}
	// }

	// Graph graph = Graph();
	// graph.createGraph(input);
	// graph.printGraph();

	waitKey(0);
	return 0;
}
