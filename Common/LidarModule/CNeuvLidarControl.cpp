#include "CNeuvLidarControl.h"
#include <neuv_defs.hpp>

#ifdef _WINDOWS
#include <Windows.h>
#include <time.h>
#endif

void lidar_usleep(unsigned long ulMicroSeconds)
{

#ifdef _WINDOWS
	if (ulMicroSeconds < 1000) {
		Sleep(1);
		return;
	}
	Sleep(ulMicroSeconds / 1000);
#else
	usleep(ulMicroSeconds);
#endif

}

void CNeuvLidarControl::_initParams()
{
	neuvition::set_flip_axis(false, true);

	//neuvition::set_g_filter_enabled(true);
	//neuvition::set_g_linear_fit_enabled(true);
	//neuvition::set_g_gaus_fit_enabled(true);

	//neuvition::set_c_surface_correct_enabled(true);
	//neuvition::set_c_filter_enabled(true);//test

	//neuvition::set_c_wire_detection_enabled(false);
	//neuvition::set_c_container_detection_enabled(false);
}

void CNeuvLidarControl::_updateLidarParams()
{
	std::vector<int>().swap(m_vec_lidarFps);
	std::map<int, std::string>().swap(m_map_lidarRate);

	int nDeviceType = neuvition::get_device_type();
	if (nDeviceType < 0)
	{
		return;
	}

	//lidar重复率:key 表示的就是传给sdk的值
	m_map_lidarRate[0] = "200KHZ";
	m_map_lidarRate[1] = "300KHZ";
	m_map_lidarRate[2] = "400KHZ";
	m_map_lidarRate[3] = "500KHZ";
	m_map_lidarRate[4] = "750KHZ";
	m_map_lidarRate[5] = "1MHZ";

	if (nDeviceType == neuvition::TITAN_M1_SL || nDeviceType == neuvition::TITAN_M1_R_SL)
	{
		m_map_lidarRate[6] = "1.5MHZ";
	}
	if (nDeviceType == neuvition::TITAN_P1_SL)
	{
		//lidar帧率
		m_vec_lidarFps.push_back(1);
		m_vec_lidarFps.push_back(2);
		m_vec_lidarFps.push_back(5);
		m_vec_lidarFps.push_back(10);
		m_vec_lidarFps.push_back(20);
		m_vec_lidarFps.push_back(40);
	}
	else
	{
		//lidar帧率
		m_vec_lidarFps.push_back(1);
		m_vec_lidarFps.push_back(2);
		m_vec_lidarFps.push_back(3);
		m_vec_lidarFps.push_back(5);
		m_vec_lidarFps.push_back(6);
		m_vec_lidarFps.push_back(10);
		m_vec_lidarFps.push_back(15);
		m_vec_lidarFps.push_back(20);
		m_vec_lidarFps.push_back(30);
	}

		// int ret = neuvition::set_laser_interval(mLaserRateIndex);
}

CNeuvLidarControl::CNeuvLidarControl()
{
}

CNeuvLidarControl::~CNeuvLidarControl()
{
}

bool CNeuvLidarControl::isInitINeuvEvent()
{
	return (m_pLidarINeuvEvent == nullptr ? false : true);
}

void CNeuvLidarControl::initINeuvEvent()
{
	if (nullptr == m_pLidarINeuvEvent)
	{
		m_pLidarINeuvEvent = new CLidarINeuvEvent;
	}
}

CLidarINeuvEvent* CNeuvLidarControl::getINeuvEventObject()
{
	return m_pLidarINeuvEvent;
}


