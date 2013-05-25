//funciones.cpp
#include "funciones.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;


Funciones::Funciones(){}

Mat Funciones::histograma(Mat input){
	Mat hist = Mat::zeros(256,1,CV_32S);
	int j = 0, i = 0;

	for(i = 0; i < input.size().height; i++){
		for(j = 0; j < input.size().width; j++){
			hist.at<int>(input.at<uchar>(i,j),0)++;
		}
	}

	return hist;
}

Mat Funciones::binarizar(Mat input, int umbral){

	Mat binaria;
	int i=0, j=0;
	if(input.type() == CV_8UC1){
		binaria = Mat::zeros(input.size(),CV_8UC1);
		for(i=0; i<input.size().height; i++){

			for(j=0; j<input.size().width; j++){

				if(input.at<uchar>(i,j) > umbral){
					binaria.at<uchar>(i,j) = 1;
				}

			}

		}
	}

	return binaria;

}

int Funciones::umbralOtsu(Mat imagen){
	Mat h = Funciones::histograma(imagen);
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

Mat Funciones::ecualizar(Mat input)
{
	//asumimos input esta en escala de grises
	Mat hist=histograma(input);
	cout<<hist<<endl;
	Mat ecu=Mat(input.size(), input.type());
	float acum[256];
	int i=0, j=0;
	hist.convertTo(hist,CV_32F);
	acum[0]=hist.at<float>(0,0);

	for(i=1;i<256;i++)
	{
		acum[i]=hist.at<float>(i,0)+acum[i-1];
	}
	for(i=0;i<256;i++)
	{
		cout<<acum[i]<<",";
	}
	cout<<endl;
	uchar val=0;

	for(i=0;i<input.size().height;i++)
	{
		for(j=0;j<input.size().width;j++)
		{
			val=(uchar)floor((255.0/(input.size().height*input.size().width))*acum[input.at<uchar>(i,j)]);
			ecu.at<uchar>(i,j)=val;
		}
	}
	return ecu;
}

Mat Funciones::edge_derivadas(Mat input, int TIPO)
{
	Mat mask_x;
	Mat mask_y;
	Point p;
	Mat Gx;
	Mat Gy;
	Mat G;

	int EDGE_DERIVADA = 1;
	int EDGE_PREWITT = 2;
	int EDGE_SOBEL = 3;

	if(TIPO==EDGE_DERIVADA)
	{
		mask_x=(Mat_<int>(1,2)<<-1,1);
		mask_y=(Mat_<int>(2,1)<<-1,1);
		p=Point(0,0);
	}
	else if(TIPO==EDGE_PREWITT)
	{
		mask_x=(Mat_<int>(3,3)<<-1, 0, 1, -1, 0, 1, -1, 0, 1);
		mask_y=(Mat_<int>(3,3)<<-1, -1, -1, 0, 0, 0, 1, 1, 1);
		p=Point(-1,-1);
	}
	else if(TIPO==EDGE_SOBEL)
	{
		mask_x=(Mat_<int>(3,3)<<-1, 0, 1, -2, 0, 2, -1, 0, 1);
		mask_y=(Mat_<int>(3,3)<<-1, -2, -1, 0, 0, 0, 1, 2, 1);
		p=Point(-1,-1);
	}

	filter2D(input,Gx,CV_32F,mask_x,p);
	filter2D(input,Gy,CV_32F,mask_y,p);
	convertScaleAbs(Gx,Gx);//valor absoluto y convierte a CV_8UC1
	convertScaleAbs(Gy,Gy);

	G=Gx+Gy;
	convertScaleAbs(G,G);
	return G;
}

float Funciones::funcion_h(float gradient, float K){
	return 1 / (1 + pow(gradient / K, 2));
}

Mat Funciones::difusion_anisotropica(Mat input,float lambda, float K, int t){
	Mat mask_n = (Mat_<int>(3,1)<<1,-1,0);
	Mat mask_s = (Mat_<int>(3,1)<<0,-1,1);
	Mat mask_e = (Mat_<int>(1,3)<<0,-1,1);
	Mat mask_w = (Mat_<int>(1,3)<<1,-1,0);

	Mat Gn, Gs, Ge, Gw;
	float c_n, c_s, c_e,c_w;

	Point p = Point(-1,-1);
	int i, j, k;
	input.convertTo(input,CV_32F);


	for(i = 0; i < t; i++){

		filter2D(input,Gn,CV_32F,mask_n,p);
		filter2D(input,Gs,CV_32F,mask_s,p);
		filter2D(input,Ge,CV_32F,mask_e,p);
		filter2D(input,Gw,CV_32F,mask_w,p);

		for(j=0; j<Gn.size().height; j++){
			for(k=0; k<Gn.size().width; k++){
				c_n = funcion_h(abs(Gn.at<float>(j,k)),K);
				c_s = funcion_h(abs(Gs.at<float>(j,k)),K);
				c_e = funcion_h(abs(Ge.at<float>(j,k)),K);
				c_w = funcion_h(abs(Gw.at<float>(j,k)),K);

				input.at<float>(j,k) = input.at<float>(j,k) + lambda*(c_n*Gn.at<float>(j,k) + c_s*Gs.at<float>(j,k) +c_e*Ge.at<float>(j,k) + c_w*Gw.at<float>(j,k));
			
			}
		}
	}
	convertScaleAbs(input,input);
	return input;
}