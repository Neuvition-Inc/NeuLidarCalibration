#include "CPTZCore.h"

#include <QCoreApplication>
#include <QTime>
#include <QSerialPortInfo>
#include <QDebug>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <QDateTime>
#include <iostream>
#define LOG_DEBUG qDebug()    << qPrintable(QString("[%1 %2 %3 %4]:").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)) << qPrintable(QString("M_V: "))


//#include "../LogModule/NeuLogger.h"



CPTZCore::CPTZCore()
{
	QObject::connect(&m_SerialPort, &QSerialPort::readyRead, this, &CPTZCore::slotReadData);
}

CPTZCore::~CPTZCore()
{
	m_SerialPort.close();
}

bool CPTZCore::scanSerialPort()
{
	foreach(const QSerialPortInfo portInfo, QSerialPortInfo::availablePorts()) {
		//设置串口名
		QString portName = portInfo.portName();
		m_SerialPort.setPortName(portName);
		return true;
	}
	return false;
}

bool CPTZCore::openSerialPort()
{
	int nBaudRate = 9600;
	//设置波特率
	m_SerialPort.setBaudRate(nBaudRate);
	//设置数据位数
	m_SerialPort.setDataBits(QSerialPort::Data8);
	//设置奇偶校验
	m_SerialPort.setParity(QSerialPort::NoParity);
	//设置停止位
	m_SerialPort.setStopBits(QSerialPort::OneStop);
	//设置流控制
	m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
	//打开串口
	return m_SerialPort.open(QIODevice::ReadWrite);
}



bool CPTZCore::openSerialPort(QString sSerialPortName)
{
	m_SerialPort.setPortName(sSerialPortName);

	int nBaudRate = 9600;
	//设置波特率
	m_SerialPort.setBaudRate(nBaudRate);
	//设置数据位数
	m_SerialPort.setDataBits(QSerialPort::Data8);
	//设置奇偶校验
	m_SerialPort.setParity(QSerialPort::NoParity);
	//设置停止位
	m_SerialPort.setStopBits(QSerialPort::OneStop);
	//设置流控制
	m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
	//打开串口
	return m_SerialPort.open(QIODevice::ReadWrite);
}

void CPTZCore::closeSerialPort()
{
	m_SerialPort.clear();
	m_SerialPort.close();
}

int CPTZCore::getErrorCode()
{
	return (int)m_SerialPort.error();
}

bool CPTZCore::testSerialAllPort(QString &availPortName)
{
	QStringList availableSerialPortNames;
	foreach(const QSerialPortInfo portInfo, QSerialPortInfo::availablePorts()) {
		//设置串口名
		QString portName = portInfo.portName();
		LOG_DEBUG << "portName = " << portName <<  endl;
		m_testSerialPort.setPortName(portName);

		//打开串口
		int nBaudRate = 9600;
		//设置波特率
		m_testSerialPort.setBaudRate(nBaudRate);
		//设置数据位数
		m_testSerialPort.setDataBits(QSerialPort::Data8);
		//设置奇偶校验
		m_testSerialPort.setParity(QSerialPort::NoParity);
		//设置停止位
		m_testSerialPort.setStopBits(QSerialPort::OneStop);
		//设置流控制
		m_testSerialPort.setFlowControl(QSerialPort::NoFlowControl);
		//打开串口
		bool bRet =  m_testSerialPort.open(QIODevice::ReadWrite);
		int errorcode = m_testSerialPort.error();
		if (!bRet)
		{
			continue;
		}

		//发送信息，并接收信息
		unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		pCmd[1] = m_PtzAddr;
		pCmd[3] = 0x51;
		pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

		m_testSerialPort.write((char*)pCmd, sizeof(pCmd));

		m_testSerialPort.waitForBytesWritten(1000);
		m_testSerialPort.waitForReadyRead(1000);
		QByteArray ba = m_testSerialPort.readAll();
		LOG_DEBUG << "test Serial Port all: " << ba.toHex(':') << endl;
		if (!ba.isEmpty())
		{
			LOG_DEBUG << "test Serial Port sucess: " << ba.toHex(':') << endl;
			availPortName = portName;
			m_testSerialPort.clear();
			m_testSerialPort.close();
			return true;
		}
		m_testSerialPort.clear();
		m_testSerialPort.close();
	}
	return false;
}


