#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
namespace ransac {
class Ransac {
public:
	/**
	 * @brief ����ʵ����
	 */
	Ransac();
	/**
	 * @brief �ṩdemo�Ĳ�������
	 * @param plane gt
	 */
	void TestDemo(const std::vector<float>& plane);

private:
	/**
	 * @brief ����ƽ���
	 * @param {plane, n}
	 * @return n*n sampled points depends on plane parameters
	 */
	std::vector<std::vector<float>> SamplePoints(const std::vector<float>& plane, int n);
	/**
	 * @brief �ṩ�����������㹦��
	 * @param vec1, vec2
	 * @return vec1-vec2
	 */
	std::vector<float> VecSubtraction(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief �ṩ����������㹦��
	 * @param vec1, vec2
	 * @return vec1 (X) vec2
	 */
	std::vector<float> VecCross(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief �ṩ����������㹦��
	 * @param vec1, vec2
	 * @return vec1 (��) vec2
	 */
	float VecDot(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief �������ڵ�ǰƽ���ĸ���
	 * @param {points, point, N, sigma}
	 * @return �뵱ǰ���ƽ�������sigma�ڵĵ������
	 */
	int CountPoint(const std::vector<std::vector<float>>& points, 
		const std::vector<float>& point, const std::vector<float>& N, float sigma);
	/**
	 * @brief �ṩ���������
	 * @param n
	 * @return ����3������{0, n-1}��Χ�ڵ������
	 */
	std::vector<int > VecRandperm(int n);
	/**
	 * @brief ���ƽ��
	 * @param points
	 * @return ƽ�����{a, b, c, d}
	 */
	std::vector<float> CalPlane(const std::vector<std::vector<float>>& points);

};
}
