
#include "CalibrationAlgorithm.h"

CAlgorithm::CAlgorithm() : 
	rootNode(NULL),
	pointsSize(0)
{

}
CAlgorithm::~CAlgorithm() {
	kdtree_clear();
}

void CAlgorithm::node_clear(TreeNode *treeNode) {
	if (treeNode) {
		node_clear(treeNode->left_node);
		node_clear(treeNode->right_node);
		free(treeNode);
		treeNode = NULL;
	}
}

void CAlgorithm::kdtree_clear() {
	if (rootNode) {
		node_clear(rootNode);
	}
}

void CAlgorithm::kdtree_create(const PointCloud<NeuvPclPoint>::Ptr &inputcloud) {
	vector<TreeNode *> treeNodes;
	if (!inputcloud) {
		std::cout << "invalid input." << std::endl;
		return;
	}
	if (inputcloud->size() <= 0) {
		std::cout << "size <= 0." << std::endl;
		return;
	}
	std::cout << " size: " << inputcloud->size() << std::endl;
	vector<float> sort_values;
	vector<NeuvPclPoint> sort_points;
	for (PointCloud<NeuvPclPoint>::const_iterator iter = inputcloud->begin(); iter != inputcloud->end(); ++iter) {
		const NeuvPclPoint &pt = *iter;
		int index = lower_bound(sort_values.begin(), sort_values.end(), pt.x) - sort_values.begin();
		sort_values.insert(sort_values.begin() + index, pt.x);
		sort_points.insert(sort_points.begin() + index, pt);
	}
	rootNode = (TreeNode *)malloc(sizeof(TreeNode));
	memset(rootNode, 0, sizeof(rootNode));
	rootNode->cur_point = sort_points[sort_points.size() / 2];
	rootNode->node_points[0] = new vector<NeuvPclPoint>;
	rootNode->node_points[1] = new vector<NeuvPclPoint>;
	rootNode->node_points[0]->assign(sort_points.begin(), sort_points.begin() + sort_points.size() / 2);
	rootNode->node_points[1]->assign(sort_points.begin() + sort_points.size() / 2 + 1, sort_points.end());
	rootNode->layer = 0;
	rootNode->left_node = NULL;
	rootNode->right_node = NULL;
	treeNodes.push_back(rootNode);
	while (true)
	{
		vector<TreeNode *> tempNodes;
		if (treeNodes.size() <= 0)
			break;
		tempNodes.swap(treeNodes);
		for (int i = 0; i < tempNodes.size(); i++)
		{
			TreeNode *node = tempNodes[i];
			int layer = node->layer + 1;
			for (int j = 0; j < 2; j++)
			{
				vector<NeuvPclPoint> *points = node->node_points[j];
				if (points != NULL) {
					if (points->size() > 0) {
						vector<float> temp_sort_values;
						vector<NeuvPclPoint> temp_sort_points;
						for (vector<NeuvPclPoint>::const_iterator iter = points->begin(); iter != points->end(); ++iter) {
							const NeuvPclPoint &pt = *iter;
							float value;
							if (layer % 3 == 0) {
								value = pt.x;
							}
							else if (layer % 3 == 1) {
								value = pt.y;
							}
							else {
								value = pt.z;
							}
							int index = lower_bound(temp_sort_values.begin(), temp_sort_values.end(), value) - temp_sort_values.begin();
							temp_sort_values.insert(temp_sort_values.begin() + index, value);
							temp_sort_points.insert(temp_sort_points.begin() + index, pt);
						}

						TreeNode *curNode = (TreeNode *)malloc(sizeof(TreeNode));
						memset(curNode, 0, sizeof(curNode));
						curNode->cur_point = temp_sort_points[temp_sort_points.size() / 2];
						curNode->node_points[0] = new vector<NeuvPclPoint>;
						curNode->node_points[1] = new vector<NeuvPclPoint>;
						curNode->node_points[0]->assign(temp_sort_points.begin(), temp_sort_points.begin() + temp_sort_points.size() / 2);
						curNode->node_points[1]->assign(temp_sort_points.begin() + temp_sort_points.size() / 2 + 1, temp_sort_points.end());
						curNode->layer = layer;
						curNode->left_node = NULL;
						curNode->right_node = NULL;
						treeNodes.push_back(curNode);
						if (j == 0) {
							tempNodes[i]->left_node = curNode;
						}
						if (j == 1) {
							tempNodes[i]->right_node = curNode;
						}
					}

					tempNodes[i]->node_points[j]->clear();
					delete tempNodes[i]->node_points[j];
					tempNodes[i]->node_points[j] = NULL;
				}
			}

		}
	}
}