void CPTZCore::_cmd_Stop()
{
#ifdef OLDPTZ
	_waitForNextCmd();

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[6] = 0x01;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	//记录命令信息
	stRecordCmd cmd;
	cmd.cmdType = CMD_STOP;
	_record_Execution_Cmd(cmd);

	_setReturnCount(0);
#else
	_waitForNextCmd();

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	//pCmd[6] = 0x01;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff; 

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	//记录命令信息
	stRecordCmd cmd;
	cmd.cmdType = CMD_STOP;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);
#endif
}

void CPTZCore::_cmd_Req_HorAngle()
{
#ifdef OLDPTZ
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x51;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_HOR_ANGLE;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);

#else
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x51;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_HOR_ANGLE;
	_record_Execution_Cmd(cmd);
	_setReturnCount(1);
#endif
}

void CPTZCore::_cmd_Sync_Req_HorAngle()
{
#ifdef OLDPTZ
#else
	//_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_VEC_ANGLE;
	_record_Execution_Cmd(cmd);

	//记录当前的命令的类型
	m_curRecordCmd.cmdType = CMD_REQ_VEC_ANGLE;

	//_setReturnCount(1);

	if (!_waitForResult(3000))
	{
		//重新执行改命令
		LOG_DEBUG << "Warning : Re-execute the command :_cmd_Req_HorAngle" << endl;
		_cmd_Req_HorAngle();
	}

#endif
}

void CPTZCore::_cmd_Req_VecAngle()
{
#ifdef OLDPTZ
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_VEC_ANGLE;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);
#else
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_VEC_ANGLE;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);


#endif

}

void CPTZCore::_cmd_Sync_Req_VecAngle()
{
#ifdef OLDPTZ
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_VEC_ANGLE;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);
#else
	//_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_REQ_VEC_ANGLE;
	_record_Execution_Cmd(cmd);

	//_setReturnCount(1);
	if (!_waitForResult(3000))
	{
		//重新执行改命令
		LOG_DEBUG << "Warning : Re-execute the command :_cmd_Req_VecAngle" << endl;
		_cmd_Req_VecAngle();
	}

#endif
}

void CPTZCore::_cmd_HorRotateToAngle(float fAngle /*= 0.00*/)
{
#ifdef OLDPTZ
	_waitForNextCmd();

	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4B;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_HOR_ANGLE;
	cmd.fHorAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);

#else
	_waitForNextCmd();
	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4B;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	int nRet = m_SerialPort.write((char*)pCmd, sizeof(pCmd));
	if (nRet <= 0)
	{
		std::cout << "Error:" << nRet << std::endl;
	}

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_HOR_ANGLE;
	cmd.fHorAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(2);

	
#endif
}

void CPTZCore::_cmd_Sync_HorRotateToAngle(float fAngle /*= 0.00*/)
{
#ifdef OLDPTZ
	_waitForNextCmd();

	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4B;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_HOR_ANGLE;
	cmd.fHorAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);

#else
	//_waitForNextCmd();
	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4B;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));


	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_HOR_ANGLE;
	cmd.fHorAngle = fAngle;
	_record_Execution_Cmd(cmd);

	m_curRecordCmd = cmd;

	//_setReturnCount(2);
	if (!_waitForResult(30*1000))
	{
		_cmd_Sync_HorRotateToAngle(fAngle);
	}


#endif
}

void CPTZCore::_cmd_VecRotateToAngle(float fAngle /*= 0.00*/)
{
#ifdef OLDPTZ
	_waitForNextCmd();

	//因为为了和pcl的坐标系进行统一，需要强行把云台俯仰角的正负性修改。 即原本仰角为负 改为 仰角为正。
	fAngle = -fAngle;

	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4D;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_VEC_ANGLE;
	cmd.fVecAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);
#else
	_waitForNextCmd();

	if (fAngle > 180)
	{
		fAngle = fAngle - 360.0;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4D;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_VEC_ANGLE;
	cmd.fVecAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(2);

#endif
}

