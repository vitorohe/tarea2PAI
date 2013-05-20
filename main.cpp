#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <limits>
#include <iostream>
#include <cstdlib>
#include <string>

#include "funciones.h"
#include "graph.h"

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

	// Graph graph = Graph();
	// graph.createGraph(channels[0]);

	// vector<Edge> edgesG = graph.getEdges();
	// sort(edgesG.begin(),edgesG.end(),Edge::compareEdges);
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