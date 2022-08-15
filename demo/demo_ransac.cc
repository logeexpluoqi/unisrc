#include "demo_ransac.h"
#include "../ransac/ransac.h"
using namespace std;
int demo_ransac_init() {
	vector<float> gt= { 1, 1, 1, 1 };
	ransac::Ransac r;
	r.TestDemo(gt);
	return 0;
}
