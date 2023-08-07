#pragma once
#include <QObject>
#include <iostream>

enum class enLidarErrorCode
{
	LidarError_Normal = 0,
	LidarError_INeuvEventIsNull,
	LidarError_Setup_Client_Error,
	LidarError_Start_Scan_Error,
	LidarError_Start_Stream_Error,
	LidarError_Stop_Stream_Error,
	LidarError_Stop_Scan_Error,
	LidarError_Teardown_Client_Error,
	LidarError_Timeout_Error,
	LidarError_System_Error,
	LidarError_Lidar_No_Conn_Error,
	LidarError_Camera_Error,
	LidarError_Power_Value_ERROR,
	LidarError_InterValue_Error,
	LidarError_FrameValue_Error,
	LidarError_FrameLinesValue_Error

};


class CLidarRunState : public QObject
{
	Q_OBJECT
public:
	enum enLidarRunState
	{
		Lidar_Run_Default = -1,
		Lidar_Run_Normal = 0,
		Lidar_Run_Abnormal
	};

	static CLidarRunState* getInstance()
	{
		static CLidarRunState instance;
		return &instance;
	}
	~CLidarRunState() {};



	void setLidarRunState(enLidarRunState enRunState);
	enLidarRunState getLidarRunState();
Q_SIGNALS:
	void sigRunState(enLidarRunState  enRunState);

private:
	CLidarRunState() {};

private:
	int  m_nCurState = -1;
	enLidarRunState  m_curRunState = Lidar_Run_Default;

};


class CLidarConnState : public QObject
{
	Q_OBJECT
public:

	enum enLidarConnState
	{
		Lidar_Default = -1,
		Lidar_DisConnected = 0,
		Lidar_Connecting_Setup,
		Lidar_Connecting_Scan,
		Lidar_Connecting_Stream,
		Lidar_Connected
	};

Q_SIGNALS:
	void sigLidarConnState(enLidarConnState  enConnState);
	void sigCameraConnState(enLidarConnState  enCameraConnState);

public:

	~CLidarConnState() {};

	static CLidarConnState* getInstance()
	{
		static CLidarConnState instance;
		return &instance;
	}

	void setLidarConnState(enLidarConnState  enConnState);

	void setCameraConnState(enLidarConnState enCameraConnState);

	enLidarConnState getLidarConnState();

	enLidarConnState getCameraConnState() ;

private:
	CLidarConnState() {
		qRegisterMetaType<enLidarConnState>("enLidarConnState");

	};
private:
	enLidarConnState  m_enLidarConnState = Lidar_Default;
	enLidarConnState  m_enCameraConnState = Lidar_DisConnected;
};


enum class enRenderMode 
{
	RENDERMODE_ERR = -1,
	MONOCHROM = 1,		
	POLYCHROM,		
	CAMARAMODE,		
	REFLECTIVITYMODE	
};


