#include "CNeuPTZCore.h"
#include <iostream>

CNeuPTZCore::CNeuPTZCore(QObject *parent)
	: QObject(parent)
{
}

CNeuPTZCore::~CNeuPTZCore()
{
}

bool CNeuPTZCore::cmd_HorRotateToAngle(float fAngle /*= 0.00*/)
{
	unsigned char pCmd[9] = { 0xaa,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x55 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0x05;

	QByteArray ba = _angle2hex(fAngle);
	pCmd[4] = (unsigned char)ba[0];
	pCmd[5] = (unsigned char)ba[1];
	pCmd[6] = (unsigned char)ba[2];
	pCmd[7] = (unsigned char)ba[3];

	return _write_cmd(pCmd, CMD_ROTATE_HOR_ANGLE);
}

//起始符	地址	命令	数据长度	数据区	结束符
//  0xaa	1byte	1byte	0x04	4bytes	0x55
bool CNeuPTZCore::cmd_stop()
{
	unsigned char pCmd[9] = { 0xaa,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x55 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0x01;
	
	return _write_cmd(pCmd, CMD_STOP);
}

bool CNeuPTZCore::cmd_Req_HorAngle()
{
	unsigned char pCmd[9] = { 0xaa,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x55 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0x07;

	bool bRet = _write_cmd(pCmd, enCmdType::CMD_REQ_HOR_ANGLE);
	if (bRet)
	{
		bool bRead = m_SerialPort.waitForReadyRead(3000);
		if (bRead)
		{
			QByteArray baCmd = m_SerialPort.readAll();
			return _parseResultCmd(baCmd);
		}
	}

	return false;
}

bool CNeuPTZCore::_write_cmd(unsigned char pCmd[9], enCmdType cmdType)
{
	int nLen = m_SerialPort.write((char*)pCmd, sizeof(pCmd));
	bool bRet = m_SerialPort.waitForBytesWritten(3000);
	if (bRet)
	{
		QString sError;
		switch (cmdType)
		{
		case CMD_NONE:
			break;
		case CMD_STOP:
			sError = "write cmd stop: time out!";
			break;
		case CMD_OPEN_CB:
			break;
		case CMD_DO_DATUM_POS_0:
			break;
		case CMD_REQ_HOR_ANGLE:
			break;
		case CMD_REQ_VEC_ANGLE:
			break;
		case CMD_ROTATE_HOR_ANGLE:
			break;
		case CMD_ROTATE_VEC_ANGLE:
			break;
		default:
			break;
		}
		std::cout << sError.toLocal8Bit().toStdString() << std::endl;
		return false;
	}
	return true;
}

bool CNeuPTZCore::_parseResultCmd(QByteArray& baCmd)
{
	int nSize = baCmd.size();
	if (nSize != 9)
	{
		std::cout << "_parseResultCmd failed: size =  "<< baCmd.size() << "  context = " << baCmd.toHex(':').toStdString() << std::endl;
		baCmd.clear();
		return false;
	}

	if ((unsigned char)baCmd[2] == 0x87) //云台（从机）当前角度读取回复&云台主动上传角度信息：
	{
		float fHorAngle = _parseAngleValue(baCmd.mid(4, 4));
	}
}

float CNeuPTZCore::_parseAngleValue(QByteArray baData)
{
	int n1 = (unsigned char)baData[0];
	int n2 = ((unsigned char)baData[1]) < 8;
	int n3 = ((unsigned char)baData[2]) < 16;
	int n4 = ((unsigned char)baData[3]) < 24;
	float f = (n1 + n2 + n3 + n4) / 1000.0;
	return f;
}

QByteArray CNeuPTZCore::_angle2hex(float fAngle)
{
	int nAngle = fAngle * 1000;

	int n1 = nAngle & 0x000000ff;
	int n2 = nAngle & 0x0000ff00;
	int n3 = nAngle & 0x00ff0000;
	int n4 = nAngle & 0xff000000;

	QByteArray ba;
	ba.push_back((unsigned char)n1);
	ba.push_back((unsigned char)(n2 >> 8));
	ba.push_back((unsigned char)(n3 >> 16));
	ba.push_back((unsigned char)(n4 >> 24));

	return ba;
}
