#include "lidar_common_defs.h"

	void CLidarRunState::setLidarRunState(enLidarRunState enRunState)
	{
		std::cout << "***** cur Lidar Run State = " << enRunState << "****" << std::endl;
		m_curRunState = enRunState;
		emit sigRunState(m_curRunState);
	}
	CLidarRunState::enLidarRunState CLidarRunState::getLidarRunState()
	{
		return m_curRunState;
	}





	void CLidarConnState::setLidarConnState(enLidarConnState  enConnState)
	{
		std::cout << "***** cur Lidar Conn State = " << enConnState << "****" << std::endl;
		m_enLidarConnState = enConnState;
		emit sigLidarConnState(m_enLidarConnState);
	}

	void CLidarConnState::setCameraConnState(enLidarConnState enCameraConnState)
	{
		std::cout << "***** cur Camera Conn State = " << enCameraConnState << "****" << std::endl;
		m_enCameraConnState = enCameraConnState;
		emit sigCameraConnState(m_enCameraConnState);
	}

	CLidarConnState::enLidarConnState CLidarConnState::getLidarConnState()
	{
		return m_enLidarConnState;
	}

	CLidarConnState::enLidarConnState CLidarConnState::getCameraConnState() 
	{
		return m_enCameraConnState;
	}




