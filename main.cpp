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



void paintImg(Node *root, Vec3b vecColor, Mat& imgSegmented, int width, int height){
    int n = root->getIndex()/width;
    int m = root->getIndex()%width;
    imgSegmented.at<Vec3b>(n,m) = vecColor;
    // cout<<"painting "<<root->getIndex()<<endl;

    // cout<<"children size "<<root->children.size()<<endl;
    // if(!root->children) {
        // cout<<"painting children size "<<root->children.size()<<endl;
    for(int i = 0; i < root->children->size(); i++){
        Node *child = (*root->children).at(i);
        // cout<<"painting child "<<child->getIndex()<<endl;
        paintImg(child,vecColor,imgSegmented,width,height);
    }
    // }
}

void showImageSegmented(vector<Component *> *components, int width, int height, int channel, Mat imagen){

	Mat imgSegmented = Mat::zeros(height,width,CV_8UC3);

	// vector<Component *> components = S.getComponents();
    // cout<<"components "<<components.size()<<endl;
    int k = 0;
    for(int i = 0; i < components->size(); i++){
        // cout<<"component "<<i<<" size "<<components[i]->getSize()<<endl;
        if((*components).at(i) != NULL){
            k++;
            int totalVal = (*components).at(i)->getTotalVal();
            int sizeC = (*components).at(i)->getSize();

            // int avg = (*components).at(i)->root->getValue();//totalVal/sizeC;
            int n = (*components).at(i)->root->getIndex()/width;
            int m = (*components).at(i)->root->getIndex()%width;
            Vec3b vecColor = imagen.at<Vec3b>(n,m);

            paintImg((*components).at(i)->root,vecColor,imgSegmented,width,height);
        }
    }
    cout<<"components "<<k<<endl;

    string name = "";
    if(channel == 0)
        name = "ImgSegmented channel 0";
    else if(channel == 1)
        name = "ImgSegmented channel 1";
    else
        name = "ImgSegmented channel 2";

    string extension = ".jpg";
    imshow(name,imgSegmented);
    imwrite(name + extension,imgSegmented);
}

Pair getSegmentation(Mat input, int channel, int type) {
    GaussianBlur(input, input, Size(7,7), 1.1, 1.1, BORDER_DEFAULT);
    // channels[2] = Funciones::difusion_anisotropica(channels[2],0.1, 40, 40);
    // pyrMeanShiftFiltering(imagen,imagen,25,25,3);
    // imshow("Gchannels[2]",channels[2]);
    // imshow("pyrMeanShiftFiltering",imagen);
    // return;
    Graph graph = Graph(input.size().height, input.size().width);
    graph.createGraph(input, type);
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

    Segmentation *S = new Segmentation();

    int height = input.size().height;
    int width = input.size().width;

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
            S->addComponent(ck);
            // graph.setNodeIndexComponent(k,k);
            k++;
        }
    }


    // for (int i = 0; i < nodesG->size(); ++i)
    // {
    //     cout<<(*nodesG).at(i)->getIndexComponent()<<endl;
    // }

    vector<Component *> components = S->getComponents();
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
            if(!S->areDisjoint(c1,c2,300,graph)) {
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
    // showImageSegmented(&components,graph,width,height, channel);
    // graph.freeClear(edgesG1);
    // graph.freeGraph();
    // S.freeSegmentation();
    // Graph *g = &graph;
	Pair p = Pair();
	p.first = &graph;
	p.second = S;
    return p;
}

