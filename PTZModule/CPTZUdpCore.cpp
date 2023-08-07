#include "CPTZUdpCore.h"


#include <iostream>
#include <QDebug>

#include <QThread>


CPTZUdpCore::CPTZUdpCore(QString sIp, int nPort /*= 6666*/, QObject *parent /*= nullptr*/)
	: QObject(parent)
	, m_hostaddr(QHostAddress(sIp))
	, m_nPort(nPort)
{
	// 这个SendSocket 既是发送端也是接收端。
	connect(&m_SendSocket, &QUdpSocket::readyRead, this, &CPTZUdpCore::slotReceiveData);
}

CPTZUdpCore::CPTZUdpCore(QObject *parent /*= nullptr*/)
{
	// 这个SendSocket 既是发送端也是接收端。
	connect(&m_SendSocket, &QUdpSocket::readyRead, this, &CPTZUdpCore::slotReceiveData);
}

CPTZUdpCore::~CPTZUdpCore()
{
}

bool CPTZUdpCore::cmd_On_Angle_Return(int nMicSec /*= 0*/)
{
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

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Off_Angle_Return()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0xE1;
	pCmd[3] = 0x02;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_On_Return_Angle_After_Reach()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0xC5;
	pCmd[3] = 0x01;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;
	
	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Off_Return_Angle_After_Reach()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[2] = 0xC5;
	pCmd[3] = 0x00;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_HorRotateToAngle(float fAngle /*= 0.00*/)
{
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

	return _send(pCmd, 7);

}

bool CPTZUdpCore::cmd_VecRotateToAngle(float fAngle /*= 0.00*/)
{
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

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_stop()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[6] = 0x01;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Req_HorAngle()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x51;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Req_VecAngle()
{
	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x53;
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);

}

bool CPTZUdpCore::cmd_Up(int nSpeed)
{
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x08; //向上
	pCmd[5] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Down(int nSpeed)
{
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x10; //向下
	pCmd[5] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Left(int nSpeed)
{
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x04;
	pCmd[4] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Rigth(int nSpeed)
{
	unsigned char cSpeed = nSpeed;

	unsigned char pCmd[7] = { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	pCmd[1] = m_PtzAddr;
	pCmd[3] = 0x02;
	pCmd[4] = cSpeed; //速度为10 ，从0x01-0x40,即1-64速度。
	pCmd[6] = (pCmd[1] + pCmd[2] + pCmd[3] + pCmd[4] + pCmd[5]) & 0xff;

	return _send(pCmd, 7);
}

bool CPTZUdpCore::cmd_Call_Datum_Position()
{
	return true;
}

bool CPTZUdpCore::open(QString sIp /*= "192.168.8.200"*/, int nPort /*= 6666*/)
{
	m_hostaddr = QHostAddress(sIp);
	m_nPort = nPort;

	m_bOpen = true;

	return true;
}

void CPTZUdpCore::close()
{
	//disconnect(&m_SendSocket, &QUdpSocket::readyRead, this, &CPTZUdpCore::slotReceiveData);
	m_bOpen = false;
}


void CPTZUdpCore::registerRealAngleReturnCB(PTZRealAngleReturnCB ptzRealAngleRetCB)
{
	m_ptzRealAngleRetCB = ptzRealAngleRetCB;
}

void CPTZUdpCore::unregisterRealAngleReturnCB()
{
	m_ptzRealAngleRetCB = nullptr;
}

void CPTZUdpCore::slotReceiveData()
{
	QByteArray ba;
//    qDebug() << "slotReceivieData = " << "one " << endl;
	while (m_SendSocket.hasPendingDatagrams())
	{
		ba.resize(m_SendSocket.pendingDatagramSize());
		m_SendSocket.readDatagram(ba.data(), ba.size());
//        qDebug()<<"m_bOpen:"<<m_bOpen;
		if (m_bOpen)
		{
            //qDebug() << "slotReceivieData = " << ba.toHex(':') << endl;
			emit sigReadData(ba);

			if (_parseReturnAngle(ba))
			{
		

				if (m_fCurHorAngle != INVALID_ANGLE && m_fCurVecAngle != INVALID_ANGLE && m_ptzRealAngleRetCB)
				{
					m_ptzRealAngleRetCB(m_fCurHorAngle, m_fCurVecAngle);
				}
			}
		}

	}
}

bool CPTZUdpCore::_send(unsigned char* pCmd, int nLen /*= 7*/)
{
	std::cout << "_send  threadid = " << QThread::currentThreadId << std::endl;
	QHostAddress  hostaddr = m_hostaddr;// QHostAddress("192.168.8.200");
	qint64 nSize = m_SendSocket.writeDatagram((char*)pCmd, nLen, hostaddr, m_nPort);
	if (nSize < 0)
	{
		QByteArray baCmd((char*)pCmd,7);
		qDebug() << "Error : _send data failed! " << baCmd.toHex(':')
			<< " Error info = " << m_SendSocket.error() << endl;
		return false;
	}
	else 
	{
		QByteArray baCmd((char*)pCmd,7);
        //qDebug() << "_send info = " << baCmd.toHex(':') << endl;
	}
	return true;
}

bool CPTZUdpCore::_parseReturnAngle(const QByteArray baReadInfo)
{
	if (baReadInfo.size() != 7)
	{
		return false;
	}

	bool bReturnAngleCmd = false;
	switch ((unsigned char)baReadInfo.at(3))
	{
	case 0x59:
		// 水平角度回传数据
		m_fCurHorAngle = _transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), false);
		bReturnAngleCmd = true;
		break;
	case 0x5b:
		// 俯仰角度回传数据
		m_fCurVecAngle = _transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), true);
		bReturnAngleCmd = true;
		break;
	default:
		break;
	}

	return bReturnAngleCmd;
}

float CPTZUdpCore::_transFromCharToFloat(char HChar, char LChar, bool bVecAngle)
{
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
}
