#pragma once

//#include "../common.h"
#include <string.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>
#include <vector>
#include <QString>

typedef pcl::PointXYZRGBA  PointT;
//typedef pcl::PointCloud<PointT> PointTCloud;
//typedef pcl::PointCloud<PointT>::Ptr  PointTCloud_Ptr;
typedef pcl::visualization::PCLVisualizer  PCLViewer;
typedef pcl::visualization::PCLVisualizer::Ptr   PCLViewer_Ptr;

using namespace std;

std::string getUuid();
/************************************************************************/
/*  �ṩ���ֻ��ƹ���   ������ƽ��ȵ�                                   */
/************************************************************************/


#include "pcl_extern_func.h"

enum class enPlaneType
{
	VERTICAL_X = 1,  //ƽ�洹ֱ��X��
	VERTICAL_Y,   //ƽ�洹ֱ��Y��
	VERTICAL_Z  //ƽ�洹ֱ��Z��
};

class CPCLPainter
{

public:
	CPCLPainter();
	~CPCLPainter();

	//�����߶�
	string drawLine(double x0, double y0, double z0, double x1, double y1, double z1, boost::shared_ptr<pcl::visualization::PCLVisualizer> pViewer, double r = 255.0, double g = 255.0, double b = 255.0);
	
	//���Ƶ�
	string drawPoint(double x, double y, double z, PCLViewer_Ptr& pViewer, int r, int g, int b, int nSize);
 

	//����ƽ��
	std::string drawPlane(pcl::ModelCoefficients& plane_coeff, pcl::visualization::PCLVisualizer::Ptr pPCLViewer, float scale[2] = nullptr, int viewport = 0);

	std::string drawPlane(pcl::ModelCoefficients& plane_coeff, pcl::visualization::PCLVisualizer::Ptr pPCLViewer,double x,double y, double z, float scale[2] = nullptr, int viewport = 0);

	//Vertical z axis
	std::string drawPlaneVerticalZAxisByCloud(
		float fx0, float fy0, float fx1, float fy1, float fz,
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pOutCloud,
		pcl::visualization::PCLVisualizer::Ptr pPCLViewer = nullptr,
		float fXPrec = 0.01,
		float fYPrec = 0.01,
		int nR = 255, int nG = 0, int nB = 0,
		int viewport = 0
		);


	//���� Eigen::VectorXf& circle_coeff ���Ƴ� 3dԲȦ�ĵ��Ƽ���
	std::string draw3dCirclePointCloud(
		Eigen::VectorXf& circle_coeff, 
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud,
		pcl::visualization::PCLVisualizer::Ptr pPCLViewer = nullptr,
		int nR = 255, int nG = 255, int nB = 255,
		int viewport = 0
	);
	std::string draw3dCirclePointCloud(
		Eigen::VectorXf& circle_coeff,
		pcl::visualization::PCLVisualizer::Ptr pPCLViewer = nullptr,
		int nR = 255, int nG = 255, int nB = 255,
		int viewport = 0
	);

	void draw3dCirclePointCloudNoText(
		Eigen::VectorXf& circle_coeff,
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud,
		QString sId,
		pcl::visualization::PCLVisualizer::Ptr pPCLViewer = nullptr,
		int nR = 255, int nG = 255, int nB = 255,
		int viewport = 0
	);
	void draw3dCirclePointCloudNoText(
		Eigen::VectorXf& circle_coeff,
		QString sId,
		pcl::visualization::PCLVisualizer::Ptr pPCLViewer = nullptr,
		int nR = 255, int nG = 255, int nB = 255,
		int viewport = 0
	);
};

