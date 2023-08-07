#pragma once

#include "Subject.h"
#include "../pcl_common_defs.h"
#include <neuv_defs.hpp>

class CNeuvSubject : public Subject
{

public:
	CNeuvSubject();
	~CNeuvSubject();

	void setData(const neuvition::NeuvUnits& data);
	void getData(neuvition::NeuvUnits& data);

	void setCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud);
	void getCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud);

private:
	neuvition::NeuvUnits  m_neuvdata;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  m_pCloud;// (new pcl::PointCloud<pcl::PointXYZRGBA>);

};