enLidarErrorCode CNeuvLidarControl::startLidar(std::string sIp, const int nPort /*= 6668*/, int nTimeout /*= 10*/)
{

	int nError = -1;
	std::cout << "*********StartLadar  Begin:*********" << std::endl;

	if (nullptr == m_pLidarINeuvEvent)
	{
		CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
		return enLidarErrorCode::LidarError_INeuvEventIsNull;
	}

	_initParams();

	nError = neuvition::setup_client(sIp.c_str(), nPort,
		(neuvition::INeuvEvent*)m_pLidarINeuvEvent, false/*auto-reconnect*/);
	
	CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connecting_Setup);


	//开始等待状态的改变-阻塞
	long long beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	
	lidar_usleep(1000 * 1000);
	bool bHas_start_scan = false;
	bool bHas_start_stream = false;
	
	while (true)
	{
		CLidarConnState::enLidarConnState enCurConnState = CLidarConnState::getInstance()->getLidarConnState();
		switch (enCurConnState)
		{
		case CLidarConnState::Lidar_Connecting_Setup:
			{
				if (bHas_start_scan)
				{
					break;
				}
				bHas_start_scan = true;
				lidar_usleep(1000 * 1000 * 1);
				nError = neuvition::start_scan();
				if (nError != 0)
				{
					std::cout << "StartLadar  neuvition::start_scan() ret != 0" << std::endl;
					CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
					return enLidarErrorCode::LidarError_Start_Scan_Error;
				}
			}
			break;
		case CLidarConnState::Lidar_Connecting_Scan:
			{
				lidar_usleep(1000 * 1000 * 1);
				if (bHas_start_stream)
				{
					break;
				}
				bHas_start_stream = true;
				nError = neuvition::start_stream();
				//lidar_usleep(1000 * 1000 * 5);
				if (nError != 0)
				{
					std::cout << "StartLadar  neuvition::start_stream() ret != 0" << std::endl;
					CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
					return enLidarErrorCode::LidarError_Start_Stream_Error;
				}
				neuvition::set_c_tof_cor_by_pw(false);
			}
			break;
		case CLidarConnState::Lidar_Connecting_Stream:
			{
				CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connected);
			}
			break;
		case CLidarConnState::Lidar_Connected:

			_updateLidarParams();

			return enLidarErrorCode::LidarError_Normal;

		case CLidarConnState::Lidar_Default:
		case CLidarConnState::Lidar_DisConnected:
			{
				CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
			}
			return enLidarErrorCode::LidarError_System_Error;

		default:
			break;
		}
		

		int nDurTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - beginTime;
		if (nDurTime >= nTimeout*1000)
		{
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
			return enLidarErrorCode::LidarError_Timeout_Error;
		}
		lidar_usleep(200);
	}
}

enLidarErrorCode CNeuvLidarControl::stopLidar()
{
	// 请求底层设备停止送点云流
	int nError = neuvition::stop_stream();
	if (nError != 0)
	{
		CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Default);
		return enLidarErrorCode::LidarError_Stop_Stream_Error;
	}
	
	//开始等待状态的改变-阻塞
	long long beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	int nTimeout = 3;
	while (true)
	{
		CLidarConnState::enLidarConnState enCurLidarConnState = CLidarConnState::getInstance()->getLidarConnState();
		switch (enCurLidarConnState)
		{
		
		case CLidarConnState::Lidar_Connecting_Setup:
			{
				// 断开与底层设备的连接
				nError = neuvition::teardown_client();
				CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
				if (nError != 0)
				{
					CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Default);
					return enLidarErrorCode::LidarError_Teardown_Client_Error;
				}
			}
			break;
		case CLidarConnState::Lidar_Connecting_Scan:
			{
				// 请求底层设备停止扫描
				nError = neuvition::stop_scan();
				if (nError != 0)
				{
					CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Default);
					return enLidarErrorCode::LidarError_Stop_Scan_Error;
				}
			}
			break;
		case CLidarConnState::Lidar_DisConnected:
			return enLidarErrorCode::LidarError_Normal;
		default:
			break;
		}

		int nDurTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - beginTime;
		if (nDurTime >= nTimeout * 1000)
		{
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Default);
			return enLidarErrorCode::LidarError_Timeout_Error;
		}
		lidar_usleep(200);
	}

	
	
	
}

enLidarErrorCode CNeuvLidarControl::startCamera()
{
	int ret = neuvition::set_camera_status(true);

	if (ret == -1)
	{
		return enLidarErrorCode::LidarError_System_Error;
	}
	else if (ret == -2)
	{
		return	enLidarErrorCode::LidarError_Lidar_No_Conn_Error;
	}
	ret = neuvition::set_mjpg_curl(true);
	if (ret != 0)
	{
		return enLidarErrorCode::LidarError_Camera_Error;
	}


	CLidarConnState::getInstance()->setCameraConnState(CLidarConnState::Lidar_Connected);
	std::cout << "***** open camera sucess!!" << std::endl;
	return enLidarErrorCode::LidarError_Normal;
}

enLidarErrorCode CNeuvLidarControl::stopCamera()
{
	neuvition::set_mjpg_curl(false);
	neuvition::set_camera_status(false);

	CLidarConnState::getInstance()->setCameraConnState(CLidarConnState::Lidar_DisConnected);
	std::cout << "***** close camera sucess!!" << std::endl;

	return enLidarErrorCode::LidarError_Normal;
}

enLidarErrorCode CNeuvLidarControl::setLidarPower(int nPowerValue)
{
	int ret = neuvition::set_laser_power(nPowerValue);

	lidar_usleep(20 * 000);//20ms

	if (ret == -1)
	{
		return enLidarErrorCode::LidarError_System_Error;
	}
	else if (ret == -2)
	{
		return enLidarErrorCode::LidarError_Lidar_No_Conn_Error;
	}
	else if (ret == -3)
	{
		return enLidarErrorCode::LidarError_Power_Value_ERROR;
	}
	return enLidarErrorCode::LidarError_Normal;
}

int CNeuvLidarControl::getLidarPower()
{
	return  neuvition::get_laser_power();
}

