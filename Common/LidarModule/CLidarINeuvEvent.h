#pragma once


#include <neuv_defs.hpp>
#include "lidar_common_defs.h"
#include "CNeuvSubject.h"
#include "CMatSubject.h"
#include "CalibrationProcess.h"


typedef void(*fun_on_framedata)(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id);
typedef void(*fun_on_mjpgdata)(int code, int64_t microsec, cv::Mat mat);
typedef void(*fun_on_imudata)(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu);


class CLidarINeuvEvent :  public neuvition::INeuvEvent
{
public:
	CLidarINeuvEvent();
	~CLidarINeuvEvent();

	virtual void on_connect(int code, const char* msg);
	virtual void on_disconnect(int code);
	virtual void on_response(int code, enum neuvition::neuv_cmd_code cmd);
	virtual void on_framedata(int code, int64_t microsec, const neuvition::NeuvUnits& data, const neuvition::nvid_t& frame_id);
	virtual void on_pczdata(bool status);
	virtual void on_mjpgdata(int code, int64_t microsec, cv::Mat mat);
	virtual void on_Ladar_Camera(neuvition::NeuvCameraLadarDatas * data);
	virtual void on_lidar_info_status(neuvition::LidarInfoStatus * lidarInfoStatus);
	virtual void on_imudata(int code, int64_t microsec, const neuvition::NeuvUnits &data, const neuvition::ImuData &imu);
	virtual void on_framestart1(int nCode) {}
	virtual void on_framestart2(int nCode) {}

	CNeuvSubject* getNeuvSubject();
	CMatSubject*  getMatSubject();


	void registerFunOnFrameData(fun_on_framedata  on_framedata_cb);
	void registerFunOnMjpgData(fun_on_mjpgdata  on_mjpgdata_cb);
	void registerFunOnImuData(fun_on_imudata on_imudata_cb);

	//carl
	void getSerialNumber();

private:

	std::vector<fun_on_framedata> m_vec_fun_onframedate;
	std::vector<fun_on_mjpgdata>  m_vec_fun_on_mjpgdata;
	std::vector<fun_on_imudata>   m_vec_fun_on_imudata;

	CNeuvSubject*  m_pNeuvSubject = nullptr;
	CMatSubject*   m_pMatSubject = nullptr;
};
