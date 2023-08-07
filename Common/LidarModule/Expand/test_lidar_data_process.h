#pragma once

#include <neuv_defs.hpp>
#include <iostream>

#include <QFile>
#include <queue>
#include <QTime>

#include <QString>
#include <QCoreApplication>



//===============================
//bool  g_bImuRead = false;
//double g_dSumImu_roll = 0.0;
//double g_dSumImu_yaw = 0.0;
//double g_dSumImu_pitch = 0.0;
//int    g_nCoundImu = 0;

//===============================
//#include "../NewPczModule/CPczWriteThread.h"
//extern QQueue<neuwPczData> g_pointcloud_queue;
//#include "../SyncDataGetModule/CGlobalDataGetInstance.h"
//extern bool g_bLoad;
////==============================
//extern bool c_dis_cor_enabled;
//
//
//bool c_cloudcopy_enabled = false;
//neuvition::NeuvUnits c_cloudunits;
//CVTKWidget*			m_pVTKWidget = NULL;
//extern bool c_pulse_filter_enabled;
//extern int c_pulse_filter_value;
//bool isdistancelimit = false;
//float fdistace1;
//float fdistace2;
////摄像头数据
//
////
//static unsigned int mTestMinPw = 0;
//static unsigned int mTestMaxPw = 255;
//===============================

const int CONNECT_STATE_CONNECTING = 0;
const int CONNECT_STATE_CONNECTED = 1;
const int CONNECT_STATE_DISCONNECTED = -1;

static int mConnectState = CONNECT_STATE_DISCONNECTED;


//int LadarRenderMode = 1;
enRenderMode g_enLadarRenderMode = POLYCHROM;
//RGB
const static uint8_t COLOR_R = 0xF5;
const static uint8_t COLOR_G = 0xFF;
const static uint8_t COLOR_B = 0x00;
#define COLOR_RANGE_SIZE 3
//point color cycle
static int mTofColorCycle = 65000;
//green, yellow, red
const uint32_t coloredge[3] = { 0x00ff00, 0xffff00, 0xff0000 };


std::string g_curFrame_pcd_path = "";
std::string g_curFrame_jpg_path = "";
queue<std::string>  g_queue_pcdFileName;
queue<std::string>  g_queue_jpgFileName;


int Ladarpointsize = 1;

int iSaveSt = 0;
int iSaveSt_image = 0;
///////////////////////////////////////////////////////////////////////////////////////////////
//CLadarControl  g_LadarControl;
//boost::mutex cloudMutex;

//extern cv::Mat  mJpegMat;
//extern neucalibration *m_neuclibration;

//
//neuwPczData  g_PrePczData;
//bool    g_bFirst = true;

////////////////////////////////////////////////////////////////////////////////////////////////




