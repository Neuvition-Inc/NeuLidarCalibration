#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>

struct NeuvPclPoint
{
	PCL_ADD_POINT4D; // This adds the members x,y,z which can also be accessed using the point (which is float[4])
	PCL_ADD_UNION_RGB;
	uint32_t tof;
	uint16_t row;
	uint16_t col;
	uint8_t intensity;
	uint32_t label;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
}EIGEN_ALIGN16;



POINT_CLOUD_REGISTER_POINT_STRUCT(NeuvPclPoint,
(float, x, x)
(float, y, y)
(float, z, z)
(uint32_t, rgba, rgba)
(uint32_t, tof, tof)
(uint16_t, row, row)
(uint16_t, col, col)
(uint8_t, intensity, intensity)
(uint32_t, label, label)
)

using namespace std;
using namespace pcl;
using namespace cv;

typedef struct _TreeNode {
	int layer;
	NeuvPclPoint cur_point;
	vector<NeuvPclPoint> *node_points[2];
	struct _TreeNode *left_node;
	struct _TreeNode *right_node;
}TreeNode;


class CAlgorithm
{
public:
	CAlgorithm();
	~CAlgorithm();
	void node_clear(TreeNode *treeNode);
	void kdtree_clear();
	void kdtree_create(const PointCloud<NeuvPclPoint>::Ptr &inputcloud);
	void kdtree_search(const NeuvPclPoint &search_point, float search_radius, vector<NeuvPclPoint>& outcloud);
	void cloud_cluster_by_distance(PointCloud<NeuvPclPoint> *input_cloud, int index, float radius, PointCloud<NeuvPclPoint> &out_cloud);
	void plane_fitting(const vector<PointXYZ> &inputcloud, cv::OutputArray dst);
	void plane_fitting1(const PointCloud<PointXYZRGB> &inputcloud, cv::OutputArray dst);
	void plane_fitting2(const PointCloud<NeuvPclPoint> &inputcloud, cv::OutputArray dst);
	void curve_fitting(std::vector<double> xvec, std::vector<double> yvec, cv::OutputArray dst, int factor);
	cv::Mat eulerAnglesToRotationMatrix(cv::Vec3f &theta);
	cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R);
private:
	TreeNode *rootNode;
	int pointsSize;
};
