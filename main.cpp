#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <limits>
#include <iostream>
#include <cstdlib>
#include <string>

#include "segmentation.h"
#include "funciones.h"

using namespace std;
using namespace cv;



void paintImg(Node *root, int avg, Mat& imgSegmented, int width, int height){
    int n = root->getIndex()/width;
    int m = root->getIndex()%width;
    imgSegmented.at<uchar>(n,m) = avg;
    // cout<<"painting "<<root->getIndex()<<endl;

    // cout<<"children size "<<root->children.size()<<endl;
    // if(!root->children) {
        // cout<<"painting children size "<<root->children.size()<<endl;
    for(int i = 0; i < root->children->size(); i++){
        Node *child = (*root->children).at(i);
        // cout<<"painting child "<<child->getIndex()<<endl;
        paintImg(child,avg,imgSegmented,width,height);
    }
    // }
}

void showImageSegmented(vector<Component *> *components, Graph graph, int width, int height){

	Mat imgSegmented = Mat::zeros(height,width,CV_8UC1);

	// vector<Component *> components = S.getComponents();
    // cout<<"components "<<components.size()<<endl;
    int k = 0;
    for(int i = 0; i < components->size(); i++){
        // cout<<"component "<<i<<" size "<<components[i]->getSize()<<endl;
        if((*components).at(i) != NULL){
            k++;
            int totalVal = (*components).at(i)->getTotalVal();
            int sizeC = (*components).at(i)->getSize();

            int avg = totalVal/sizeC;

            paintImg((*components).at(i)->root,avg,imgSegmented,width,height);
        }
    }
    cout<<"components "<<k<<endl;
    imshow("imgSegmented",imgSegmented);
    imwrite("imgSegmented.png",imgSegmented);
}

void segmentarManos(string filename, int type){
	Mat imagen = imread(filename,-1);

	if(imagen.empty()){
		cout<<"ERROR: la imagen"<<filename<<" no pudo ser encontrada"<<endl;
		exit(EXIT_FAILURE);
	}

	vector<Mat> channels(3);

	split(imagen,channels);
    // imshow("channels[2]",channels[2]);
    // GaussianBlur(channels[2], channels[2], Size(7,7), 1.1, 1.1, BORDER_DEFAULT);
    // channels[2] = Funciones::difusion_anisotropica(channels[2],0.1, 40, 40);
    pyrMeanShiftFiltering(imagen,imagen,25,25,3);
    // imshow("Gchannels[2]",channels[2]);
    imshow("pyrMeanShiftFiltering",imagen);
    return;
    Graph graph = Graph(imagen.size().height, imagen.size().width);
    graph.createGraph(channels[2]);
    // graph.printGraph();
    vector<Node *> *nodesG = graph.getNodes();
    vector<Edge *> *edgesG = graph.getEdges();

    vector<pair<int,int> > edgesG1;
    edgesG1.reserve(edgesG->size());
    for (int i = 0; i < edgesG->size(); i++)
    {
        int j = (*edgesG).at(i)->getIndex();
        int w = (*edgesG).at(i)->getWeight();
        pair<int,int> p;
        p.first = j;
        p.second = w;
        edgesG1.push_back(p);
    }

    sort(edgesG1.begin(),edgesG1.end(),Graph::compareEdges1);

    // vector<Edge *> *edgesG1(edgesG);
    // sort(edgesG1->begin(),edgesG1->end(),Edge::compareEdges);

    // for (int i = 0; i<edgesG->size(); i++)
    // {
    //     int j = (*edgesG).at(i)->getIndex();
    //     int w = (*edgesG).at(i)->getWeight();
    //     cout<<"edge "<<j<<" w "<<w<<endl;
    //     // cout<<"edge "<<(*edgesG).at(i)->getIndex()<<" w "<<(*edgesG).at(i)->getWeight()<<endl;
    // }    
    // for (int i = 0; i<edgesG->size(); i++)
    // {
    //     int j = edgesG1[i].first;
    //     int w = edgesG1[i].second;
    //     cout<<"edge "<<j<<" w "<<w<<endl;
    //     // cout<<"edge "<<(*edgesG).at(i)->getIndex()<<" w "<<(*edgesG).at(i)->getWeight()<<endl;
    // }

    // return;

    Segmentation S = Segmentation();

    int height = imagen.size().height;
    int width = imagen.size().width;

    //creation of segmentation 0 with theirs components
    cout<<"Creating components in Segmentation 0"<<endl;
    int k = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Component *ck = new Component();
            ck->setIndex(k);
            ck->root = (*nodesG)[k];
            ck->root->indexComponent = k;
            ck->root->parent = ck->root;
            ck->addToTotalVal(ck->root->getValue());
            ck->setSize(1);
            S.addComponent(ck);
            // graph.setNodeIndexComponent(k,k);
            k++;
        }
    }


    // for (int i = 0; i < nodesG->size(); ++i)
    // {
    //     cout<<(*nodesG).at(i)->getIndexComponent()<<endl;
    // }

    vector<Component *> components = S.getComponents();
    cout<<"Creating segmentation"<<endl;
    int size = 0;
    for (int q = 0; q < edgesG1.size(); q++) {
        // cout<<"comps size "<<components.size()<<endl;
        Edge *edge = (*edgesG).at(edgesG1[q].first);
    	// cout<<"Analizing components related to edge "<<edge->getIndex()<<" q "<<q<<endl;
        vector<int> nodesEdge = edge->getNodes();

        Node *node1 = (*nodesG)[nodesEdge[0]];
        Node *node2 = (*nodesG)[nodesEdge[1]];
        // cout<<"comp indexes "<<node1->getIndexComponent()<<", "<<node2->getIndexComponent()<<endl;
        int index1 = node1->findN(node1)->getIndexComponent();
        // cout<<"middle find"<<endl;
        int index2 = node2->findN(node2)->getIndexComponent();
        // cout<<"index 1: "<<index1<<", 2: "<<index2<<endl;
        if(index1 != index2) {

            Component *c1 = components[index1];
            Component *c2 = components[index2];
            // cout<<"children1 root"<<endl;
            // cout<<"children2 root"<<endl;
            // cout<<"comps index2 "<<c1->getIndex()<<", "<<c2->getIndex()<<endl;
            // cout<<"Antes de max edge mst 1 "<<c1.getMaxEdgeMST()<<" "<<c1.getIndex()<<endl;
            // cout<<"Antes de max edge mst 2 "<<c2.getMaxEdgeMST()<<" "<<c2.getIndex()<<endl;
            // cout<<"-----------------are disjoint------------------------------------"<<endl;
            if(!S.areDisjoint(c1,c2,300,graph)) {
                // cout<<"pre union"<<endl;
                c1->unionC(c2->root);
                // cout<<"post union"<<endl;
                c1->root->indexComponent = index1;
                c1->setSize(c1->getSize() + c2->getSize());
                c1->addToTotalVal(c2->getTotalVal());
                components[index2] = NULL;
                // cout<<"children root"<<c1->root->children->size()<<endl;
            }
            // else{
            //     cout<<"max edge mst 1 "<<c1.getMaxEdgeMST()<<endl;
            //     cout<<"max edge mst 2 "<<c2.getMaxEdgeMST()<<endl;
            // }

        }
    }
    showImageSegmented(&components,graph,width,height);
    // graph.freeClear(edgesG1);
    graph.freeGraph();
    // S.freeSegmentation();

}

