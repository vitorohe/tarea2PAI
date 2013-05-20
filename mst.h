#include <opencv2/core/core.hpp>

using namespace cv;

class MST
{

Mat graphMST;

public:
	MST();
	void createMST(Mat graph, int nodes);
	int maxWeightMST();
};