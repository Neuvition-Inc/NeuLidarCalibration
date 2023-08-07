#pragma once


#include "neuv_defs.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>

#include <QApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>


// Point Cloud Library
//#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/search/kdtree.h>
#include <pcl/segmentation/extract_clusters.h>
#include "CalibrationAlgorithm.h"


#define TEST_OPEN 1
#if TEST_OPEN
#define MAIN_WIN 0
#else
#define MAIN_WIN 1
#endif

#define P_A 0.017453
#define A_P 57.295779

enum cal_code
{
	CAL_ORIGIN_COR = 1,
	CAL_DEPTH_COR = 2,
	CAL_AZIMUTH_COR = 3,
	CAL_OTHERS_COR = 4,
	CAL_DEPTH_Z_COR = 5,
	CAL_TOF_COR = 6,
	CAL_CAMERA_LIDAR = 11,
	CAL_EXTERNAL_CAMERA_LIDAR = 12,
	CAL_EXTERNAL_CAMERA_CAMERA = 13

};

class CalibrationProcess
{
public:
	//int -> float 8.11
	typedef struct _DepthDataNode { 
		uint8_t laserId;
		float laserDepth; 
		float lidarDepth; }DepthDataNode;
public:
	CalibrationProcess();
	~CalibrationProcess();
	QString GetDeviceName();
	void SetSerialNumber(const char *serialNumber);
	char* GetSerialNumber();
	bool CreateCalibrationLog(const char *fileName);
	bool OriginCorrection(const neuvition::NeuvUnits& points);
	int DepthCollection(const neuvition::NeuvUnits& points, int laserDepth);
	bool DepthCorrection();
	void ClearDepthData();

	//平面数据采集处理
	int SurfaceCollection(const neuvition::NeuvUnits& points, float laserDepth,float hAngle,float vAngle);
	bool SurfaceCalibration();
	void ClearSurfaceData();

	void SetOriginColRow(int col,int row);

	float getLidarYawAngle(float z, float ptzYaw);
	float getLidarPitchAngle(float z, float ptzPitch);

	void GetRotateOrigin(const neuvition::NeuvUnits& points, float horAngle,float vorAngle, pcl::PointXYZL& poi);
	void AppendSurfaceCorPoint(neuvition::NeuvUnit poi);

	void CalculatFitCoeff(int poiNum, int lid, neuvition::NeuvUnits pois);
	void updata();
	void updataScaleXY(float x,float y);

	void SetBordCutNum(int num);

private:
	std::ofstream ofStream;
	char lidarSerialNumber[32];
	int depthCount;
	DepthDataNode depthDataNodes[30];

	neuvition::NeuvUnits mSurfaceCorPoints;
	std::ofstream mBofs;

	int mOriginCol = 900;
	int mOriginRow = 360;

	int mBordCutNum = 0;

	bool tofCorEnabled = false;
	cv::Mat  tofCorCoeffsMat;
};
