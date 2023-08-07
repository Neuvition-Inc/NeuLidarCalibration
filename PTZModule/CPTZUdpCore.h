#pragma once

#include <QObject>
#include <QUdpSocket>

struct stPTZData
{
	int64_t nCurTimestamp = -1;
	float fHorAngle = 0;
	float fVecAngle = 0;
};

typedef void(*PTZRealAngleReturnCB)(float fHorAngle, float fVecAngle); //实时角度回调函数，只有udp下有效，并且打开云台的角度回传功能

#define  INVALID_ANGLE  -370.0

class CPTZUdpCore : public QObject
{
	Q_OBJECT

public:
	CPTZUdpCore(QString sIp,int nPort = 6666,QObject *parent = nullptr);
	CPTZUdpCore(QObject *parent = nullptr);
	~CPTZUdpCore();

public:
	//启动实时角度回传
	bool cmd_On_Angle_Return(int nMicSec = 0);
	//关闭实时角度回传
	bool cmd_Off_Angle_Return();
	//启动角度到达后回传
	bool cmd_On_Return_Angle_After_Reach(); 
	//关闭角度到达后回传
	bool cmd_Off_Return_Angle_After_Reach();

	//水平旋转到角度
	bool cmd_HorRotateToAngle(float fAngle = 0.00);
	//垂直旋转到角度
	bool cmd_VecRotateToAngle(float fAngle = 0.00);

	//停止
	bool cmd_stop();
	//单次查询水平角度
	bool cmd_Req_HorAngle();
	//单次查询垂直角度
	bool cmd_Req_VecAngle();

	//按住鼠标移动的
	bool cmd_Up(int nSpeed);
	bool cmd_Down(int nSpeed);
	bool cmd_Left(int nSpeed);
	bool cmd_Rigth(int nSpeed);

	//其他功能
	bool cmd_Call_Datum_Position(); //兼容旧云台

	//打开192.168.8.200
	bool open(QString sIp = "192.168.8.200", int nPort = 6666);
	//关闭
	void close();

	//注册回调函数
	void registerRealAngleReturnCB(PTZRealAngleReturnCB ptzRealAngleRetCB);
	//注销回调函数
	void unregisterRealAngleReturnCB();

private Q_SLOTS:
	void slotReceiveData();

Q_SIGNALS:
	void sigReadData(const QByteArray baReadData);
private:
	bool _send(unsigned char* pCmd, int nLen = 7); 


	//角度回传数据: 
	bool _parseReturnAngle(const QByteArray baData);

	float _transFromCharToFloat(char HChar, char LChar, bool bVecAngle);



private:
	QUdpSocket    m_SendSocket;
	QUdpSocket    m_ReceiveSocket;

	int			  m_nPort;
	QHostAddress  m_hostaddr;
	unsigned char  m_PtzAddr = 0x01;//云台地址： 默认设置为0x01

	bool			m_bOpen = false;

	float          m_fCurHorAngle = INVALID_ANGLE;
	float          m_fCurVecAngle = INVALID_ANGLE;
	PTZRealAngleReturnCB    m_ptzRealAngleRetCB = nullptr;
};
