#include "CGlobalSignalSlot.h"

CGlobalSignalSlot::CGlobalSignalSlot(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<pcl::PointXYZ>("pcl::PointXYZ");
	//qRegisterMetaType<CCube>("CCube");
	qRegisterMetaType<Eigen::VectorXf>("Eigen::VectorXf");
	qRegisterMetaType<pcl::PointCloud<pcl::PointXYZRGBA>::Ptr>("pcl::PointCloud<pcl::PointXYZRGBA>::Ptr");
	qRegisterMetaType<st_PCL_CAMERA_POSITION>("st_PCL_CAMERA_POSITION");
}   



CGlobalSignalSlot::~CGlobalSignalSlot()
{
}
