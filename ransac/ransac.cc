#include "ransac.h"
namespace ransac {
Ransac::Ransac(){
}

void Ransac::TestDemo(const std::vector<float>& plane) {
	std::cout << "The ground truth of plane is:"
		<< plane[0] << ' ' << plane[1] << ' ' << plane[2] << ' ' << plane[3] << std::endl;
	std::vector<std::vector<float>> points = SamplePoints(plane, 10);
	std::vector<float> out = CalPlane(points);
	std::cout << "The predition of plane is:"
		<< ' ' << plane[0]
		<< ' ' << plane[0] * out[1] / out[0]
		<< ' ' << plane[0] * out[2] / out[0] 
		<< ' ' << plane[0] *  out[3] / out[0] << std::endl;
}

std::vector<std::vector<float>> Ransac::SamplePoints(const std::vector<float>& plane, int n) {
	std::vector<std::vector<float>> out;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float x = i;
			float y = j;
			float rand_num = float((rand() % 100)) / 500-0.1; //精度为小数后两位的(-0.1, 0.1)范围随机数
			float z = (plane[0] * x + plane[1] * y + plane[3]) / (-plane[2]) + rand_num;
			out.push_back(std::vector<float>{x, y, z});
		}
	}
	return out;
}

std::vector<float> Ransac::VecSubtraction(const std::vector<float>& vec1, const std::vector<float>& vec2) {
	std::vector<float> out;
	for (int i = 0; i < vec1.size(); i++) {
		out.push_back(vec1[i] - vec2[i]);
	}
	return out;
}

std::vector<float> Ransac::VecCross(const std::vector<float>& vec1, const std::vector<float>& vec2) {
	std::vector<float> out(3, 1);
	out[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	out[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	out[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	if (out[0] < 0) {
		out[0] = -out[0];
		out[1] = -out[1];
		out[2] = -out[2];
	}
	return out;
}

float Ransac::VecDot(const std::vector<float>& vec1, const std::vector<float>& vec2) {
	float out = 0.0;
	for (int i = 0; i < vec1.size(); i++) {
		out = out + vec1[i] * vec2[i];
	}
	return out;
}


int Ransac::CountPoint(const std::vector<std::vector<float>>& points,
		const std::vector<float>& point, const std::vector<float>& N, float sigma) {
	int n = points.size();
	float norm = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]+0.00001);
	int num = 0;
	float d = 0.0;
	for (int i = 0; i < n; i++) {
		std::vector<float>f = VecSubtraction(points[i], point);
		d = fabs(VecDot(f, N) / norm);
		if (d <= sigma) num++;
	}
	return num;
}

std::vector<int > Ransac::VecRandperm(int n){
	std::vector<int>out(3, 0);
	int tmp = 0;
	std::vector<int> temp;
	for (int i = 0; i < 10; ++i) {
		temp.push_back(i);
	}
	for (int i = 0; i < 4; i++) {
		random_shuffle(temp.begin(), temp.end());
		out[0] = out[0] * 10 + temp[0];
		out[1] = out[1] * 10 + temp[1];
		out[2] = out[2] * 10 + temp[2];
	}
	float scale = float(n) / 10000.;
	out[0] = int(float(out[0])*scale);
	out[1] = int(float(out[1])*scale);
	out[2] = int(float(out[2])*scale);
	return out;
}

std::vector<float> Ransac::CalPlane(const std::vector<std::vector<float>>& points) {
	float best_a = 0.0;
	float best_b = 0.0;
	float best_c = 0.0;
	float best_d = 0.0;
	float P = 0.99;
	int pretotal = 0;
	int n = points.size();
	int iters = 100;
	float sigma = 0.01;
	float outline_ratio = 0.4;
	for (int i = 0; i < iters; i++) {
		std::vector<int>ps = VecRandperm(points.size());
		std::vector<float>point1 = points[ps[0]];
		std::vector<float>point2 = points[ps[1]];
		std::vector<float>point3 = points[ps[2]];
		std::vector<float>point1_2 = VecSubtraction(point1, point2);
		std::vector<float>point1_3 = VecSubtraction(point1, point3);
		std::vector<float>N = VecCross(point1_3, point1_2);
		float a = N[0];
		float b = N[1];
		float c = N[2];
		float d = -1 * VecDot(N, point1);
		int nowtotal = CountPoint(points, point1, N, sigma);
		if (nowtotal >= pretotal) {
			pretotal = nowtotal;
			best_a = a;
			best_b = b;
			best_c = c;
			best_d = d;
			if (pretotal >= points.size()*(1 - outline_ratio)) break;
		}
	}
	if (best_a < 0) {
		best_a = -best_a;
		best_b = -best_b;
		best_c = -best_c;
		best_d = -best_d;
	}
	return std::vector<float>{best_a, best_b, best_c, best_d};
}

}
