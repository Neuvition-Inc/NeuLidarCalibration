#pragma once

#include <QObject>
#include <QUdpSocket>

struct stPTZData
{
	int64_t nCurTimestamp = -1;
	float fHorAngle = 0;
	float fVecAngle = 0;
};

typedef void(*PTZRealAngleReturnCB)(float fHorAngle, float fVecAngle); //ʵʱ�ǶȻص�������ֻ��udp����Ч�����Ҵ���̨�ĽǶȻش�����

#define  INVALID_ANGLE  -370.0

class CPTZUdpCore : public QObject
{
	Q_OBJECT

public:
	CPTZUdpCore(QString sIp,int nPort = 6666,QObject *parent = nullptr);
	CPTZUdpCore(QObject *parent = nullptr);
	~CPTZUdpCore();

public:
	//����ʵʱ�ǶȻش�
	bool cmd_On_Angle_Return(int nMicSec = 0);
	//�ر�ʵʱ�ǶȻش�
	bool cmd_Off_Angle_Return();
	//�����Ƕȵ����ش�
	bool cmd_On_Return_Angle_After_Reach(); 
	//�رսǶȵ����ش�
	bool cmd_Off_Return_Angle_After_Reach();

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

	//��ס����ƶ���
	bool cmd_Up(int nSpeed);
	bool cmd_Down(int nSpeed);
	bool cmd_Left(int nSpeed);
	bool cmd_Rigth(int nSpeed);

	//��������
	bool cmd_Call_Datum_Position(); //���ݾ���̨

	//��192.168.8.200
	bool open(QString sIp = "192.168.8.200", int nPort = 6666);
	//�ر�
	void close();

	//ע��ص�����
	void registerRealAngleReturnCB(PTZRealAngleReturnCB ptzRealAngleRetCB);
	//ע���ص�����
	void unregisterRealAngleReturnCB();

private Q_SLOTS:
	void slotReceiveData();

Q_SIGNALS:
	void sigReadData(const QByteArray baReadData);
private:
	bool _send(unsigned char* pCmd, int nLen = 7); 


	//�ǶȻش�����: 
	bool _parseReturnAngle(const QByteArray baData);

	float _transFromCharToFloat(char HChar, char LChar, bool bVecAngle);



private:
	QUdpSocket    m_SendSocket;
	QUdpSocket    m_ReceiveSocket;

	int			  m_nPort;
	QHostAddress  m_hostaddr;
	unsigned char  m_PtzAddr = 0x01;//��̨��ַ�� Ĭ������Ϊ0x01

	bool			m_bOpen = false;

	float          m_fCurHorAngle = INVALID_ANGLE;
	float          m_fCurVecAngle = INVALID_ANGLE;
	PTZRealAngleReturnCB    m_ptzRealAngleRetCB = nullptr;
};
