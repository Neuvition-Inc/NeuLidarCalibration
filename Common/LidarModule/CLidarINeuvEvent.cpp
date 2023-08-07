#include "CLidarINeuvEvent.h"
#include <iostream>
#include "lidar_common_defs.h"

extern CalibrationProcess *pCalibrationProcess;
bool getFrameDataEnabled = false;
neuvition::NeuvUnits frameDataPoints;
extern bool select_pixel_enabled;

CLidarINeuvEvent::CLidarINeuvEvent()
{
	m_pMatSubject = new CMatSubject;
	m_pNeuvSubject = new CNeuvSubject;
}

CLidarINeuvEvent::~CLidarINeuvEvent()
{
}

void CLidarINeuvEvent::on_connect(int code, const char* msg)
{
	std::cout << "[NEUVITION]| Connect... code=" << code << ", msg=" << msg << std::endl;
	if (code == 0) {
		//CLidarStatus::getInstance()->setLidarState(en_LidarState::LIDAR_STATE_CONNECT);
		//CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::);
		//emit sigDoCmd(1);
		std::cout << "QThread on_connect end : " << std::endl;
	}
	else {
		//CLidarStatus::getInstance()->setLidarState(en_LidarState::LIDAR_STATE_DISCONNECT);
		//CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Not_Connected);
	}
}

void CLidarINeuvEvent::on_disconnect(int code)
{
	if (code == 0) 
	{ 
		std::cout << "[NEUVITION]| Disconnect..." << std::endl; 
		//CLidarStatus::getInstance()->setLidarState(en_LidarState::LIDAR_STATE_DISCONNECT);
		CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_DisConnected);
	}
	else 
	{
		//CLidarStatus::getInstance()->setLidarState(en_LidarState::LIDAR_STATE_UNKNOWN);
		CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Default);
	}
	//mConnectState = CONNECT_STATE_DISCONNECTED;
}

void CLidarINeuvEvent::on_response(int code, enum neuvition::neuv_cmd_code cmd)
{
	switch (cmd) 
	{
	case neuvition::NEUV_CMD_START_SCAN: {
		if (code == 0) { 
			std::cout << "[NEUVITION]| Start scanning..." << std::endl;
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connecting_Scan);
		}

		break;
	}

	case neuvition::NEUV_CMD_STOP_SCAN: {
		if (code == 0) { 
			std::cout << "[NEUVITION]| Stop scanning..." << std::endl;
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connecting_Setup);
		}
		break;
	}

	case neuvition::NEUV_CMD_START_STREAM: {
		if (code == 0) { 
			std::cout << "[NEUVITION]| Start data streaming..." << std::endl; 
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connecting_Stream);
		}
		break;
	}

	case neuvition::NEUV_CMD_STOP_STREAM: {
		if (code == 0) { 
			std::cout << "[NEUVITION]| Stop data streaming..." << std::endl;
			CLidarConnState::getInstance()->setLidarConnState(CLidarConnState::Lidar_Connecting_Scan);
		}
		break;
	}

										  // 上传底层设备配置参数
	case neuvition::NEUV_CMD_GET_PARAMS: {
		if (code == 0) { 
			std::cout << "[NEUVITION]| Device parameters synced..." << std::endl; 
		
		}
		break;
	}

	}

	//emit sigResponseInfo(code, cmd);

}

void CLidarINeuvEvent::on_framedata(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id)
{
	//std::cout << "[NEUVITION]| On framedata... | Size " << data.size() << std::endl;
	if (m_pNeuvSubject)
	{
		m_pNeuvSubject->setData(data);
	}
	
	//return;

	int nSize = m_vec_fun_onframedate.size();
	if (nSize <= 0)
	{
		std::cout << "todo --" << std::endl;
		return;
	}
	for (int i = 0; i < nSize; i++)
	{
		fun_on_framedata  on_framedata_cb = nullptr;
		on_framedata_cb = m_vec_fun_onframedate[i];
		if (on_framedata_cb)
		{
			on_framedata_cb(code, microsec, data, frame_id);
		}
		
	}

	neuvition::jason_camearinfo_clear(frame_id);

	if (getFrameDataEnabled) {
		frameDataPoints.clear();
		frameDataPoints.assign(data.begin(), data.end());
		getFrameDataEnabled = false;
		std::cout << "getFrameDataEnabled = false; "<<std::endl;
	}

}