enLidarErrorCode CNeuvLidarControl::setLidarInterval(int nInterValue)
{
	int ret = neuvition::set_laser_interval(nInterValue);
	lidar_usleep(20000);//20ms
	if (ret == -1)
	{
		return enLidarErrorCode::LidarError_System_Error;
	}
	else if (ret == -2)
	{
		return enLidarErrorCode::LidarError_Lidar_No_Conn_Error;
	}
	else if (ret == -3)
	{
		return enLidarErrorCode::LidarError_InterValue_Error;
	}
	return  enLidarErrorCode::LidarError_Normal;
}

int CNeuvLidarControl::getLidarInterval()
{
	return neuvition::get_laser_interval();
}


int CNeuvLidarControl::transLidarInterval(std::string sLidarInter)
{
	for (int i = 0; i < m_map_lidarRate.size(); i++)
	{
		if (m_map_lidarRate[i]==sLidarInter)
		{
			return i;
		}
	}
	return -1;
}

std::string CNeuvLidarControl::transLidarInterval(int nLidarInter)
{
	return m_map_lidarRate[nLidarInter];
}

std::map<int, std::string> CNeuvLidarControl::getMapLidarInterval()
{
	return m_map_lidarRate;
}

enLidarErrorCode CNeuvLidarControl::setFrameFrequency(int nFps)
{
	int ret = neuvition::set_frame_frequency(nFps);
	lidar_usleep(20000);//20ms
	if (ret == -1)
	{
		return enLidarErrorCode::LidarError_System_Error;
	}
	else if (ret == -2)
	{
		return enLidarErrorCode::LidarError_Lidar_No_Conn_Error;
	}
	else if (ret == -3)
	{
		return enLidarErrorCode::LidarError_FrameValue_Error;
	}
	return  enLidarErrorCode::LidarError_Normal;
}


int CNeuvLidarControl::getFrameFrequency()
{
	return  neuvition::get_frame_frequency();
}

std::vector<int> CNeuvLidarControl::getVecFps()
{
	return m_vec_lidarFps;
}

enLidarErrorCode CNeuvLidarControl::setFrameLines(int nLines)
{
	int ret = neuvition::set_frame_line_quantity(nLines);
	lidar_usleep(2000);//20ms
	if (ret == -1)
	{
		return enLidarErrorCode::LidarError_System_Error;
	}
	else if (ret == -2)
	{
		return enLidarErrorCode::LidarError_Lidar_No_Conn_Error;
	}
	else if (ret == -3)
	{
		return enLidarErrorCode::LidarError_FrameLinesValue_Error;
	}
	return  enLidarErrorCode::LidarError_Normal;
	
}

int CNeuvLidarControl::getFrameLines()
{
	return neuvition::get_frame_line_quantity();
}

int CNeuvLidarControl::getDeviceType()
{
	int device_type = neuvition::get_device_type();
	return device_type;
}

std::string CNeuvLidarControl::getDeviceTypeName()
{
	std::string deviceName = "";
	neuvition::neuv_device_type enDeviceType = static_cast<neuvition::neuv_device_type>(neuvition::get_device_type());
	switch (enDeviceType)
	{
	case neuvition::TITAN_M1:
		deviceName.append("TITAN_M1");
		break;
	case neuvition::TITAN_M1_PRO:
		deviceName.append("TITAN_M1_PRO");
		break;
	case neuvition::TITAN_M1_PLUS:
		deviceName.append("TITAN_M1_PLUS");
		break;
	case neuvition::TITAN_M1_R:
		deviceName.append("TITAN_M1_R");
		break;
	case neuvition::TITAN_S1:
		deviceName.append("TITAN_S1");
		break;
	case neuvition::TITAN_M1_A:
		deviceName.append("TITAN_M1_A");
		break;
	case neuvition::TITAN_M1_SL:
		deviceName.append("TITAN_M1_SL");
		break;
	case neuvition::TITAN_M2:
		deviceName.append("TITAN_M2");
		break;
	case neuvition::TITAN_M1_M:
		deviceName.append("TITAN_M1_M");
		break;
	case neuvition::TITAN_M1_PRO_SL:
		deviceName.append("TITAN_M1_PRO_SL");
		break;
	case neuvition::TITAN_M1_R_SL:
		deviceName.append("TITAN_M1_R_SL");
		break;
	case neuvition::TITAN_M1_120:
		deviceName.append("TITAN_M1_120");
		break;
	case neuvition::TITAN_S1_PLUS:
		deviceName.append("TITAN_S1_PLUS");
		break;
	case neuvition::TITAN_S1_PLUS_SL:
		deviceName.append("TITAN_S1_PLUS_SL");
		break;
	case neuvition::TITAN_P1:
		deviceName.append("TITAN_P1");
		break;
	case neuvition::TITAN_S2:
		deviceName.append("TITAN_S2");
		break;
	case neuvition::TITAN_M1_A_SL:
		deviceName.append("TITAN_M1_A_SL");
		break;
	case neuvition::TITAN_P1_SL:
		deviceName.append("TITAN_P1_SL");
		break;
	case neuvition::TITAN_S2_70:
		deviceName.append("TITAN_S2_70");
		break;
	case neuvition::TITAN_S2_120:
		deviceName.append("TITAN_S2_120");
		break;
	default:
		deviceName.append("Initial");
		break;
	}
	
	return deviceName;
}

