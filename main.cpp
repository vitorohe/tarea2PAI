#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <limits>
#include <iostream>
#include <cstdlib>
#include <string>

#include "funciones.h"
#include "segmentation.h"

using namespace std;
using namespace cv;

void showImageSegmented(Segmentation S, Graph graph, int width, int height){

	Mat imgSegmented = Mat::zeros(height,width,CV_8UC1);

	vector<Component> components = S.getComponents();
	vector<int> valComponent;
	vector<Node> nodesG = graph.getNodes();
	int k = 0;
	for (int i = 0; i < components.size(); ++i)
	{
		vector<int> nodesC = components[i].getNodes();
		int sum = 0;
		for (int j = 0; j < nodesC.size(); ++j)
		{
			sum += nodesG[nodesC[j]].getValue();
			k++;
		}
		valComponent.push_back(sum/nodesC.size());
		// cout<<"sum: "<<sum<<" proms: "<<valComponent[valComponent.size()-1]<<endl;

		for (int j = 0; j < nodesC.size(); ++j)
		{
			int n = nodesG[nodesC[j]].getIndex()/width;
			int m = nodesG[nodesC[j]].getIndex()%width;
			imgSegmented.at<uchar>(n,m) = valComponent[valComponent.size()-1];
		}
	}

	imshow("imgSegmented",imgSegmented);
	imwrite("imgSegmented.png",imgSegmented);

}

void updateNodeIndexComponent(Component c, int index, Graph& graph){
	// cout<<"Updating Nodes IndexComponent "<<index<<endl;
	vector<int> nodes  = c.getNodes();

	for (int i = 0; i < nodes.size(); ++i)
	{
		graph.setNodeIndexComponent(nodes[i],index);
	}

}

void segmentarManos(string filename, int type){
	Mat imagen = imread(filename,-1);

	if(imagen.empty()){
		cout<<"ERROR: la imagen"<<filename<<" no pudo ser encontrada"<<endl;
		exit(EXIT_FAILURE);
	}

	vector<Mat> channels(3);

	split(imagen,channels);
	medianBlur(channels[0], channels[0], 5);

    Graph graph = Graph();
    graph.createGraph(channels[0]);

    vector<Edge> edgesG = graph.getEdges();
    sort(edgesG.begin(),edgesG.end(),Edge::compareEdges);

    vector<Node> nodesG = graph.getNodes();

    Segmentation S0 = Segmentation();

    int height = imagen.size().height;
    int width = imagen.size().width;

    //creation of segmentation 0 with theirs components
    cout<<"Creating components in Segmentation 0"<<endl;
    int k = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Component ck = Component();
            ck.setIndex(k);
            ck.addNodes(k);
            S0.addComponent(ck);
            graph.setNodeIndexComponent(k,k);
            k++;
        }
    }

    vector<Component> components;
    Segmentation S = S0;
    for (int q = 0; q < edgesG.size(); q++) {
    // for (int q = 0; q < 4; q++) {
    	vector<Node> nodesG = graph.getNodes();
    	components = S.getComponents();
    	// cout<<"comps size "<<components.size()<<endl;
        Edge edge = edgesG[q];
    	// cout<<"Analizing components related to edge "<<edge.getIndex()<<endl;
        vector<int> nodesEdge = edge.getNodes();

        Node node1 = nodesG[nodesEdge[0]];
        Node node2 = nodesG[nodesEdge[1]];
        // cout<<"comp indexes "<<node1.getIndexComponent()<<", "<<node2.getIndexComponent()<<endl;
        if(node1.getIndexComponent() != node2.getIndexComponent()) {

            Component c1 = components[node1.getIndexComponent()];
            Component c2 = components[node2.getIndexComponent()];
            // cout<<"comps index2 "<<c1.getIndex()<<", "<<c2.getIndex()<<endl;
            if(!S.areDisjoint(c1,c2,300,graph)) {
                Component new_c1 = S.joinComponents(c1,c2);
                // cout<<"Components were joined"<<endl;
                int indexc1 = c1.getIndex();
                int indexc2 = c2.getIndex();

                Segmentation Saux = Segmentation();

                int k = 0;
                for (int i = 0; i < components.size(); ++i)
                {
                	if(i == indexc1) {
                		new_c1.setIndex(k++);
                		Saux.addComponent(new_c1);
                		updateNodeIndexComponent(new_c1,k-1,graph);
                	} else if(i != indexc2){
                		Component c = components[i];
                		c.setIndex(k++);
                		Saux.addComponent(c);
                		updateNodeIndexComponent(c,k-1,graph);
                	}

                }

                S = Saux;
            }

        }
    }

    showImageSegmented(S,graph,width,height);

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
