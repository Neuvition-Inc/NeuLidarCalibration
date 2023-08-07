#pragma once

#include <QObject>
#include "pcl_common_defs.h"
#include <opencv2/opencv.hpp>
//#include "CCube.h"
#include "CViewController.h"

class CGlobalSignalSlot : public QObject
{
	Q_OBJECT

public:
	
	static CGlobalSignalSlot* getInstance() 
	{
		static CGlobalSignalSlot  instance;
		return &instance;
	}

	~CGlobalSignalSlot();

Q_SIGNALS:
	void sigUpdateRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRotationCloud);
	void sigAddRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId, int nR, int nG, int nB);
	void sigAddRenderCloudWithSelfColor(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId,int nRenderSize);
	void sigDrawCircleCloud(Eigen::VectorXf circle_coeff);
	void sigDrawCircleNoTextWithId(Eigen::VectorXf circle_coeff, QString sId);
	void sigAddCloudWithText(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud, QString sText, QString sId, int nRenderSize, int nFontSize);

	//void sigDrawCube(CCube cube,QString sCubeId);
	//void sigDrawCubeWithColor(CCube cube, QString sCubeId, int nR, int nG, int nB);

	void sigClearAllClouds();
	void sigClearCloudById(QString sId);

	void sigShowText(QString sText, QString sId, int nXPos, int nYPos);
	void sigShowErrorInfo(QString sText, bool bError);

	void sigAddArrow(pcl::PointXYZ a, pcl::PointXYZ b, int nR, int nG, int nB, QString sId);

	void sigUpdateImageByCamera1(cv::Mat mat);

	void sigMat(cv::Mat mat);

	void sigSendVolResult(float fVol);

	void sigCameraPosition(st_PCL_CAMERA_POSITION stCameraPos);

	void sigMergeFinishCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pMergeCloud);


	void sigDisConnModbus();
private:
	CGlobalSignalSlot(QObject *parent = nullptr);
};
