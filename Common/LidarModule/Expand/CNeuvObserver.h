#pragma once

#include "../Subject.h"

#include <neuv_defs.hpp>
#include "../../pcl_common_defs.h"
#include <QString>
#include <string>

class CNeuvObserver : public Observer
{


public:
	CNeuvObserver(Subject* pSubject);
	~CNeuvObserver();

	void update(Subject* pSubject);

	void savePcd2Local(QString sDir, QString sFileName);

	//pcz±£´æ
	void openPczWriter(QString sDir, QString sFileName);
	void closePczWirter();

protected:
	std::string _getOldPczName();
	void _transNeuvData2Cloud(neuvition::NeuvUnits& data, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud);
private:
	Subject* m_pSubject = nullptr;
	//pcd
	std::string  m_sPcdPath;
	bool    m_bSavePcd = false;
	//pcz
	std::string m_sPczDir;
	std::string m_sPczFileName;
	std::string m_sOldPczFileName;
	bool       m_bOpenPczWriter = false;
};