void CPTZCore::_cmd_Sync_VecRotateToAngle(float fAngle /*= 0.00*/)
{
#ifdef OLDPTZ
	_waitForNextCmd();

	//因为为了和pcl的坐标系进行统一，需要强行把云台俯仰角的正负性修改。 即原本仰角为负 改为 仰角为正。
	fAngle = -fAngle;

	if (fAngle < 0)
	{
		fAngle = 360 + fAngle;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4D;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_VEC_ANGLE;
	cmd.fVecAngle = fAngle;
	_record_Execution_Cmd(cmd);

	_setReturnCount(1);
#else
	//_waitForNextCmd();

	if (fAngle > 180)
	{
		fAngle = fAngle - 360.0;
	}
	int nAngle = fAngle * 100;
	nAngle = nAngle % 36000; //保证在0-36000之间

	int nH = nAngle & 0x0000ff00;
	int nL = nAngle & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x4D;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

	stRecordCmd cmd;
	cmd.cmdType = CMD_ROTATE_VEC_ANGLE;
	cmd.fVecAngle = fAngle;
	_record_Execution_Cmd(cmd);

	m_curRecordCmd = cmd;

	if (!_waitForResult(30*1000))
	{
		_cmd_Sync_VecRotateToAngle(fAngle);
	}
	//_setReturnCount(2);

#endif
}

void CPTZCore::_cmd_Up(int nSpeed)
{
#ifdef OLDPTZ

#endif 

	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x08; //向上
	pCmd[5] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

}

void CPTZCore::_cmd_Down(int nSpeed)
{
#ifdef OLDPTZ

#endif 

	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x10; //向下
	pCmd[5] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);

}

void CPTZCore::_cmd_Left(int nSpeed)
{
#ifdef OLDPTZ

#endif 
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x04;
	pCmd[4] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);
}

void CPTZCore::_cmd_Rigth(int nSpeed)
{
#ifdef OLDPTZ

#endif 
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x02;
	pCmd[4] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);
}

void CPTZCore::_cmd_Call_Datum_Position()
{
#ifdef OLDPTZ
	//_waitForNextCmd();

	////普通预置位点调用
	//unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//pCmd[1] = mPtzAddr;
	//pCmd[3] = 0x07;
	//pCmd[5] = 0x00;//0x63 -- 0号预置位 或 99号预置位
	//pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	//mSerialPort.write((char*)pCmd, sizeof(pCmd));
	//
	//stRecordCmd cmd;
	//cmd.cmdType = CMD_DO_DATUM_POS_0;
	//_record_Execution_Cmd(cmd);
	//
	//_waitForCmd(3000);
	//_setReturnCount(0);

#else

#endif
}

void CPTZCore::_cmd_OpenCBAngleInfo(bool bOpen /*= true*/)
{
#ifdef OLDPTZ
	
#else
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	if (bOpen)
	{
		pCmd[1] = m_PtzAddr;
		pCmd[2] = 0xC5;
		pCmd[3] = 0x01;
		pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;
	}
	else
	{
		pCmd[1] = m_PtzAddr;
		pCmd[2] = 0xC5;
		pCmd[3] = 0x00;
		pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;
	}

	//stRecordCmd cmd;
	//cmd.cmdType = CMD_OPEN_CB;
	//_record_Execution_Cmd(cmd);

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);
	//m_SerialPort.waitForBytesWritten(3000);//等待系统把串口数据发送完成

	_setReturnCount(1);
#endif
}

void CPTZCore::_cmd_On_Angle_Return(int nMicSec /*= 0*/)
{
#ifdef OLDPTZ
#else
	_waitForNextCmd();

	int nH = nMicSec & 0x0000ff00;
	int nL = nMicSec & 0x000000ff;
	nH = nH >> 8;
	unsigned char cH = nH;
	unsigned char cL = nL;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0xE1;
	pCmd[3] = 0x01;
	pCmd[4] = cH;
	pCmd[5] = cL;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	/*stRecordCmd cmd;
	cmd.cmdType = CMD_OPEN_CB;
	_record_Execution_Cmd(cmd);*/

	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);
	//m_SerialPort.waitForBytesWritten(3000);//等待系统把串口数据发送完成

	_setReturnCount(1);
#endif
}

void CPTZCore::_cmd_Off_Angle_Return()
{
#ifdef OLDPTZ
#else
	_waitForNextCmd();
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0xE1;
	pCmd[3] = 0x02;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;


	m_SerialPort.write((char*)pCmd, sizeof(pCmd));

	memset(m_pCurCmd, 0, sizeof(m_pCurCmd));
	memcpy(m_pCurCmd, pCmd, 7);
	//m_SerialPort.waitForBytesWritten(3000);//等待系统把串口数据发送完成

	_setReturnCount(1);
#endif
}

