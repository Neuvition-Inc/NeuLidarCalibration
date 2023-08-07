#include "CNeuvObserver.h"
#include "../CNeuvSubject.h"
#include <iostream>
#include <QDir>
#include <QThread>
#include <QDateTime>

#include "../../CGlobalSignalSlot.h"
//#include "../common.h"
//#include "./CLidarControl.h"

CNeuvObserver::CNeuvObserver(Subject* pSubject)
{
	m_pSubject = pSubject;
	m_pSubject->attach(this);
}

CNeuvObserver::~CNeuvObserver()
{
	if (nullptr != m_pSubject)
	{
		m_pSubject->detach(this);
	}
}

void CNeuvObserver::update(Subject* pSubject)
{
	CNeuvSubject* pNeuvSubject = static_cast<CNeuvSubject*>(pSubject);
	neuvition::NeuvUnits  data;
	pNeuvSubject->getData(data);

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	
	_transNeuvData2Cloud(data, pCloud);


	CGlobalSignalSlot::getInstance()->emit sigUpdateRenderCloud(pCloud);

	if (!m_bSavePcd)
	{
		return;
	}
	m_bSavePcd = false;



	


	int nErrorCode = -1;
	if (pCloud->size() > 0) 
	{
		// nErrorCode = pcl::io::savePCDFileBinaryCompressed(m_sPcdPath, *pCloud);
	}
	
	
	std::cout << " nErrorCode =  " << nErrorCode << " =====" << std::endl;
}

void CNeuvObserver::savePcd2Local(QString sDir, QString sFileName)
{
	QDir dir;
	if (!dir.exists(sDir))
	{
		dir.mkpath(sDir);
	}

	QString sPath = sDir + "/" + sFileName;
	m_sPcdPath = sPath.toLocal8Bit().toStdString();
	m_bSavePcd = true;
}

void CNeuvObserver::openPczWriter(QString sDir, QString sFileName)
{
	QDir dir;
	if (!dir.exists(sDir))
	{
		dir.mkpath(sDir);
	}
	m_sPczDir = sDir.toLocal8Bit().toStdString();
	m_sPczFileName = sFileName.toLocal8Bit().toStdString();

	neuvition::set_pcz_path(sDir.toLocal8Bit().toStdString().c_str());
	neuvition::set_data_save(true);
	std::string sPczName = _getOldPczName();
	m_sOldPczFileName = std::string("neuvition_") + sPczName +".pcz";
	
	m_bOpenPczWriter = true;

}

void CNeuvObserver::closePczWirter()
{
	neuvition::set_data_save(false);
	m_bOpenPczWriter = false;

	//重命名
	QFile file;
	QString sOldFileName = QString::fromStdString(m_sPczDir + "/" + m_sOldPczFileName);
	QString sNewFileName = QString::fromStdString(m_sPczDir + "/" + m_sPczFileName);
	
	bool bRet = file.rename(sOldFileName, sNewFileName);
	int nCount = 5;
	while (nCount-- > 0 && !bRet)
	{
		QThread::msleep(100);
		bRet = file.rename(sOldFileName, sNewFileName);
	}
}

std::string CNeuvObserver::_getOldPczName()
{
	int nCount = 10;
	/*while (!neuvition::is_can_get_pcz_path_id() && nCount--)
	{
		QThread::usleep(1000);
	}
	if (!neuvition::is_can_get_pcz_path_id())
	{
		return QDateTime::currentDateTime().toString("MMdd_hhmmss").toStdString();
	}
	else
	{
		char pczname[128];
		memset(pczname, 0, 128);
		neuvition::get_pcz_path_id(pczname);
		return std::string(pczname);
	}*/
	return std::string();
}

void CNeuvObserver::_transNeuvData2Cloud(neuvition::NeuvUnits& data, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{
	for (neuvition::NeuvUnits::const_iterator iter = data.begin(); iter != data.end(); iter++) {
		const neuvition::NeuvUnit& np = (*iter);
		pcl::PointXYZRGBA point;


		point.x = np.x*0.001;
		point.y = np.y*0.001;
		point.z = np.z*0.001;
		point.r = np.r;
		point.g = np.g;
		point.b = np.b;
		point.a = 255;
		// 如果设置摄像头开启，并开启图像获取，则RGB为真实颜色，否则为128


		enRenderMode renderMode;
		renderMode = CLidarControl::getInstance()->getRenderMode();

		switch (renderMode)
		{
		case enRenderMode::RENDERMODE_ERR:
			break;
		case enRenderMode::MONOCHROM:
			point.r = 255; 
			point.g = 0;
			point.b = 0;
			break;
		case enRenderMode::POLYCHROM:
			getColorByTof(np.tof, point.r, point.g, point.b); //按照距离来生成颜色
			break;
		case enRenderMode::CAMARAMODE:
			point.r = np.r;
			point.g = np.g;
			point.b = np.b;
			break;
		case enRenderMode::REFLECTIVITYMODE:
			//to do
			break;
		default:
			break;
		}


		pCloud->push_back(point);

	}

}