void test_on_framedata(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id) 
{
//	//LOG(INFO) << "LogModule is Open! --------------------------";
//
//	if (data.size() == 0) return;
//
//
//
//	pcl::PointCloud<PointXYZRGBATI> cloud_;
//	cloud_.reserve(data.size());
//	cloud_.is_dense = false;
//
//
//	for (neuvition::NeuvUnits::const_iterator iter = data.begin(); iter != data.end(); iter++) {
//		const neuvition::NeuvUnit& np = (*iter);
//		PointXYZRGBATI point;
//		//NeuLogERROR("Log ERROR -------------------");
//		//LOG(ERROR) << "Log ERROR22 -------------------";
//
//
//		switch (g_enLadarRenderMode)
//		{
//		case POLYCHROM:
//		{
//			int colorrange = mTofColorCycle / COLOR_RANGE_SIZE;
//			int tofcolor = np.tof%mTofColorCycle;
//			int tofclass = tofcolor / (colorrange);
//			int tofreman = tofcolor % (colorrange);
//			if (tofclass >= COLOR_RANGE_SIZE) tofclass = COLOR_RANGE_SIZE - 1;
//			uint32_t cbase = coloredge[(tofclass + 0) % COLOR_RANGE_SIZE];
//			uint32_t cnext = coloredge[(tofclass + 1) % COLOR_RANGE_SIZE];
//			int rbase = (cbase & 0xff0000) >> 16;
//			int gbase = (cbase & 0x00ff00) >> 8;
//			int bbase = (cbase & 0x0000ff) >> 0;
//			int rnext = (cnext & 0xff0000) >> 16;
//			int gnext = (cnext & 0x00ff00) >> 8;
//			int bnext = (cnext & 0x0000ff) >> 0;
//			int rdelay = (rnext - rbase) * tofreman / colorrange;
//			int gdelay = (gnext - gbase) * tofreman / colorrange;
//			int bdelay = (bnext - bbase) * tofreman / colorrange;
//			point.r = (rbase + rdelay) & 0xFF;
//			point.g = (gbase + gdelay) & 0xFF;
//			point.b = (bbase + bdelay) & 0xFF;
//		}
//		break;
//		case CAMARAMODE:
//		{
//			point.r = np.r; point.g = np.g; point.b = np.b;
//		}
//		break;
//		case REFLECTIVITYMODE:
//		{
//			int tofcolor = np.intensity;
//
//			if (tofcolor >= 255) {
//				point.r = 255;
//				point.g = 0;
//				point.b = 0;
//			}
//			else {
//				point.r = tofcolor;
//				point.g = 255 - tofcolor;
//				point.b = 0;
//			}
//		}
//		break;
//		case MONOCHROM:
//			point.b = COLOR_B; point.g = COLOR_G; point.r = COLOR_R;
//			break;
//		default:
//			point.b = COLOR_B; point.g = COLOR_G; point.r = COLOR_R;
//			break;
//		}
//
//
//
//		point.x = np.x*0.001;
//		point.y = np.y*0.001;
//		point.z = np.z*0.001;
//		point.a = 255;
//		// 如果设置摄像头开启，并开启图像获取，则RGB为真实颜色，否则为128
//
//		point.intensity = np.intensity;
//		point.time_sec = np.time_sec;
//		if (c_dis_cor_enabled)
//		{
//			m_pVTKWidget->config_c_area_color(np, point.r, point.g, point.b);
//		}
//		cloud_.push_back(point);
//
//		if (m_neuclibration->mRenderMode == 0)
//		{ /* 0:monochrome */
//			point.b = COLOR_B;
//			point.g = COLOR_G;
//			point.r = COLOR_R;
//		}
//
//
//	}
//	size_t size = cloud_.size();
//
//	if (size > 0)
//	{
//		boost::mutex::scoped_lock lockit(cloudMutex);
//		//g_LadarControl.m_pRenderCloud->reserve(size);
//		PointCloudT::Ptr pRenderCloud(new PointCloudT);
//
//		pcl::copyPointCloud(cloud_, *pRenderCloud);
//
//		PointCloudT::Ptr pRenderCloud2(new PointCloudT);
//		pRenderCloud2->swap(*pRenderCloud);
//
//
//
//		g_LadarControl.updateRenderPointCloud(pRenderCloud2, frame_id);
//		usleep(10000);
//
//	}
//	if (c_cloudcopy_enabled)
//	{
//		c_cloudunits.clear();
//		for (neuvition::NeuvUnits::const_iterator iter = data.begin(); iter != data.end(); iter++)
//		{
//			neuvition::NeuvUnit np = (*iter);
//			if (c_pulse_filter_enabled && np.intensity < c_pulse_filter_value)
//				continue;
//			if (np.z == 0)
//				continue;
//			if (np.intensity < mTestMinPw || np.intensity > mTestMaxPw)
//				continue;
//			if (isdistancelimit == true && (np.z > fdistace2 || np.z < fdistace1))
//				continue;
//			if (c_dis_cor_enabled)
//			{
//				uint8_t r = 255, g = 0, b = 0;
//				m_pVTKWidget->config_c_area_color(np, np.r, np.g, np.b);
//			}
//			c_cloudunits.push_back(np);
//		}
//		c_cloudcopy_enabled = false;
//	}
//
//
//	//自动化图像标定
//	CSingletonGetDates* pSingletonObj = CSingletonGetDates::getSingletonObj();
//	if (pSingletonObj->m_bGetPointCloud)
//	{
//		pSingletonObj->m_bGetPointCloud = false;
//		//获取数据
//		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pPointCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
//		pcl::copyPointCloud(cloud_, *pPointCloud);
//
//		pSingletonObj->setPointCloud(pPointCloud);
//
//
//	}
//
//
//	//录制带xsens数据的点云
//	/*static int nTime = 0;
//	if (g_bLoad)
//	{
//		nTime++;
//		logger_ptz_info << "on_frame -- time count = " << nTime;
//	}*/
//
//#define _XSENS_NEUV_PCZ
//#ifdef _XSENS_NEUV_PCZ
//	static int nQueueSize = 0;
//	CGlobalDataGetInstance* pGlobalDataGetIns = CGlobalDataGetInstance::getInstance();
//	if (g_bLoad)
//	{
//		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  newpczpointcloud(new pcl::PointCloud<pcl::PointXYZRGBA>());
//		pcl::copyPointCloud(cloud_, *newpczpointcloud);
//		//g_pointcloud_queue.enqueue(newpczpointcloud);
//
//		neuvition::NeuGPRMC pp = neuvition::get_gps_details();
//		std::cout << "------pp.longitude = " << pp.longitude;
//		std::cout << "------pp.latitude  = " << pp.latitude;
//		CGlobalDataGetInstance* pGlobalDataGetIns = CGlobalDataGetInstance::getInstance();
//		pGlobalDataGetIns->setSaveXsensData();
//		//xsense
//		stXsensData xsensData;
//		while (true && pGlobalDataGetIns->isXsensWorking())
//		{
//			if (pGlobalDataGetIns->isCanGetXsensData())
//			{
//				pGlobalDataGetIns->getXsensData(xsensData);
//				break;
//			}
//			//QThread::usleep(10);
//		}
//
//
//
//		neuwPczData  pczData;
//		pczData.gpsData.dLon = pp.longitude;
//		pczData.gpsData.dLat = pp.latitude;
//		pczData.nineAxisData.euler_x = xsensData.euler_x;
//		pczData.nineAxisData.euler_y = xsensData.euler_y;
//		pczData.nineAxisData.euler_z = xsensData.euler_z;
//		pczData.nineAxisData.accelerate_x = xsensData.accelerate_x;
//		pczData.nineAxisData.accelerate_y = xsensData.accelerate_y;
//		pczData.nineAxisData.accelerate_z = xsensData.accelerate_z;
//		pczData.nineAxisData.angularV_x = xsensData.angularV_x;
//		pczData.nineAxisData.angularV_y = xsensData.angularV_y;
//		pczData.nineAxisData.angularV_z = xsensData.angularV_z;
//		pczData.nineAxisData.velocity_x = xsensData.velocity_x;
//		pczData.nineAxisData.velocity_y = xsensData.velocity_y;
//		pczData.nineAxisData.velocity_z = xsensData.velocity_z;
//		pczData.nineAxisData.magnetic_x = xsensData.magnetic_x;
//		pczData.nineAxisData.magnetic_y = xsensData.magnetic_y;
//		pczData.nineAxisData.magnetic_z = xsensData.magnetic_z;
//
//		pczData.p_point_cloud = newpczpointcloud;
//		g_pointcloud_queue.enqueue(pczData);
//		nQueueSize++;
//		std::cout << "on_frame --nQueueSize = " << nQueueSize << std::endl;
//		logger_ptz_info << "on_frame --nQueueSize = " << nQueueSize;
//	}
//#endif // _XSENS_NEUV_PCZ
//
}