void CAlgorithm::kdtree_search(const NeuvPclPoint &search_point, float search_radius, vector<NeuvPclPoint>& outcloud) {
	vector<TreeNode *> searchNodes;
	searchNodes.push_back(rootNode);
	while (true)
	{
		vector<TreeNode *> tempNodes;
		if (searchNodes.size() <= 0)
			break;
		tempNodes.swap(searchNodes);
		for (int i = 0; i < tempNodes.size(); i++)
		{
			TreeNode * node = tempNodes[i];
			int layer = node->layer;
			float node_value, search_value, diff_value;
			if (layer % 3 == 0) {
				node_value = node->cur_point.x;
				search_value = search_point.x;
			}
			else if (layer % 3 == 1) {
				node_value = node->cur_point.y;
				search_value = search_point.y;
			}
			else {
				node_value = node->cur_point.z;
				search_value = search_point.z;
			}
			diff_value = node_value - search_value;
			if (fabs(diff_value) <= search_radius) {
				float diff_dis = sqrtf((node->cur_point.x - search_point.x) * (node->cur_point.x - search_point.x) +
					(node->cur_point.y - search_point.y) * (node->cur_point.y - search_point.y) +
					(node->cur_point.z - search_point.z) * (node->cur_point.z - search_point.z));
				if (diff_dis <= search_radius) {
					outcloud.push_back(node->cur_point);
					//std::cout << "layer : " << layer << " " << node->cur_point.x << " " << node->cur_point.y << " " << node->cur_point.z << std::endl;
				}
				if (node->left_node) {
					searchNodes.push_back(node->left_node);
				}
				if (node->right_node) {
					searchNodes.push_back(node->right_node);
				}
			}
			else if (diff_value - search_radius > 0) {
				if (node->left_node) {
					searchNodes.push_back(node->left_node);
				}
			}
			else if (diff_value + search_radius < 0) {
				if (node->right_node) {
					searchNodes.push_back(node->right_node);
				}
			}

		}
	}
}

