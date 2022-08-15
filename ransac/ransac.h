#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
namespace ransac {
class Ransac {
public:
	/**
	 * @brief 构造实体类
	 */
	Ransac();
	/**
	 * @brief 提供demo的测试样例
	 * @param plane gt
	 */
	void TestDemo(const std::vector<float>& plane);

private:
	/**
	 * @brief 采样平面点
	 * @param {plane, n}
	 * @return n*n sampled points depends on plane parameters
	 */
	std::vector<std::vector<float>> SamplePoints(const std::vector<float>& plane, int n);
	/**
	 * @brief 提供向量减法运算功能
	 * @param vec1, vec2
	 * @return vec1-vec2
	 */
	std::vector<float> VecSubtraction(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief 提供向量叉乘运算功能
	 * @param vec1, vec2
	 * @return vec1 (X) vec2
	 */
	std::vector<float> VecCross(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief 提供向量点乘运算功能
	 * @param vec1, vec2
	 * @return vec1 (·) vec2
	 */
	float VecDot(const std::vector<float>& vec1, const std::vector<float>& vec2);
	/**
	 * @brief 计算落在当前平面点的个数
	 * @param {points, point, N, sigma}
	 * @return 与当前拟合平面距离在sigma内的点的数量
	 */
	int CountPoint(const std::vector<std::vector<float>>& points, 
		const std::vector<float>& point, const std::vector<float>& N, float sigma);
	/**
	 * @brief 提供随机数向量
	 * @param n
	 * @return 返回3个落在{0, n-1}范围内的随机数
	 */
	std::vector<int > VecRandperm(int n);
	/**
	 * @brief 拟合平面
	 * @param points
	 * @return 平面参数{a, b, c, d}
	 */
	std::vector<float> CalPlane(const std::vector<std::vector<float>>& points);

};
}
