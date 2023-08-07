#pragma once

#include <QWidget>
#include "ui_CVTKWidget.h"
#include <string>
#include <neuv_defs.hpp>
//PCL 头文件
#include <pcl/visualization/pcl_visualizer.h>

#include "../pcl_common_defs.h"
//BOOST 头文件
//#include "../CCube.h"
#include "./CPCLPainter.h"
//#include "CPCLViewerRegisterCB.h"

#include "../CViewController.h"

//struct st_PCL_CAMERA_POSITION
//{
//	double pos[3] = { 0 };
//	double view[3] = { 0 };
//	double up[3] = { 0 };
//
//	void print()
//	{
//		std::cout << "pos = ( " << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << std::endl;
//		std::cout << "view = ( " << view[0] << ", " << view[1] << ", " << view[2] << ")" << std::endl;
//		std::cout << "up = ( " << up[0] << ", " << up[1] << ", " << up[2] << ")" << std::endl;
//	}
//
//	void operator= (const st_PCL_CAMERA_POSITION params)
//	{
//		pos[0] = params.pos[0];
//		pos[1] = params.pos[1];
//		pos[2] = params.pos[2];
//
//		view[0] = params.view[0];
//		view[1] = params.view[1];
//		view[2] = params.view[2];
//
//		up[0] = params.up[0];
//		up[1] = params.up[1];
//		up[2] = params.up[2];
//	}
//};

class CVTKWidget : public QWidget
{
	Q_OBJECT

public:
	CVTKWidget(QWidget *parent = Q_NULLPTR);
	~CVTKWidget();

	QVTKWidget *getVTKWidgetPtr()
	{
		return ui.qvtkWidget;
	}

	pcl::visualization::PCLVisualizer::Ptr getPCLViewerPtr()
	{
		return m_pPCLViewer;
	}
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  getRenderPointCloud()
	{
		return m_pCloud;
	}

	void copyLocalPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud);


	//手动更新渲染的点云
	void updateRenderPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud);

	//添加点云（而不是替换）
	bool addRenderPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud, QString sCloudId, int nPointSize = 1);
	bool addRenderPointCloudWithColor(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud, QString sCloudId, int nR = 255, int nG = 0, int nB = 0, int nPointSize = 1);



	//绘制从原点指向某个点的带箭头
	bool addArrow(Eigen::Vector3f point, int nR = 255, int nG = 0, int nB = 0, QString sId = QString("arrow"), Eigen::Vector3f oripoint = Eigen::Vector3f(0, 0, 0));
	bool addArrow(Eigen::VectorXf normal_coeff, int nR = 255, int nG = 0, int nB = 0, QString sId = QString("arrow"), Eigen::Vector3f oripoint = Eigen::Vector3f(0, 0, 0));


	//点云颜色修改
	//	void config_c_area_color(const neuvition::NeuvUnit &np, uint8_t& r, uint8_t& g, uint8_t& b);

	//渲染单帧的pcd
	bool renderLocalPcd(QString sFileName);
	bool renderLocalPcd(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud);

	//添加坐标系
	void addCoor();
	void removeCoor();

	void setRenderPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud)
	{
		m_pCloud = pCloud;
		_initRenderPointCloud();
	}

	//设置视角
	void update_pcl_cam_direction(int nIndex);//jerry 待完善
	void set_pcl_cam_direction(const st_PCL_CAMERA_POSITION stPCLCamParam);

	//鼠标点击回调设置
	//void openMouseSelPoint();  //开启鼠标选点功能
	//void closeMouseSelPoint(); //关闭鼠标选点功能

	void setaddtextline();

	//设置点云的大小
	void setPointSize(int nValue)
	{
		//boost::mutex::scoped_lock lockit(cloudMutex); //锁
		m_nPointSizeValue = nValue <= 0 ? 1 : nValue;
		m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, m_nPointSizeValue, "neuvgram");
	}

	//绘制矩阵
	void addCube(const Eigen::Vector3f &translation, const Eigen::Quaternionf &rotation, double width, double height, double depth, const std::string &id = "cube", int viewport = 0);



public Q_SLOTS:
	void slotNeedRender();
	//void slotSelFinished();

	void slotShowText(QString sText, QString sId, int nXPos, int nYPos);
	void slotAddArrow(pcl::PointXYZ a, pcl::PointXYZ b, int nR, int nG, int nB, QString sId);

	void slotClearAllClouds();
	void slotClearCloudById(QString sId);


	void slotUpdateRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud);
	void slotAddRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId, int nR, int nG, int nB);
	void slotAddRenderCloudWithSelfColor(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId, int nRenderSize);
	void slotDrawCircleCloud(Eigen::VectorXf circle_coeff);
	void slotDrawCircleNoTextWithId(Eigen::VectorXf circle_coeff, QString sId);
	void slotAddCloudWithText(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud, QString sText, QString sId, int nRenderSize, int nFontSize);
	//void slotDrawCube(CCube cube, QString sCubeId);
	//void slotDrawCubeWithColor(CCube cube, QString sCubeId, int nR, int nG, int nB);
	void slotCameraPosition(st_PCL_CAMERA_POSITION stCameraPos);

private:
	void _initPCLViewer();
	void _configQVTKWgt();
	void _initRenderPointCloud();
	//void _initViewerRegisterCB();

protected:
	pcl::visualization::PCLVisualizer::Ptr m_pPCLViewer;
private:
	Ui::CVTKWidget ui;
	int m_nPointSizeValue = 1; //点云绘制的大小
	//boost::shared_ptr<pcl::visualization::PCLVisualizer> m_pPCLViewer;

	pcl::visualization::Camera mPCLCam;


	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr     m_pCloud;

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr     m_pLocalCloud;

	//boost::signals2::connection m_conn_areapicking ;  //记录鼠标框选的回调连接
	//sCPCLViewerRegisterCB *m_pPCLViewerRegCB = nullptr;

};