void CAlgorithm::cloud_cluster_by_distance(PointCloud<NeuvPclPoint> *input_cloud, int index, float radius, PointCloud<NeuvPclPoint> &out_cloud) {
	vector<NeuvPclPoint> cloud_search;
	std::vector<int> k_indices;
	std::vector<float> k_sqr_distances;
	input_cloud->points[index].label = 1;
	out_cloud.push_back(input_cloud->points[index]);
	cloud_search.push_back(input_cloud->points[index]);
	pointsSize = 0;
	for (;;) {
		vector<NeuvPclPoint> temp_cloud;
		if (cloud_search.size() <= 0)
			break;
		pointsSize += cloud_search.size();
		std::cout << " cluster, size: " << pointsSize << std::endl;
		cloud_search.swap(temp_cloud);
		for (vector<NeuvPclPoint>::const_iterator iter = temp_cloud.begin(); iter != temp_cloud.end(); iter++) {
			vector<NeuvPclPoint> kdsearch;
			const NeuvPclPoint& pt = *iter;
			kdtree_search(pt, radius, kdsearch);
			for (vector<NeuvPclPoint>::const_iterator kditer = kdsearch.begin(); kditer != kdsearch.end(); kditer++) {
				const NeuvPclPoint& kdpt = *kditer;
				if (input_cloud->points[kdpt.label].label == 1)
					continue;
				input_cloud->points[kdpt.label].label = 1;
				out_cloud.push_back(input_cloud->points[kdpt.label]);
				cloud_search.push_back(input_cloud->points[kdpt.label]);
			}
		}
	}
}
void CAlgorithm::plane_fitting(const vector<PointXYZ> &inputcloud, cv::OutputArray dst) {
	int n = 0;
	double coeff[8];
	memset(&coeff[0], 0, sizeof(double) * 8);
	cv::Mat coffeeMat = dst.getMat();
	cv::Mat srcMat = cv::Mat::zeros(3, 3, CV_64F);
	cv::Mat invMat = cv::Mat::zeros(3, 3, CV_64F);
	cv::Mat outMat = cv::Mat::zeros(3, 1, CV_64F);
	for (vector<PointXYZ>::const_iterator iter = inputcloud.begin(); iter != inputcloud.end(); iter++) {
		const PointXYZ &pt = *iter;
		coeff[0] += static_cast<double>(pt.x * pt.x);
		coeff[1] += static_cast<double>(pt.x * pt.y);
		coeff[2] += static_cast<double>(pt.x);
		coeff[3] += static_cast<double>(pt.y * pt.y);
		coeff[4] += static_cast<double>(pt.y);
		coeff[5] += static_cast<double>(pt.x * pt.z);
		coeff[6] += static_cast<double>(pt.y * pt.z);
		coeff[7] += static_cast<double>(pt.z);
		n++;
	}
	srcMat.at<double>(0, 0) = coeff[0];
	srcMat.at<double>(0, 1) = coeff[1];
	srcMat.at<double>(0, 2) = coeff[2];
	srcMat.at<double>(1, 0) = coeff[1];
	srcMat.at<double>(1, 1) = coeff[3];
	srcMat.at<double>(1, 2) = coeff[4];
	srcMat.at<double>(2, 0) = coeff[2];
	srcMat.at<double>(2, 1) = coeff[4];
	srcMat.at<double>(2, 2) = n;
	for (size_t i = 0; i < 3; i++) {
		outMat.at<double>(i, 0) = coeff[5 + i];
	}
	cv::invert(srcMat, invMat, cv::DECOMP_LU);
	coffeeMat = invMat * outMat;
}

void CAlgorithm::plane_fitting1(const PointCloud<PointXYZRGB> &inputcloud, cv::OutputArray dst) {
	vector<PointXYZ> simpleCloudPoint;
	for (PointCloud<PointXYZRGB>::const_iterator iter = inputcloud.begin(); iter != inputcloud.end(); iter++) {
		const PointXYZRGB &pt = *iter;
		PointXYZ ipt;
		ipt.x = pt.x; ipt.y = pt.y; ipt.z = pt.z;
		simpleCloudPoint.push_back(ipt);
	}
	plane_fitting(simpleCloudPoint, dst);
}

void CAlgorithm::plane_fitting2(const PointCloud<NeuvPclPoint> &inputcloud, cv::OutputArray dst) {
	vector<PointXYZ> simpleCloudPoint;
	for (PointCloud<NeuvPclPoint>::const_iterator iter = inputcloud.begin(); iter != inputcloud.end(); iter++) {
		const NeuvPclPoint &pt = *iter;
		PointXYZ ipt;
		ipt.x = pt.x; ipt.y = pt.y; ipt.z = pt.z;
		simpleCloudPoint.push_back(ipt);
	}
	plane_fitting(simpleCloudPoint, dst);
}

cv::Mat CAlgorithm::eulerAnglesToRotationMatrix(cv::Vec3f &theta) {
	// Calculate rotation about x axis
	cv::Mat R_x = (cv::Mat_<float>(3, 3) <<
		1, 0, 0,
		0, cos(theta[0]), -sin(theta[0]),
		0, sin(theta[0]), cos(theta[0])
		);
	// Calculate rotation about y axis
	cv::Mat R_y = (cv::Mat_<float>(3, 3) <<
		cos(theta[1]), 0, sin(theta[1]),
		0, 1, 0,
		-sin(theta[1]), 0, cos(theta[1])
		);
	// Calculate rotation about z axis
	cv::Mat R_z = (cv::Mat_<float>(3, 3) <<
		cos(theta[2]), -sin(theta[2]), 0,
		sin(theta[2]), cos(theta[2]), 0,
		0, 0, 1
		);
	// Combined rotation matrix
	cv::Mat R = R_z * R_y * R_x;
	return R;
}

