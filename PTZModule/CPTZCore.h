#pragma once
#include <QObject>
#include <QSerialPort>
enum enCmdType
{
	CMD_NONE,
	CMD_STOP,
	CMD_OPEN_CB,
	CMD_DO_DATUM_POS_0,//调用预置位0
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
	//串口操作
	bool scanSerialPort();//扫描串口：获取第一个串口
	bool openSerialPort();//打开串口
	bool openSerialPort(QString sSerialPortName);
	void closeSerialPort();//关闭串口
	int  getErrorCode(); //获取串口打开错误码

	bool testSerialAllPort(QString &availPortName); //测试串口： 获取所有可用串口，然后打开串口，发送指令，并查看是否有返回值。

	//QByteArray getReadAll();//获取串口读取到的数据
	//cmd

	void _cmd_Stop();
	void _cmd_Req_HorAngle();
	void _cmd_Sync_Req_HorAngle(); //同步查询函数
	void _cmd_Req_VecAngle();
	void _cmd_Sync_Req_VecAngle();  //同步查询函数
	void _cmd_HorRotateToAngle(float fAngle = 0.00);
	void _cmd_Sync_HorRotateToAngle(float fAngle = 0.00); //同步水平角度跳转
	void _cmd_VecRotateToAngle(float fAngle = 0.00);
	void _cmd_Sync_VecRotateToAngle(float fAngle = 0.00); //同步垂直角度跳转
	
	void _cmd_Up(int nSpeed);
	void _cmd_Down(int nSpeed);
	void _cmd_Left(int nSpeed);
	void _cmd_Rigth(int nSpeed);

	void _cmd_Call_Datum_Position(); //调用云台默认的0点位置参考点： 也就是云台中的预置位0或99的功能。 
								 //--- 解决的问题是，在某些特殊外因下，云台的0点位置可能不是真实的0点位置。
								 //--- 通过调用这个预置位，就能重新以云台默认的0点位置作为参考点作为真是的0点位置。
	void _cmd_OpenCBAngleInfo(bool bOpen = true);


	//启动实时角度回传
	void _cmd_On_Angle_Return(int nMicSec = 0);
	//关闭实时角度回传
	void _cmd_Off_Angle_Return();
	
	//解析
	float _transFromCharToFloat(char HChar, char LChar, bool bVecAngle); //角度解析：从字节转为角度
private:

	void _waitForNextCmd(); //等待可以执行到下一条命令
	
	bool _waitForResult(int nMSec = 3000);//同步函数，用于等待返回结果。  毫秒

	void _protectMechanism(); //系统保护
	void _setReturnCount(int i = 0);
	void _ReturnCountAutoDel();
	void _execute_last_cmd(); //执行最近的那条命令
	void _record_Execution_Cmd(stRecordCmd recordCmd); //记录执行过的命令

	void _is_Correct_ReadData_Sync(QByteArray& baReadDate, stRecordCmd& curRecordCmd);
	
public Q_SLOTS:
	void slotReadData();
	void slotReadData_Sync();

Q_SIGNALS:
	void sigReadData(const QByteArray baReadData);

private:
	QSerialPort    m_SerialPort;
	QSerialPort    m_testSerialPort; //测试用的串口。
	unsigned char  m_PtzAddr = 0x01;//云台地址： 默认设置为0x01
	QByteArray     m_baReadInfo;
	int            m_nReturnCount = 0; //用于记录发送的命令需要返回的信息条数： 每个命令必然都有一个返回信息的条数 >=1

	bool           m_bCmdDoing = false; //命令正在被执行


	unsigned char  m_pCurCmd[7];   //当前执行的命令值。
	bool           m_bCmdDoSuccess = false; //当前执行命令结果
	stRecordCmd    m_curRecordCmd; //记录下当前的cmd的基本信息：类型，命令值。

};

