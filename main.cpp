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
	GaussianBlur(channels[0], channels[0], Size(5,5), 0.8, 0.8, BORDER_DEFAULT);

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
            ck.addNode(k);
            S0.addComponent(ck);
            graph.setNodeIndexComponent(k,k);
            k++;
        }
    }

    vector<Component> components;
    Segmentation S = S0;
    int size = 0;
    for (int q = 0; q < edgesG.size(); q++) {
    // for (int q = 0; q < 4; q++) {

        if(S.getComponents().size() != size){
            components = S.getComponents();
        }
        size = components.size();
    	vector<Node> nodesG = graph.getNodes();
    	
    	// cout<<"comps size "<<components.size()<<endl;
        Edge edge = edgesG[q];
    	// cout<<"Analizing components related to edge "<<edge.getIndex()<<endl;
        vector<int> nodesEdge = edge.getNodes();

        Node node1 = nodesG[nodesEdge[0]];
        Node node2 = nodesG[nodesEdge[1]];
        // cout<<"comp indexes "<<node1.getIndexComponent()<<", "<<node2.getIndexComponent()<<endl;
        if(node1.getIndexComponent() != node2.getIndexComponent()) {

            Component& c1 = components[node1.getIndexComponent()];
            Component& c2 = components[node2.getIndexComponent()];
            // cout<<"comps index2 "<<c1.getIndex()<<", "<<c2.getIndex()<<endl;
            // cout<<"Antes de max edge mst 1 "<<c1.getMaxEdgeMST()<<" "<<c1.getIndex()<<endl;
            // cout<<"Antes de max edge mst 2 "<<c2.getMaxEdgeMST()<<" "<<c2.getIndex()<<endl;
            if(!S.areDisjoint(c1,c2,50,graph)) {
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
                        // updateNodeIndexComponent(new_c1,k-1,graph);
                        for (int j = 0; j < new_c1.getNodes().size(); ++j)
                        {
                            graph.setNodeIndexComponent(new_c1.getNodes()[j],new_c1.getIndex());
                        }
                		Saux.addComponent(new_c1);
                	} else if(i != indexc2){
                		Component& c = components[i];
                		c.setIndex(k++);
                        // updateNodeIndexComponent(c,k-1,graph);
                        for (int j = 0; j < c.getNodes().size(); ++j)
                        {
                            graph.setNodeIndexComponent(c.getNodes()[j],c.getIndex());
                        }
                		Saux.addComponent(c);
                	}

                }

                S = Saux;
            }
            // else{
            //     cout<<"max edge mst 1 "<<c1.getMaxEdgeMST()<<endl;
            //     cout<<"max edge mst 2 "<<c2.getMaxEdgeMST()<<endl;
            // }

        }
    }

    showImageSegmented(S,graph,width,height);

}

void tryMST(){

    Mat mst = (Mat_<uchar>(3,3)<<10, 9, 4, 11, 1, 7, 3, 2, 5);
    // cout<<mst<<endl;

    Graph graph = Graph();
    graph.createGraph(mst);
    // graph.printGraph();

    vector<Edge> edgesG = graph.getEdges();

    sort(edgesG.begin(),edgesG.end(),Edge::compareEdges);

    vector<Node> nodesG = graph.getNodes();

    Segmentation S0 = Segmentation();

    int height = mst.size().height;
    int width = mst.size().width;

    //creation of segmentation 0 with theirs components
    cout<<"Creating components in Segmentation 0"<<endl;
    
    Component c1 = Component();
    c1.setIndex(0);
    c1.addNode(0);
    graph.setNodeIndexComponent(0,0);
    c1.addNode(1);
    graph.setNodeIndexComponent(1,0);
    c1.addNode(2);
    graph.setNodeIndexComponent(2,0);
    c1.addNode(3);
    graph.setNodeIndexComponent(3,0);
    
    Component c2 = Component();
    c2.setIndex(1);
    c2.addNode(4);
    graph.setNodeIndexComponent(4,1);
    c2.addNode(5);
    graph.setNodeIndexComponent(5,1);
    c2.addNode(6);
    graph.setNodeIndexComponent(6,1);
    c2.addNode(7);
    graph.setNodeIndexComponent(7,1);
    c2.addNode(8);
    graph.setNodeIndexComponent(8,1);
    
    S0.addComponent(c1);
    S0.addComponent(c2);
    // int a = Segmentation::getMaxEdgeMST(c,graph);
    // cout<<a<<endl;
    int a = Segmentation::getDiffComponents(c1,c2,graph);
    cout<<a<<endl;
}

int main(int argc, char const *argv[])
{
	if(argc != 3){
		cout<<"ERROR: debe especificar un nombre de archivo a procesar"<<endl;
		exit(EXIT_FAILURE);
	}

	int type = (int)argv[2];

    segmentarManos(argv[1],type);
    // tryMST();

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
