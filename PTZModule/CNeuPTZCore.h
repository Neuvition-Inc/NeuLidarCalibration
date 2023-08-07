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

	//����ʵʱ�ǶȻش�
	bool cmd_On_Angle_Return(int nMicSec = 0); //todo ��Ϊ�����ٷ��ز���һ��������
	//�ر�ʵʱ�ǶȻش�
	bool cmd_Off_Angle_Return();   //todo
	//�����Ƕȵ����ش�
	bool cmd_On_Return_Angle_After_Reach();   //todo
	//�رսǶȵ����ش�
	bool cmd_Off_Return_Angle_After_Reach();   //todo
	//ˮƽ��ת���Ƕ�
	bool cmd_HorRotateToAngle(float fAngle = 0.00);
	//��ֱ��ת���Ƕ�
	bool cmd_VecRotateToAngle(float fAngle = 0.00);

	//ֹͣ
	bool cmd_stop();
	//���β�ѯˮƽ�Ƕ�
	bool cmd_Req_HorAngle();
	//���β�ѯ��ֱ�Ƕ�
	bool cmd_Req_VecAngle();

private:
	bool _write_cmd(unsigned char pCmd[9], enCmdType cmdType);
	bool _parseResultCmd(QByteArray& baCmd);
	float _parseAngleValue(QByteArray baData);
	QByteArray _angle2hex(float fAngle);

private:
	QSerialPort    m_SerialPort;
	QSerialPort    m_testSerialPort; //�����õĴ��ڡ�
	unsigned char  m_PtzAddr = 0x01;//��̨��ַ�� Ĭ������Ϊ0x01




};
