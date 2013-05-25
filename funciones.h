//funciones.h
#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <opencv2/core/core.hpp>

using namespace cv;

class Funciones
{
public:
	Funciones();
	static Mat binarizar(Mat input,int umbral);
	static int umbralOtsu(Mat input);
	static Mat histograma(Mat input);
	static Mat ecualizar(Mat input);
	static Mat edge_derivadas(Mat input,int TIPO);
	static float funcion_h(float gradient, float K);
	static Mat difusion_anisotropica(Mat input,float lambda, float K, int t);
};

#endif // FUNCIONES_H