void test_on_mjpgdata(int code, int64_t microsec, cv::Mat mat) 
{
	
	//if (mat.empty()) return;


	////自动化图像标定
	//CSingletonGetDates* pSingletonObj = CSingletonGetDates::getSingletonObj();
	//if (pSingletonObj->m_bGetJpg)
	//{
	//	pSingletonObj->m_bGetJpg = false;
	//	cv::Mat  copyMat;
	//	mat.copyTo(copyMat);
	//	pSingletonObj->setJpgDate(copyMat);
	//}
	////emit g_LadarControl.UpdateCamera_signal(copyMat);
	////static int index = 0;
	////index++;
	////std::cout << "on_jpg_zbs-------" << index << std::endl;
	//mat.copyTo(mJpegMat);
	//m_neuclibration->mRefreshCamImage = true;
	//m_neuclibration->imageWidth = mat.cols;
	//m_neuclibration->imageHight = mat.rows;


}

void test_on_imudata(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu) 
{
//	// int32_t sec;
//		// int32_t usec;
//		// int32_t roll;
//		// int32_t yaw;
//		// int32_t pitch;
//		// int16_t quat_i;
//		// int16_t quat_j;
//		// int16_t quat_k;
//		// int16_t quat_r;
//
//	
//
//	CGlobalDataGetInstance* pGlobalDataGetIns = CGlobalDataGetInstance::getInstance();
//	pGlobalDataGetIns->setImuIsWorking(true);
//	if (pGlobalDataGetIns->isSaveImuData())
//	{
//		std::cout << "pGlobalDataGetIns -- save imu--" << std::endl;
//
//		pGlobalDataGetIns->saveImuData(imu);
//		pGlobalDataGetIns->setCanGetImuData();
//	}
//
//
//	if (g_bImuRead)
//	{
//		g_bImuRead = false;
//		logger_trace << "---imu---:";
//		logger_imu_one_roll << imu.roll / 1000000.0;
//		logger_imu_one_pitch << imu.pitch / 1000000.0;
//		logger_imu_one_yaw << imu.yaw / 1000000.0;
//	}
//
//	//std::cout << "correct angle :  pitch =" << imu.pitch / 1000000.0 
//	//	<< "  roll = " << imu.roll / 1000000.0 
//	//	<<  " yaw = " << imu.yaw / 1000000.0
//	//	<< std::endl;
//
//
//	logger_imu_roll << imu.roll / 1000000.0; //
//	logger_imu_yaw << imu.yaw / 1000000.0;;//imu.yaw / 1000000.0;
//	logger_imu_pitch << imu.pitch / 1000000.0; ;
//
//	g_dSumImu_roll += imu.roll / 1000000.0;
//	g_dSumImu_pitch += imu.pitch / 1000000.0;
//	g_dSumImu_yaw += imu.yaw / 1000000.0;
//	g_nCoundImu++;
//	if (g_nCoundImu % 100 == 0)
//	{
//		/*logger_debug << "avg imu roll = " << g_dSumImu_roll / g_nCoundImu;
//		logger_debug << "avg imu yaw = " << g_dSumImu_yaw / g_nCoundImu;
//		logger_debug << "avg imu pitch = " << g_dSumImu_pitch / g_nCoundImu;*/
//	}
//
//	/*	sensor_msgs::Imu imu_data;
//
//		imu_data.header.stamp = ros::Time::now();
//		imu_data.header.frame_id = "neuvition";
//
//#define QP(n) (1.0f/(1<<n))
//
//		imu_data.orientation.x = QP(14) * imu.quat_i;
//		imu_data.orientation.y = QP(14) * imu.quat_j;
//		imu_data.orientation.z = QP(14) * imu.quat_k;
//		imu_data.orientation.w = QP(14) * imu.quat_r;
//
//		imu_data.linear_acceleration.x = 0.0f;
//		imu_data.linear_acceleration.y = 0.0f;
//		imu_data.linear_acceleration.z = 0.0f;
//
//		imu_data.angular_velocity.x = 0.0f;
//		imu_data.angular_velocity.y = 0.0f;
//		imu_data.angular_velocity.z = 0.0f;
//
//		neudrv->IMU_pub.publish(imu_data);
//*/
//
//
//
///*if (mImuRollStart == 0 && mImuPitchStart == 0 && mImuYawStart == 0)
//{
//	mImuRollStart = imu.roll / 1000000.0 * M_PI / 180.0;
//	;
//	mImuPitchStart = imu.pitch / 1000000.0 * M_PI / 180.0;
//	;
//	mImuYawStart = imu.yaw / 1000000.0 * M_PI / 180.0;
//	;
//}
//else
//{
//	boost::mutex::scoped_lock lockit(imuMutex);
//	mImuRollCur = imu.roll / 1000000.0 * M_PI / 180.0;
//	;
//	mImuPitchCur = imu.pitch / 1000000.0 * M_PI / 180.0;
//	;
//	mImuYawCur = imu.yaw / 1000000.0 * M_PI / 180.0;
//	;
//}*/
}