float CPTZCore::_transFromCharToFloat(char HChar, char LChar, bool bVecAngle)
{
#ifdef OLDPTZ
	int a = 0, b = 0;
	float fAngle = 0.0;
	if (bVecAngle)
	{
		if (a < 0)
			a += 256;
		if (b < 0)
			b += 256;
		fAngle = (a * 256 + b) / 100.0;
	}
	else 
	{
		if (a < 0)
			a += 256;
		if (b < 0)
			b += 256;
		fAngle = (a * 256 + b) / 100.0;
	}
	//qDebug("M_V:_transFromCharToFloat : fAngle = %f , hChar : %X , LChar : %X \n", fAngle, HChar, LChar);
	return fAngle;
#else
	int a = 0, b = 0, c = 0;
	float fAngle = 0.0;
	a = HChar;
	b = LChar;
	if (bVecAngle)
	{
		a = a << 8;
		b = b & 0x000000ff;
		c = a | b;
		fAngle = c / 100.0;
	}
	else
	{
		if (a < 0)
			a += 256;
		if (b < 0)
			b += 256;

		fAngle = (a * 256 + b) / 100.0;
	}
	//qDebug("M_V:_transFromCharToFloat : fAngle = %f , hChar : %X , LChar : %X \n", fAngle, HChar, LChar);
	return fAngle;


#endif
}

void CPTZCore::_waitForNextCmd()
{
	int delayms = 500;
	QTime time;
	QTime timeTotal; //超时保护
	timeTotal.start();
	int totalDelayms = 1 * 30 * 1000; //30s的超时保护
	while (1)
	{
		time.start();
		while (time.elapsed() < delayms)
		{
			//boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(100));
			QCoreApplication::processEvents();
		}
		if (m_nReturnCount == 0)
		{
			break;
		}

		if (timeTotal.elapsed() > totalDelayms)
		{
			//清空到初始状态，并且执行最近的那条命令：?或者直接执行当前卡住的命令？
			//_execute_last_cmd();
			LOG_DEBUG << "_waitForNextCmd : overtime : totalDelyams = "<< timeTotal.elapsed() << endl;
			m_nReturnCount = 0;
			break;
		}
	}
}


bool CPTZCore::_waitForResult(int nMSec /*= 3000*/)
{
	QTime time;
	time.start();
	
	while (time.elapsed() < nMSec)
	{
		//boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(100));
		
		QCoreApplication::processEvents();

		if (m_bCmdDoSuccess)
		{
			return true;
		}
	
	}
	return false;
	
}

void CPTZCore::_protectMechanism()
{

}

void CPTZCore::_setReturnCount(int i /*= 0*/)
{
	m_nReturnCount = i;
	LOG_DEBUG << "_setReturnCount : m_nReturnCount = " << m_nReturnCount << endl;
	m_bCmdDoing = true;
}

void CPTZCore::_ReturnCountAutoDel()
{
	m_nReturnCount--;
	LOG_DEBUG << "_ReturnCountAutoDel  m_nReturnCount =  " << m_nReturnCount << endl;
	if (m_nReturnCount == 0)
	{
		m_bCmdDoing = false;
	}
	
}

void CPTZCore::_execute_last_cmd()
{
	//还未实现
	qDebug() << "_execute_last_cmd : " << endl;
}

void CPTZCore::_record_Execution_Cmd(stRecordCmd recordCmd)
{
	enCmdType cmdType = recordCmd.cmdType;

	switch (cmdType)
	{
	case CMD_NONE:
		LOG_DEBUG << "CMD_NONE : " << endl;
		break;
	case CMD_STOP:
		LOG_DEBUG << "CMD_STOP : " << endl;
		break;
	case CMD_OPEN_CB:
		LOG_DEBUG << "CMD_OPEN_CB : " << endl;
		break;
	case CMD_DO_DATUM_POS_0:
		LOG_DEBUG << "CMD_DO_DATUM_POS_0 : " << endl;
		break;
	case CMD_REQ_HOR_ANGLE:
		LOG_DEBUG << "CMD_REQ_HOR_ANGLE : "  << endl;
		break;
	case CMD_REQ_VEC_ANGLE:
		LOG_DEBUG << "CMD_REQ_VEC_ANGLE : " << endl;
		break;
	case CMD_ROTATE_HOR_ANGLE:
		LOG_DEBUG << "CMD_ROTATE_HOR_ANGLE :  fHorAngle  = " << recordCmd.fHorAngle << endl;
		break;
	case CMD_ROTATE_VEC_ANGLE:
		LOG_DEBUG << "CMD_ROTATE_VEC_ANGLE :  fVecAngle = " << recordCmd.fVecAngle << endl;
		break;
	default:
		break;
	}

}

