#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>


struct st_PCL_CAMERA_POSITION
{
	double pos[3] = { 0 };
	double view[3] = { 0 };
	double up[3] = { 0 };

	void print()
	{
		std::cout << "pos = ( " << pos[0] << ", " << pos[1] << ", " << pos[2] << ")" << std::endl;
		std::cout << "view = ( " << view[0] << ", " << view[1] << ", " << view[2] << ")" << std::endl;
		std::cout << "up = ( " << up[0] << ", " << up[1] << ", " << up[2] << ")" << std::endl;
	}

	void operator= (const st_PCL_CAMERA_POSITION params)
	{
		pos[0] = params.pos[0];
		pos[1] = params.pos[1];
		pos[2] = params.pos[2];

		view[0] = params.view[0];
		view[1] = params.view[1];
		view[2] = params.view[2];

		up[0] = params.up[0];
		up[1] = params.up[1];
		up[2] = params.up[2];
	}

};



class CViewController
{
public:
	CViewController();
	~CViewController();

	//设置三视图的基本参数
	void initViewInfo(double dCenterX,double dCenterY,double dCenterZ, double dDisRadius);

	st_PCL_CAMERA_POSITION getTopView();
	st_PCL_CAMERA_POSITION getLeftView();
	st_PCL_CAMERA_POSITION getFrontView();


	//返回一个以"圆心坐标（x, y, z）  r 平面的法向量 normal(x,y,z) 为轨迹的，看向圆心，up轴;
	//参数1 : normal  摄像头绕的轴 ,只支持 平行于x， y, z 的轴:
	//参数2 : center
	//参数3 : radius
	//参数4 : up
	//参数5 : unitAngle : 每次移动的单位角度
	//参数6 : 
	void  getRotateAxisViews(
		Eigen::Vector3f normal,
		Eigen::Vector3f center,
		float  fRadius,
		Eigen::Vector3f up,
		float  fUnitAngle,
		std::vector<st_PCL_CAMERA_POSITION>& vec_cam_positions
	);


private:
	double m_dCenterX = 0.0;
	double m_dCenterY = 0.0;
	double m_dCenterZ = 0.0;
	double m_dDisRadius = 0.0;
};

/*
	CViewController controller;
	controller.initViewInfo(center[0], center[1], center[2], fRadius);
	st_PCL_CAMERA_POSITION  campos = controller.getFrontView();

	emit sigCameraPosition(campos);

*/