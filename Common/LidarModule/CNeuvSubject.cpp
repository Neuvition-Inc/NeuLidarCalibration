#include "CNeuvSubject.h"

CNeuvSubject::CNeuvSubject()
{
	m_pCloud.reset(new pcl::PointCloud<pcl::PointXYZRGBA>);
	//m_pCloud = new pcl::PointCloud<pcl::PointXYZRGBA>;
}

CNeuvSubject::~CNeuvSubject()
{
}

void CNeuvSubject::setData(const neuvition::NeuvUnits& data)
{
	//清空数据
	neuvition::NeuvUnits tempdata;
	tempdata.swap(m_neuvdata);

	//存储数据
	m_neuvdata.assign(data.begin(),data.end());

	this->notify();
}

void CNeuvSubject::getData(neuvition::NeuvUnits& data)
{
	data.assign(m_neuvdata.begin(), m_neuvdata.end());
}

void CNeuvSubject::setCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{
	if (!pCloud || pCloud->size() <= 0)
	{
		return;
	}

	pcl::PointCloud<pcl::PointXYZRGBA> temp;
	m_pCloud->swap(temp);

	pcl::copyPointCloud(*pCloud, *m_pCloud);

	this->notify();
}

void CNeuvSubject::getCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{
	if (m_pCloud->size() <= 0)
	{
		return;
	}

	pcl::copyPointCloud(*m_pCloud, *pCloud);
}