void CPTZCore::_is_Correct_ReadData_Sync(QByteArray& baReadDate, stRecordCmd& curRecordCmd)
{
	enCmdType cmdType = curRecordCmd.cmdType;
	switch (cmdType)
	{
	case CMD_NONE:
		break;
	case CMD_STOP:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(2) == 0x00
			&& (unsigned char)baReadDate.at(6) == m_PtzAddr) //
		{
			m_bCmdDoSuccess = true;
		}
		else
		{
			m_bCmdDoSuccess = false;
		}
		break;
	case CMD_OPEN_CB:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(2) == 0xc5
			&& (unsigned char)baReadDate.at(3) == 0x01) //
		{
			m_bCmdDoSuccess = true;
		}
		else
		{
			m_bCmdDoSuccess = false;
		}
		break;
	case CMD_DO_DATUM_POS_0:
		break;
	case CMD_REQ_HOR_ANGLE:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(3) == 0x59)
		{
			m_bCmdDoSuccess = true;
		}
		else
		{
			m_bCmdDoSuccess = false;
		}
		break;
	case CMD_REQ_VEC_ANGLE:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(3) == 0x5b)
		{
			m_bCmdDoSuccess = true;
		}
		else
		{
			m_bCmdDoSuccess = false;
		}
		break;
	case CMD_ROTATE_HOR_ANGLE:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(2) == 0xc5
			&& (unsigned char)baReadDate.at(3) == 0x02) //
		{
			m_bCmdDoSuccess = true;
		}
		else 
		{
			m_bCmdDoSuccess = false;
		}
		break;
	case CMD_ROTATE_VEC_ANGLE:
		if ((unsigned char)baReadDate.at(0) == 0xff && (unsigned char)baReadDate.at(1) == m_PtzAddr
			&& (unsigned char)baReadDate.at(2) == 0xc5
			&& (unsigned char)baReadDate.at(3) == 0x03) //
		{
			m_bCmdDoSuccess = true;
		}
		else
		{
			m_bCmdDoSuccess = false;
		}
		break;
	default:
		m_bCmdDoSuccess = false;
		break;
	}
}

void CPTZCore::slotReadData()
{
	QByteArray buffer = m_SerialPort.readAll();
	static int i = 0;
	i++;
	//logger_ptz_info << " i = " << i << "; slotReadData = " << buffer.toHex(':').toStdString();

	int nSize = buffer.size();
	m_baReadInfo.append(buffer);
	bool bContinue = true;
	while (m_baReadInfo.size() >= 7 && bContinue)
	{
		QByteArray tempInfo;
		bool bTag = false;
		for (int i = 0; i < m_baReadInfo.size() ; i++)
		{
			if ((unsigned char)m_baReadInfo.at(i) == 0xff)
			{
				if (i + 6 <= m_baReadInfo.size() - 1)
				{
					if ((unsigned char)m_baReadInfo.at(i+1) == 0x01)
					{
						tempInfo = m_baReadInfo.mid(i, 7);
						int nRest = m_baReadInfo.size() - (i + 7);
						if (nRest >=0)
						{
							m_baReadInfo = m_baReadInfo.right(nRest);
							bTag = true;
						}

						
					}
				}
				else
				{
					
					bContinue = false;
					break;
				}
				
			}
			
			
		}
		if (bTag)
		{
			_ReturnCountAutoDel();
			//logger_ptz_info << "CPTZCore::slotReadData : ReadInfo :" << tempInfo.toHex(':').toStdString();
			emit sigReadData(tempInfo);
			//parseReadInfo(m_baReadInfo);
			m_baReadInfo.clear();
			
		}

	}
}

void CPTZCore::slotReadData_Sync()
{
	m_bCmdDoSuccess;

	QByteArray buffer = m_SerialPort.readAll();
	int nSize = buffer.size();
	m_baReadInfo.append(buffer);
	if (m_baReadInfo.size() >= 7)
	{
		//判断是否命令执行成功： 成功的话标志位改为true.
		//1. 查询角度的，返回值是返回当前角度值
		QByteArray tempInfo = m_baReadInfo;
		_is_Correct_ReadData_Sync(tempInfo, m_curRecordCmd);
		//_ReturnCountAutoDel();
		LOG_DEBUG << "CPTZCore::slotReadData : ReadInfo :" << tempInfo.toHex(':');

		emit sigReadData(tempInfo);
		//parseReadInfo(m_baReadInfo);
		m_baReadInfo.clear();
		//cmdStop();
	}
}
