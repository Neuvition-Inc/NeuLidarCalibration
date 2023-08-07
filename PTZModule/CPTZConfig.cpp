#include "CPTZConfig.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDir>



CPTZConfig::CPTZConfig(QObject *parent)
	: QObject(parent)
{
	QString sPath = QCoreApplication::applicationDirPath() + "/" + "PTZConf.ini";

	_readConfigIni(sPath);
}

void CPTZConfig::_readConfigIni(QString sFilePath)
{
	QDir dir;
	if (!dir.exists(sFilePath))
	{
		//logger_ptz_error << "the file PTZConf.ini isnot exist! \n";
		_createDefaultIni(sFilePath,m_stPTZConfParams);
		return;
	}
	//判断文件是否存在-- 不存在的情况下就用默认值
	QSettings  setting(sFilePath, QSettings::IniFormat);

	bool bOpenPTZ = setting.value("PTZCoreConf/OpenPTZ").toBool();
	bool bAuthorizeSerialPort = setting.value("PTZCoreConf/AuthorizeSerialPort").toBool();

	m_stPTZConfParams.bOpenPTZ = bOpenPTZ;
	m_stPTZConfParams.bAuthorizeSerialPort = bAuthorizeSerialPort;

	bool bRet = false;
	int num = setting.value("NUM/num").toInt(&bRet);
	if (bRet)
	{
		m_stPTZConfParams.vec_posinfo.clear();
		for (int i = 1; i <= num; i++)
		{
			QString sHorKey = QString("POS%1").arg(i) + "/" + "horAngle";
			QString sVecKey = QString("POS%1").arg(i) + "/" + "vecAngle";
			float fHorAngle = setting.value(sHorKey).toFloat();
			float fVecAngle = setting.value(sVecKey).toFloat();
			m_stPTZConfParams.vec_posinfo.push_back(std::pair<float, float>(fHorAngle, fVecAngle));
		}
	}

}

bool CPTZConfig::_createDefaultIni(QString sFilePath, stPTZConfParams& stPtzConfParams)
{
	QDir dir;
	if (!dir.exists(sFilePath))
	{
		int nIndex = sFilePath.lastIndexOf('/');
		if (nIndex == -1)
		{
			return false;
		}
		QString sDir = sFilePath.left(nIndex);
		
		QDir newDir(sDir);
		if (!newDir.exists())
		{
			newDir.mkpath(sDir);
		}
		
		//创建文本
		QSettings settings(sFilePath, QSettings::IniFormat);
		//写入默认值内容
		settings.beginGroup("PTZCoreConf");
		settings.setValue("OpenPTZ", m_stPTZConfParams.bOpenPTZ);
		settings.setValue("AuthorizeSerialPort", m_stPTZConfParams.bAuthorizeSerialPort);
		settings.endGroup();

		return true;
	}
}

CPTZConfig::~CPTZConfig()
{
}

bool CPTZConfig::isOpenPTZ()
{
	return m_stPTZConfParams.bOpenPTZ;
}

bool CPTZConfig::isAuthorizationSerialPort()
{
	return m_stPTZConfParams.bAuthorizeSerialPort;
}

void CPTZConfig::getPosInfo(std::vector<std::pair<float, float>>& vec_posinfo)
{
	vec_posinfo = m_stPTZConfParams.vec_posinfo;
}