cv::Vec3f CAlgorithm::rotationMatrixToEulerAngles(cv::Mat &R) {
	float sy = sqrt(R.at<float>(0, 0) * R.at<float>(0, 0) + R.at<float>(1, 0) * R.at<float>(1, 0));
	bool singular = sy < 1e-6; // If
	float x, y, z;
	if (!singular) {
		x = atan2(R.at<float>(2, 1), R.at<float>(2, 2));
		y = atan2(-R.at<float>(2, 0), sy);
		z = atan2(R.at<float>(1, 0), R.at<float>(0, 0));
	}
	else {
		x = atan2(-R.at<float>(1, 2), R.at<float>(1, 1));
		y = atan2(-R.at<float>(2, 0), sy);
		z = 0;
	}
#if 1
	x = x * 180.0f / 3.141592653589793f;
	y = y * 180.0f / 3.141592653589793f;
	z = z * 180.0f / 3.141592653589793f;
#endif
	return cv::Vec3f(x, y, z);
}

void CAlgorithm::curve_fitting(std::vector<double> xvec, std::vector<double> yvec, cv::OutputArray dst, int factor = 2) {
	double cf[10];
	int n = 0;
	int steps = 3;
	cv::Mat cfMat = dst.getMat();
	memset(&cf[0], 0, sizeof(double) * 10);
	if (factor == 1) {
		steps = 2;
		for (int j = 0; j < xvec.size(); ++j)
		{
			double x = xvec[j];
			double y = yvec[j];
			cf[0] += x * x;
			cf[1] += x;
			cf[2] += x * y;
			cf[3] += y;
			n++;
		}
	}
	if (factor == 2) {
		steps = 3;
		for (int j = 0; j < xvec.size(); ++j)
		{
			double x = xvec[j];
			double y = yvec[j];
			cf[0] += x * x * x * x;
			cf[1] += x * x * x;
			cf[2] += x * x;
			cf[3] += x;
			cf[4] += x * x * y;
			cf[5] += x * y;
			cf[6] += y;
			n++;
		}
	}
	else if (factor == 3) {
		steps = 4;
		for (int j = 0; j < xvec.size(); ++j)
		{
			double x = xvec[j];
			double y = yvec[j];
			cf[0] += x * x * x * x * x * x;
			cf[1] += x * x * x * x * x;
			cf[2] += x * x * x * x;
			cf[3] += x * x * x;
			cf[4] += x * x;
			cf[5] += x;
			cf[6] += x * x * x * y;
			cf[7] += x * x * y;
			cf[8] += x * y;
			cf[9] += y;
			n++;
		}
	}
	cv::Mat srcMat = cv::Mat::zeros(steps, steps, CV_64F);
	cv::Mat invMat = cv::Mat::zeros(steps, steps, CV_64F);
	cv::Mat outMat = cv::Mat::zeros(steps, 1, CV_64F);
	for (int k = 0; k < steps; ++k)
	{
		for (int t = k; t < k + steps; ++t)
		{
			if ((k == steps - 1) && (t == 2 * steps - 2)) {
				srcMat.at<double>(k, t - k) = n;
				continue;
			}
			srcMat.at<double>(k, t - k) = cf[t];
		}
	}
	for (int k = 0; k < steps; ++k)
	{
		outMat.at<double>(k, 0) = cf[k + 2 * steps - 2];
	}
	cv::invert(srcMat, invMat, cv::DECOMP_LU);
	cfMat = invMat * outMat;
	//std::cout << " cfMat: " << cfMat << std::endl;
}
