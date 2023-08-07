#pragma once
#include <QObject>
#include <QSerialPort>
enum enCmdType
{
	CMD_NONE,
	CMD_STOP,
	CMD_OPEN_CB,
	CMD_DO_DATUM_POS_0,//����Ԥ��λ0
	CMD_REQ_HOR_ANGLE,
	CMD_REQ_VEC_ANGLE,
	CMD_ROTATE_HOR_ANGLE,
	CMD_ROTATE_VEC_ANGLE
};

struct stRecordCmd
{
	enCmdType cmdType = CMD_NONE;
	float     fHorAngle = 0.0;
	float     fVecAngle = 0.0;
};

#define OLDPTZ0
#ifndef OLDPTZ
#define NEWPTZ
#endif

class CPTZCore :public QObject
{
	Q_OBJECT

public:
	CPTZCore();
	~CPTZCore();

public:
	//���ڲ���
	bool scanSerialPort();//ɨ�贮�ڣ���ȡ��һ������
	bool openSerialPort();//�򿪴���
	bool openSerialPort(QString sSerialPortName);
	void closeSerialPort();//�رմ���
	int  getErrorCode(); //��ȡ���ڴ򿪴�����

	bool testSerialAllPort(QString &availPortName); //���Դ��ڣ� ��ȡ���п��ô��ڣ�Ȼ��򿪴��ڣ�����ָ����鿴�Ƿ��з���ֵ��

	//QByteArray getReadAll();//��ȡ���ڶ�ȡ��������
	//cmd

	void _cmd_Stop();
	void _cmd_Req_HorAngle();
	void _cmd_Sync_Req_HorAngle(); //ͬ����ѯ����
	void _cmd_Req_VecAngle();
	void _cmd_Sync_Req_VecAngle();  //ͬ����ѯ����
	void _cmd_HorRotateToAngle(float fAngle = 0.00);
	void _cmd_Sync_HorRotateToAngle(float fAngle = 0.00); //ͬ��ˮƽ�Ƕ���ת
	void _cmd_VecRotateToAngle(float fAngle = 0.00);
	void _cmd_Sync_VecRotateToAngle(float fAngle = 0.00); //ͬ����ֱ�Ƕ���ת
	
	void _cmd_Up(int nSpeed);
	void _cmd_Down(int nSpeed);
	void _cmd_Left(int nSpeed);
	void _cmd_Rigth(int nSpeed);

	void _cmd_Call_Datum_Position(); //������̨Ĭ�ϵ�0��λ�òο��㣺 Ҳ������̨�е�Ԥ��λ0��99�Ĺ��ܡ� 
								 //--- ����������ǣ���ĳЩ���������£���̨��0��λ�ÿ��ܲ�����ʵ��0��λ�á�
								 //--- ͨ���������Ԥ��λ��������������̨Ĭ�ϵ�0��λ����Ϊ�ο�����Ϊ���ǵ�0��λ�á�
	void _cmd_OpenCBAngleInfo(bool bOpen = true);


	//����ʵʱ�ǶȻش�
	void _cmd_On_Angle_Return(int nMicSec = 0);
	//�ر�ʵʱ�ǶȻش�
	void _cmd_Off_Angle_Return();
	
	//����
	float _transFromCharToFloat(char HChar, char LChar, bool bVecAngle); //�ǶȽ��������ֽ�תΪ�Ƕ�
private:

	void _waitForNextCmd(); //�ȴ�����ִ�е���һ������
	
	bool _waitForResult(int nMSec = 3000);//ͬ�����������ڵȴ����ؽ����  ����

	void _protectMechanism(); //ϵͳ����
	void _setReturnCount(int i = 0);
	void _ReturnCountAutoDel();
	void _execute_last_cmd(); //ִ���������������
	void _record_Execution_Cmd(stRecordCmd recordCmd); //��¼ִ�й�������

	void _is_Correct_ReadData_Sync(QByteArray& baReadDate, stRecordCmd& curRecordCmd);
	
public Q_SLOTS:
	void slotReadData();
	void slotReadData_Sync();

Q_SIGNALS:
	void sigReadData(const QByteArray baReadData);

private:
	QSerialPort    m_SerialPort;
	QSerialPort    m_testSerialPort; //�����õĴ��ڡ�
	unsigned char  m_PtzAddr = 0x01;//��̨��ַ�� Ĭ������Ϊ0x01
	QByteArray     m_baReadInfo;
	int            m_nReturnCount = 0; //���ڼ�¼���͵�������Ҫ���ص���Ϣ������ ÿ�������Ȼ����һ��������Ϣ������ >=1

	bool           m_bCmdDoing = false; //�������ڱ�ִ��


	unsigned char  m_pCurCmd[7];   //��ǰִ�е�����ֵ��
	bool           m_bCmdDoSuccess = false; //��ǰִ��������
	stRecordCmd    m_curRecordCmd; //��¼�µ�ǰ��cmd�Ļ�����Ϣ�����ͣ�����ֵ��

};

