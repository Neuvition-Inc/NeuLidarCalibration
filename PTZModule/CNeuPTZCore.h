#pragma once

#include <QObject>
#include <QSerialPort>
#include "CPTZCore.h"

class CNeuPTZCore : public QObject
{
	Q_OBJECT

public:
	CNeuPTZCore(QObject *parent = nullptr);
	~CNeuPTZCore();

	//启动实时角度回传
	bool cmd_On_Angle_Return(int nMicSec = 0); //todo 因为请求再返回不如一次性请求
	//关闭实时角度回传
	bool cmd_Off_Angle_Return();   //todo
	//启动角度到达后回传
	bool cmd_On_Return_Angle_After_Reach();   //todo
	//关闭角度到达后回传
	bool cmd_Off_Return_Angle_After_Reach();   //todo
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

private:
	bool _write_cmd(unsigned char pCmd[9], enCmdType cmdType);
	bool _parseResultCmd(QByteArray& baCmd);
	float _parseAngleValue(QByteArray baData);
	QByteArray _angle2hex(float fAngle);

private:
	QSerialPort    m_SerialPort;
	QSerialPort    m_testSerialPort; //测试用的串口。
	unsigned char  m_PtzAddr = 0x01;//云台地址： 默认设置为0x01




};