void segmentarManos(string filename, int type){
	Mat imagen = imread(filename,-1);

	if(imagen.empty()){
		cout<<"ERROR: la imagen"<<filename<<" no pudo ser encontrada"<<endl;
		exit(EXIT_FAILURE);
	}
	Mat imagenLab;
	cvtColor(imagen,imagenLab,CV_BGR2HSV);
	// imshow("imagen",imagen);
	// return;
	vector<Mat> channels(3);

	split(imagenLab,channels);

	int width = imagen.size().width;
	int height = imagen.size().height;

	if(type == 1) {
		Pair res0 = getSegmentation(channels[0], 0, type);
		Pair res1 = getSegmentation(channels[1], 1, type);
		Pair res2 = getSegmentation(channels[2], 2, type);

		vector<Node *> *nodesG0 = res0.first->getNodes();
		vector<Node *> *nodesG1 = res1.first->getNodes();
		vector<Node *> *nodesG2 = res2.first->getNodes();
	
		Segmentation Super = Segmentation();
		vector<Node *> *nodesS = new vector<Node *>;
		nodesS->reserve(width*height);

		int k = 0;
		for (int i = 0; i < height; ++i) {
		    for (int j = 0; j < width; ++j) {
		        Component *ck = new Component();
		        ck->setIndex(k);
		        Node *n = new Node(k);
		        ck->root = n;
		        ck->root->indexComponent = k;
		        ck->root->parent = ck->root;
		        ck->addToTotalVal(ck->root->getValue());
		        ck->setSize(1);
		        Super.addComponent(ck);
		        nodesS->push_back(n);
		        k++;
		    }
		}
		// cout<<"hola"<<endl;
		vector<Component *> components = Super.getComponents();
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				int index_a = j+width*i;
				Node *node_a0 = (*nodesG0)[index_a];
				Node *node_a1 = (*nodesG1)[index_a];
				Node *node_a2 = (*nodesG2)[index_a];

				int index_aC0 = node_a0->findN(node_a0)->getIndexComponent();
				int index_aC1 = node_a1->findN(node_a1)->getIndexComponent();
				int index_aC2 = node_a2->findN(node_a2)->getIndexComponent();

				int index_b;
				int weight;
				// cout<<"holaij "<<i<<","<<j<<endl;
				//right
				if(j < width-1) {
					// cout<<"holaif1"<<endl;
					index_b = (j + 1) + i*width;

					Node *node_b0 = (*nodesG0)[index_b];
					int index_bC0 = node_b0->findN(node_b0)->getIndexComponent();
					
					if(index_aC0 == index_bC0){
						// cout<<"holaif1c0"<<endl;
						Node *node_b1 = (*nodesG1)[index_b];
						int index_bC1 = node_b1->findN(node_b1)->getIndexComponent();
						
						if(index_aC1 == index_bC1){
							// cout<<"holaif1c1"<<endl;
							Node *node_b2 = (*nodesG2)[index_b];
							int index_bC2 = node_b2->findN(node_b2)->getIndexComponent();

							if(index_aC2 == index_bC2){
								// cout<<"holaif1c2"<<endl;
								Node *node_a = (*nodesS).at(index_a);
								Node *node_b = (*nodesS).at(index_b);
								
								int index_aC = node_a->findN(node_a)->getIndexComponent();
								int index_bC = node_b->findN(node_b)->getIndexComponent();
						
								// cout<<"aC "<<index_aC<<" bC "<<index_bC<<endl;
						
								if(index_aC != index_bC){
									Component *ca = components.at(index_aC);
									Component *cb = components.at(index_bC);
									ca->unionC(cb->root);
									ca->root->indexComponent = index_aC;
									ca->setSize(ca->getSize() + cb->getSize());
									ca->addToTotalVal(ca->getTotalVal());
									components[index_bC] = NULL;
								}
							}
						}
					}
				}

				//right and down
				if(j < width-1 && i < height-1) {
					// cout<<"holaif2"<<endl;
					index_b = (j + 1) + (i + 1)*width;
					
					Node *node_b0 = (*nodesG0)[index_b];
					int index_bC0 = node_b0->findN(node_b0)->getIndexComponent();
					
					if(index_aC0 == index_bC0){
						// cout<<"holaif2c0"<<endl;
						Node *node_b1 = (*nodesG1)[index_b];
						int index_bC1 = node_b1->findN(node_b1)->getIndexComponent();
						
						if(index_aC1 == index_bC1){
							// cout<<"holaif2c1"<<endl;
							Node *node_b2 = (*nodesG2)[index_b];
							int index_bC2 = node_b2->findN(node_b2)->getIndexComponent();

							if(index_aC2 == index_bC2){
								// cout<<"holaif2c2"<<endl;
								Node *node_a = (*nodesS).at(index_a);
								Node *node_b = (*nodesS).at(index_b);
								
								int index_aC = node_a->findN(node_a)->getIndexComponent();
								int index_bC = node_b->findN(node_b)->getIndexComponent();
		        				// cout<<"aC "<<index_aC<<" bC "<<index_bC<<endl;
		        				
		        				if(index_aC != index_bC){
		        					Component *ca = components[index_aC];
		        					Component *cb = components[index_bC];
								
									ca->unionC(cb->root);
									ca->root->indexComponent = index_aC;
									ca->setSize(ca->getSize() + cb->getSize());
									ca->addToTotalVal(ca->getTotalVal());
									components[index_bC] = NULL;
								}
							}
						}
					}
				}

				//down
				if(i < height-1) {
					// cout<<"holaif3"<<endl;
					index_b = j + (i + 1)*width;
					
					Node *node_b0 = (*nodesG0)[index_b];
					int index_bC0 = node_b0->findN(node_b0)->getIndexComponent();
					
					if(index_aC0 == index_bC0){
						// cout<<"holaif3c0"<<endl;
						Node *node_b1 = (*nodesG1)[index_b];
						int index_bC1 = node_b1->findN(node_b1)->getIndexComponent();
						
						if(index_aC1 == index_bC1){
							// cout<<"holaif3c1"<<endl;
							Node *node_b2 = (*nodesG2)[index_b];
							int index_bC2 = node_b2->findN(node_b2)->getIndexComponent();

							if(index_aC2 == index_bC2){
								// cout<<"holaif3c2"<<endl;
								Node *node_a = (*nodesS).at(index_a);
								Node *node_b = (*nodesS).at(index_b);
								
								int index_aC = node_a->findN(node_a)->getIndexComponent();
								int index_bC = node_b->findN(node_b)->getIndexComponent();
		        				// cout<<"aC "<<index_aC<<" bC "<<index_bC<<endl;
		        				
		        				if(index_aC != index_bC){
		        					Component *ca = components[index_aC];
		        					Component *cb = components[index_bC];
								
									ca->unionC(cb->root);
									ca->root->indexComponent = index_aC;
									ca->setSize(ca->getSize() + cb->getSize());
									ca->addToTotalVal(ca->getTotalVal());
									components[index_bC] = NULL;
								}
							}
						}
					}
				}

				//left and down
				if(j > 0 && i < height-1) {
					// cout<<"holaif4"<<endl;
					index_b = (j - 1) + (i + 1)*width;
					
					Node *node_b0 = (*nodesG0)[index_b];
					int index_bC0 = node_b0->findN(node_b0)->getIndexComponent();
					
					if(index_aC0 == index_bC0){
						// cout<<"holaif4c0"<<endl;
						Node *node_b1 = (*nodesG1)[index_b];
						int index_bC1 = node_b1->findN(node_b1)->getIndexComponent();
						
						if(index_aC1 == index_bC1){
							// cout<<"holaif4c1"<<endl;
							Node *node_b2 = (*nodesG2)[index_b];
							int index_bC2 = node_b2->findN(node_b2)->getIndexComponent();

							if(index_aC2 == index_bC2){
								// cout<<"holaif4c2"<<endl;
								Node *node_a = (*nodesS).at(index_a);
								Node *node_b = (*nodesS).at(index_b);
								
								int index_aC = node_a->findN(node_a)->getIndexComponent();
								int index_bC = node_b->findN(node_b)->getIndexComponent();
		        				// cout<<"aC "<<index_aC<<" bC "<<index_bC<<endl;
		        				
		        				if(index_aC != index_bC){
		        					Component *ca = components[index_aC];
		        					Component *cb = components[index_bC];
								
									ca->unionC(cb->root);
									ca->root->indexComponent = index_aC;
									ca->setSize(ca->getSize() + cb->getSize());
									ca->addToTotalVal(ca->getTotalVal());
									components[index_bC] = NULL;
								}
							}
						}
					}
				}
			}
		}

		showImageSegmented(&components,width,height, 0,imagen);
	
	} else if(type == 2) {
		Pair res = getSegmentation(imagenLab, 0, type);
		vector<Component *> components = res.second->getComponents();
		showImageSegmented(&components,width,height, 0,imagen);
	}
    

}