void CLidarINeuvEvent::on_pczdata(bool status)
{

}

void CLidarINeuvEvent::on_mjpgdata(int code, int64_t microsec, cv::Mat mat)
{
	std::cout << "[NEUVITION]| On 111111111111111111111 on_mjpgdata... | time | " << microsec << std::endl; // microsec为图像时间戳
	if (m_pMatSubject)
	{
		m_pMatSubject->setData(mat);
	}
	
	
	int nSize = m_vec_fun_on_mjpgdata.size();
	if (nSize <= 0)
	{
		std::cout << "todo --" << std::endl;
		return;
	}
	for (int i = 0; i < nSize; i++)
	{
		fun_on_mjpgdata  on_mjpgdata_cb = nullptr;
		on_mjpgdata_cb = m_vec_fun_on_mjpgdata[i];
		if (on_mjpgdata_cb)
		{
			on_mjpgdata_cb(code, microsec, mat);
		}
		
	}

}

void CLidarINeuvEvent::on_Ladar_Camera(neuvition::NeuvCameraLadarDatas * data)
{

}

void CLidarINeuvEvent::on_lidar_info_status(neuvition::LidarInfoStatus * lidarInfoStatus)
{
	printf("============on_lidar_info_status================\n");
	printf("serial_number = %s\n", lidarInfoStatus->serial_number);
	printf("============on_lidar_info_status================\n");
	pCalibrationProcess->SetSerialNumber(lidarInfoStatus->serial_number);
}

void CLidarINeuvEvent::on_imudata(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu)
{
	std::cout << "--------begin-------------------\n" <<
		"imu.pitch = " << imu.pitch <<
		"imu.roll =  " << imu.roll <<
		"imu.yaw =   " << imu.yaw <<
		"imu.quat_i = " << imu.quat_i <<
		"imu.quat_j = " << imu.quat_j <<
		"imu.quat_k  = " << imu.quat_k <<
		"imu.quat_r = " << imu.quat_r <<
		"imu.sec = " << imu.sec <<
		"imu.usec = " << imu.usec <<
		"imu.x = " << imu.gro_x <<
		"imu.y = " << imu.gro_y <<
		"imu.z = " << imu.gro_z
		<< "\n-------------end--------------------"
		<< std::endl;

	//保存到全局变量中
	//CLidarDatas::getInstance()->setImuData(imu);

	//执行回调
	int nSize = m_vec_fun_on_imudata.size();
	if (nSize <= 0)
	{
		std::cout << "todo --" << std::endl;
		return;
	}
	for (int i = 0; i < nSize; i++)
	{
		fun_on_imudata  on_imudata_cb = nullptr;
		on_imudata_cb = m_vec_fun_on_imudata[i];
		if (on_imudata_cb)
		{
			on_imudata_cb(code, microsec, data, imu);
		}
	}
};


CNeuvSubject* CLidarINeuvEvent::getNeuvSubject()
{
	return m_pNeuvSubject;
}

CMatSubject* CLidarINeuvEvent::getMatSubject()
{
	return m_pMatSubject;
}

void CLidarINeuvEvent::registerFunOnFrameData(fun_on_framedata on_framedata_cb)
{
	m_vec_fun_onframedate.push_back(on_framedata_cb);
}

void CLidarINeuvEvent::registerFunOnMjpgData(fun_on_mjpgdata on_mjpgdata_cb)
{
	m_vec_fun_on_mjpgdata.push_back(on_mjpgdata_cb);
}

void CLidarINeuvEvent::registerFunOnImuData(fun_on_imudata on_imudata_cb)
{
	m_vec_fun_on_imudata.push_back(on_imudata_cb);
}

