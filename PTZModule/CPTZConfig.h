#pragma once

#include <QObject>

struct stPTZConfParams
{
	bool bOpenPTZ = false;
	bool bAuthorizeSerialPort = false;

	std::vector<std::pair<float, float>>  vec_posinfo;
};

class CPTZConfig : public QObject
{
	Q_OBJECT

public:
	~CPTZConfig();

	static CPTZConfig* getInstance() 
	{
		static CPTZConfig m_ptzConfig;
		return &m_ptzConfig;
	}

	bool isOpenPTZ();
	bool isAuthorizationSerialPort();

	void getPosInfo(std::vector<std::pair<float, float>>& vec_posinfo);

private:
	CPTZConfig(QObject *parent = nullptr);

	void _readConfigIni(QString sFilePath);
	bool _createDefaultIni(QString sFilePath, stPTZConfParams& stPtzConfParams);
	//void _writeConfigIni();

	stPTZConfParams m_stPTZConfParams;
};