void tryMST(){

    // Mat mst = (Mat_<uchar>(3,3)<<10, 9, 8, 11, 1, 7, 3, 2, 5);
    Mat mst = (Mat_<uchar>(3,3)<<10, 9, 8, 11, 20, 1, 3, 2, 5);
    // // cout<<mst<<endl;

    Graph graph = Graph(mst.size().height, mst.size().width);
    graph.createGraph(mst,1);
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

Funciones::Funciones(){}

Mat histograma(Mat input){
	Mat hist = Mat::zeros(256,1,CV_32S);
	int j = 0, i = 0;

	for(i = 0; i < input.size().height; i++){
		for(j = 0; j < input.size().width; j++){
			hist.at<int>(input.at<uchar>(i,j),0)++;
		}
	}

	return hist;
}

int umbralOtsu(Mat imagen){
	Mat h = histograma(imagen);
	h.convertTo(h,CV_32F);
	int i = 0;

	int suma = sum(h)[0];
	
	h= h/suma;
	
	float acum [256];
	float media [256];

	acum[0] = h.at<float>(0,0);
	media[0] = 0;


	for(i = 1; i <= 255; i++){
		acum[i] = h.at<float>(i,0) + acum[i-1];
		media[i] = i*h.at<float>(i,0) + media[i-1];
	}

	float m = media[255];
	float maxVal = 0;
	uchar umbral = 0;
	float val = 0;
	float P1 = 0, P2 = 0, m1 = 0, m2 = 0;

	for(i = 0; i <= 255; i++){
		P1 = acum[i];
		P2 = 1 - P1;
		m1 = media[i]/P1;
		m2 = (m - media[i])/P2;
		val = P1*pow(m1 - m,2) + P2*pow(m2 - m,2);
		if(val > maxVal){
			maxVal = val;
			umbral = i;
		}
	}

	return umbral;
}

void binarizar(string filename){
	Mat imagen = imread(filename,-1);

	if(imagen.empty()){
		cout<<"ERROR: la imagen"<<filename<<" no pudo ser encontrada"<<endl;
		exit(EXIT_FAILURE);
	}

	cvtColor(imagen,imagen,CV_BGR2GRAY);
	threshold(imagen, imagen, (double)umbralOtsu(imagen), 255, THRESH_BINARY);
	imshow("imagen",imagen);
	imwrite("imagenbin.jpg",imagen);

}

int main(int argc, char const *argv[])
{
	if(argc != 3){
		cout<<"ERROR: debe especificar un nombre de archivo a procesar"<<endl;
		exit(EXIT_FAILURE);
	}

	int type;
	stringstream ss (argv[2]);
	ss >> type;
	
    segmentarManos(argv[1],type);
    // tryMST();
    // binarizar(argv[1]);

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
