#include "globalLidarCallbackFun.h"
#include <iostream>
#include "../CGlobalSignalSlot.h"
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include "../opencv_common_defs.h"
#include <neuv_defs.hpp>

#include "./Common/LidarModule/CNeuvLidarControl.h"
 
bool showValidCube = false;
float coreX = 0;
float coreY = 0;
float coreZ = 0;
bool select_pixel_enabled = false;
bool onlyShowSelectPicel = true;
int lid = 0;
int minRow = 0;
int maxRow = 720;
int minCol = 0;
int maxCol = 4096;
bool data_collect_enabled = false;
std::vector<neuvition::NeuvUnit> selectPixelVec;

void g_cb_on_framedata(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id)
{
    //std::cout << "********************* g_cb_on_framedata *****************************" << std::endl;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);

	enRenderMode renderMode;
	renderMode = CNeuvLidarControl::getInstance()->getRenderMode();
	for (neuvition::NeuvUnits::const_iterator iter = data.begin(); iter != data.end(); iter++)
	{
		const neuvition::NeuvUnit& np = (*iter);
		pcl::PointXYZRGBA point;



		point.x = np.x*0.001;
		point.y = np.y*0.001;
		point.z = np.z*0.001;
		point.r = np.r;
		point.g = np.g;
		point.b = np.b;
		point.a = 255;
		if (onlyShowSelectPicel)
		{
			if (np.row < minRow - 50 || np.row > maxRow + 50 || np.col < minCol - 50 || np.col > maxCol + 50)
			{
				continue;
			}
			if (lid != 0 && np.lid != lid)
			{
				continue;
			}
		}





		if (showValidCube)
		{
			if (coreX != 0 || coreY != 0 || coreZ != 0)
			{
				if (point.x < coreX - 0.5 || point.x > coreX + 0.5 || point.y < coreY - 0.5 || point.y > coreY + 0.5 || point.z < coreZ - 1 || point.z > coreZ + 1)
				{
					continue;
				}
			}

		}
		// 如果设置摄像头开启，并开启图像获取，则RGB为真实颜色，否则为128

		switch (renderMode)
		{
		case enRenderMode::RENDERMODE_ERR:
			break;
		case enRenderMode::MONOCHROM:
			point.r = 255;
			point.g = 0;
			point.b = 0;
			break;
		case enRenderMode::POLYCHROM:
			CNeuvLidarControl::getInstance()->getColorByTof(np.tof, point.r, point.g, point.b); //按照距离来生成颜色
			break;
		case enRenderMode::CAMARAMODE:
			point.r = np.r;
			point.g = np.g;
			point.b = np.b;
			break;
		case enRenderMode::REFLECTIVITYMODE:
			//to do
			break;
		default:
			CNeuvLidarControl::getInstance()->getColorByTof(np.tof, point.r, point.g, point.b); //按照距离来生成颜色
			break;
		}
		if (select_pixel_enabled)
		{
			if (np.col >= minCol && np.col <= maxCol && np.row >= minRow && np.row <= maxRow)
			{

				if (lid != 0 && np.lid == lid)
				{
					point.r = 255;
					point.g = 255;
					point.b = 255;
				}
				if (lid == 0)
				{
					point.r = 255;
					point.g = 255;
					point.b = 255;
				}
			}
		}

		pCloud->push_back(point);

	}
	if (data_collect_enabled)
	{
		selectPixelVec.clear();
		for (neuvition::NeuvUnits::const_iterator iter = data.begin(); iter != data.end(); iter++)
		{
			neuvition::NeuvUnit np = (*iter);
			if (np.row >= minRow && np.row <= maxRow && np.col >= minCol && np.col <= maxCol )
			{
				if (lid != 0)
				{
					if (np.lid != lid)
					{
						continue;
					}
				}
				selectPixelVec.push_back(np);
			}
			
		}
		data_collect_enabled = false;
	}

	CGlobalSignalSlot::getInstance()->emit sigUpdateRenderCloud(pCloud);
	//保存单线数据和云台数据
}

void g_cb_on_mjpgdata(int code, int64_t microsec, cv::Mat mat)
{
	std::cout << "********************* g_cb_on_mjpgdata *****************************" << std::endl;
}

void g_cb_on_imudata(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu)
{
	std::cout << "********************* g_cb_on_imudata *****************************" << std::endl;
}