void tryMST(){

    // Mat mst = (Mat_<uchar>(3,3)<<10, 9, 8, 11, 1, 7, 3, 2, 5);
    Mat mst = (Mat_<uchar>(3,3)<<10, 9, 8, 11, 20, 1, 3, 2, 5);
    // // cout<<mst<<endl;

    Graph graph = Graph(mst.size().height, mst.size().width);
    graph.createGraph(mst);
    // graph.printGraph();

    vector<Edge *> *edgesG = graph.getEdges();

    // sort(edgesG.begin(),edgesG.end(),Edge::compareEdges);

    vector<Node *> *nodesG = graph.getNodes();

    Segmentation S = Segmentation();

    int height = mst.size().height;
    int width = mst.size().width;

    // //creation of segmentation 0 with theirs components
    // cout<<"Creating components in Segmentation 0"<<endl;
    
    cout<<"Creating components in Segmentation 0"<<endl;
    int k = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Component *ck = new Component();
            ck->setIndex(k);
            ck->root = (*nodesG)[k];
            ck->root->indexComponent = k;
            ck->root->parent = ck->root;
            ck->addToTotalVal(ck->root->getValue());
            ck->setSize(1);
            S.addComponent(ck);
            k++;
        }
    }
    vector<Component *> components = S.getComponents();
    for(int i=1; i<5;i++){
        components.at(0)->unionC(components.at(i)->root);
        components.at(0)->root->indexComponent = 0;
        components.at(0)->setSize(components.at(0)->getSize() + components.at(i)->getSize());
        components.at(0)->addToTotalVal(components.at(i)->getTotalVal());
    }

    Component::printComponent(components.at(0)->root);

    for(int i=6; i<9;i++){
        components.at(5)->unionC(components.at(i)->root);
        components.at(5)->root->indexComponent = 5;
        components.at(5)->setSize(components.at(5)->getSize() + components.at(i)->getSize());
        components.at(5)->addToTotalVal(components.at(i)->getTotalVal());
    }
    cout<<"------------------------------"<<endl;
    Component::printComponent(components.at(5)->root);

    //int a = Segmentation::getMaxEdgeMST(components.at(5)->root, nodesG, edgesG, 5);
    //cout<<a<<endl;
    int a = Segmentation::getDiffComponents(components.at(0),components.at(5),graph);
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
