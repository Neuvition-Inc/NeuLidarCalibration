#pragma once

#include <iostream>
#include "lidar_common_defs.h"
#include "CLidarINeuvEvent.h"



class CNeuvLidarControl
{

public:
	static CNeuvLidarControl* getInstance()
	{
		static CNeuvLidarControl instance;
		return &instance;
	}


	~CNeuvLidarControl();

	bool isInitINeuvEvent();
	void initINeuvEvent();
	CLidarINeuvEvent* getINeuvEventObject();


	//enLidarErrorCode startLidar2(std::string sIp, const int nPort = 6668);
	enLidarErrorCode startLidar(std::string sIp, const int nPort = 6668, int nTimeout = 10);
	enLidarErrorCode stopLidar();

	enLidarErrorCode startCamera();
	enLidarErrorCode stopCamera();


	enLidarErrorCode setLidarPower(int nPowerValue);
	int getLidarPower();

	enLidarErrorCode setLidarInterval(int nInterValue);
	int getLidarInterval();
	int transLidarInterval(std::string sLidarInter);
	std::string transLidarInterval(int nLidarInter);
	std::map<int, std::string> getMapLidarInterval();


	enLidarErrorCode setFrameFrequency(int nFps);
	int getFrameFrequency();
	std::vector<int> getVecFps();


	enLidarErrorCode setFrameLines(int nLines);
	int getFrameLines();

	int getDeviceType();
	std::string getDeviceTypeName();

	void setPointSize(int value);
	int getPointSize();

	enRenderMode  transRenderMode(std::string sRenderMode);
	void setRenderMode(enRenderMode enRenMode);
	enRenderMode  getRenderMode();

	void getColorByTof(uint32_t nTof, int& nR, int& nG, int& nB, int nTofColorCycle = 65000, int nColorRangeSize = 3);
	void getColorByTof(uint32_t nTof, uint8_t& nR, uint8_t& nG, uint8_t& nB, int nTofColorCycle = 65000, int nColorRangeSize = 3);


private:
	void _initParams();

	void _updateLidarParams();

	CNeuvLidarControl();

private:
	

	CLidarINeuvEvent* m_pLidarINeuvEvent = nullptr;
	int m_nPointSize = 1;
	enRenderMode  m_enRenderMode = enRenderMode::POLYCHROM;


	std::map<int, std::string>  m_map_lidarRate; //key 表示的就是传给sdk的值，keys:表示所有的可能可以设置的值
	std::vector<int>   m_vec_lidarFps; //key: 表示的就是传给sdk的值, keys:表示所有的fps的值。

};