void CNeuvLidarControl::setPointSize(int value)
{
	m_nPointSize = value;
}

int CNeuvLidarControl::getPointSize()
{
	return m_nPointSize;
}

enRenderMode CNeuvLidarControl::transRenderMode(std::string sRenderMode)
{
	if (sRenderMode == "CameraMode")
	{
		return enRenderMode::CAMARAMODE;
	}
	else if (sRenderMode == "Monochrome")
	{
		return enRenderMode::MONOCHROM;
	}
	else if (sRenderMode == "Polychrome")
	{
		return enRenderMode::POLYCHROM;
	}
	else if (sRenderMode == "ReflectivityMode")
	{
		return enRenderMode::REFLECTIVITYMODE;
	}
	else
	{
		return enRenderMode::RENDERMODE_ERR;
	}
}

void CNeuvLidarControl::setRenderMode(enRenderMode enRenMode)
{
	m_enRenderMode = enRenMode;
}

enRenderMode CNeuvLidarControl::getRenderMode()
{
	return m_enRenderMode;
}

void CNeuvLidarControl::getColorByTof(uint32_t nTof, int& nR, int& nG, int& nB, int nTofColorCycle /*= 65000*/, int nColorRangeSize /*= 3*/)
{
	//green, yellow, red
	const uint32_t coloredge[3] = { 0x00ff00, 0xffff00, 0xff0000 };

	int colorrange = nTofColorCycle / nColorRangeSize;
	int tofcolor = nTof % nTofColorCycle;
	int tofclass = tofcolor / (colorrange);
	int tofreman = tofcolor % (colorrange);
	if (tofclass >= nColorRangeSize) tofclass = nColorRangeSize - 1;
	uint32_t cbase = coloredge[(tofclass + 0) % nColorRangeSize];
	uint32_t cnext = coloredge[(tofclass + 1) % nColorRangeSize];
	int rbase = (cbase & 0xff0000) >> 16;
	int gbase = (cbase & 0x00ff00) >> 8;
	int bbase = (cbase & 0x0000ff) >> 0;
	int rnext = (cnext & 0xff0000) >> 16;
	int gnext = (cnext & 0x00ff00) >> 8;
	int bnext = (cnext & 0x0000ff) >> 0;
	int rdelay = (rnext - rbase) * tofreman / colorrange;
	int gdelay = (gnext - gbase) * tofreman / colorrange;
	int bdelay = (bnext - bbase) * tofreman / colorrange;
	nR = (rbase + rdelay) & 0xFF;
	nG = (gbase + gdelay) & 0xFF;
	nB = (bbase + bdelay) & 0xFF;
}

void CNeuvLidarControl::getColorByTof(uint32_t nTof, uint8_t& nR, uint8_t& nG, uint8_t& nB, int nTofColorCycle /*= 65000*/, int nColorRangeSize /*= 3*/)
{
	//green, yellow, red
	const uint32_t coloredge[3] = { 0x00ff00, 0xffff00, 0xff0000 };

	int colorrange = nTofColorCycle / nColorRangeSize;
	int tofcolor = nTof % nTofColorCycle;
	int tofclass = tofcolor / (colorrange);
	int tofreman = tofcolor % (colorrange);
	if (tofclass >= nColorRangeSize) tofclass = nColorRangeSize - 1;
	uint32_t cbase = coloredge[(tofclass + 0) % nColorRangeSize];
	uint32_t cnext = coloredge[(tofclass + 1) % nColorRangeSize];
	int rbase = (cbase & 0xff0000) >> 16;
	int gbase = (cbase & 0x00ff00) >> 8;
	int bbase = (cbase & 0x0000ff) >> 0;
	int rnext = (cnext & 0xff0000) >> 16;
	int gnext = (cnext & 0x00ff00) >> 8;
	int bnext = (cnext & 0x0000ff) >> 0;
	int rdelay = (rnext - rbase) * tofreman / colorrange;
	int gdelay = (gnext - gbase) * tofreman / colorrange;
	int bdelay = (bnext - bbase) * tofreman / colorrange;
	nR = (rbase + rdelay) & 0xFF;
	nG = (gbase + gdelay) & 0xFF;
	nB = (bbase + bdelay) & 0xFF;
}

