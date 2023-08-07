#include "CPTZBaseController.h"
//#include "../LogModule/NeuLogger.h"



CPTZBaseController::CPTZBaseController(bool bUdpType /*= false*/, QObject *parent /*= nullptr*/)
	: QObject(parent)
{
	m_bUdpType = bUdpType;
	_init_slots();
}


CPTZBaseController::~CPTZBaseController()
{
}

void CPTZBaseController::cmdStop()
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_stop();
	}
	else 
	{
		m_PTZCore._cmd_Stop();
	}
	
}

void CPTZBaseController::cmdUp(int nSpeed /*= 10*/)
{
	if (nSpeed < 0 || nSpeed >64)
	{
		//logger_info << "cmdUp :Unreasonable speed setting ";
		//logger_ptz_error << "cmdUp :Unreasonable speed setting " ;
		return;
	}

	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Up(nSpeed);
	}
	else 
	{
		m_PTZCore._cmd_Up(nSpeed);
	}
	
}

void CPTZBaseController::cmdDown(int nSpeed /*= 10*/)
{
	if (nSpeed < 0 || nSpeed >64)
	{
		//logger_ptz_error << "cmdDown :Unreasonable speed setting ";
		return;
	}
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Down(nSpeed);
	}
	else
	{
		m_PTZCore._cmd_Down(nSpeed);
	}
}

void CPTZBaseController::cmdLeft(int nSpeed /*= 10*/)
{
	if (nSpeed < 0 || nSpeed >64)
	{
		return;
	}
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Left(nSpeed);
	}
	else
	{
		m_PTZCore._cmd_Left(nSpeed);
	}
}

void CPTZBaseController::cmdRight(int nSpeed /*= 10*/)
{
	if (nSpeed < 0 || nSpeed >64)
	{
		return;
	}
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Rigth(nSpeed);
	}
	else
	{
		m_PTZCore._cmd_Rigth(nSpeed);
	}
}

void CPTZBaseController::cmdHorRotateToAngle(float fAngle /*= 0.00*/)
{
	mHorFinish = false; //ˮƽ�ƶ�δ����
	mCmdType = enPTZ_CMD_TYPE::CMD_HOR_ROTATE;

	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_HorRotateToAngle(fAngle);
	}
	else
	{
		m_PTZCore._cmd_HorRotateToAngle(fAngle);
	}
}

void CPTZBaseController::cmdVecRotateToAngle(float fAngle /*= 0.00*/)
{
	mVecFinish = false; //ˮƽ�ƶ�δ����
	mCmdType = enPTZ_CMD_TYPE::CMD_VEC_ROTATE;

	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_VecRotateToAngle(fAngle);
	}
	else
	{
		m_PTZCore._cmd_VecRotateToAngle(fAngle);
	}
}

void CPTZBaseController::cmdHorAndVecRotateToAngle(float fHorAngle /*= 0.0*/, float fVecAngle /*= 0.0*/)
{
	mHorFinish = false; //ˮƽ�ƶ�δ����
	mVecFinish = false;
	mCmdType = enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE;

	//��ֱ��
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_VecRotateToAngle(fVecAngle);
		int n = 10000;
		while (n--)
		{
			
		}
		m_PTZUdpCore.cmd_HorRotateToAngle(fHorAngle);
	}
	else
	{
		m_PTZCore._cmd_VecRotateToAngle(fVecAngle);
		int n = 10000;
		while (n--)
		{

		}
		m_PTZCore._cmd_HorRotateToAngle(fHorAngle);
	}
	
	
}

void CPTZBaseController::cmdReset()
{
	//logger_ptz_info << "cmdReset begin";
	mHorFinish = false; //ˮƽ�ƶ�δ����
	mVecFinish = false;
	mCmdType = enPTZ_CMD_TYPE::CMD_RESET;

	//���µ���Ԥ��λ0
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Call_Datum_Position();

		//��ֱ
		m_PTZUdpCore.cmd_VecRotateToAngle(360.0);
		//logger_ptz_info << "cmdReset meddite";
		m_PTZUdpCore.cmd_HorRotateToAngle(0.0);
		//logger_ptz_info << "cmdReset end";
	}
	else
	{
		m_PTZCore._cmd_Call_Datum_Position();

		//��ֱ
		m_PTZCore._cmd_VecRotateToAngle(360.0);
		//logger_ptz_info << "cmdReset meddite";
		m_PTZCore._cmd_HorRotateToAngle(0.0);
		//logger_ptz_info << "cmdReset end";
	}
	

	
}

void CPTZBaseController::cmdReqHorAngle()
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Req_HorAngle();
	}
	else
	{
		m_PTZCore._cmd_Req_HorAngle();
	}
}

void CPTZBaseController::cmdReqVecAngle()
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.cmd_Req_VecAngle();
	}
	else
	{
		m_PTZCore._cmd_Req_VecAngle();
	}
}

void CPTZBaseController::close()
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.close();
	}
	else 
	{
		m_PTZCore.closeSerialPort();
		
	}
	m_bAlreadyOpen = false;
	
}

bool CPTZBaseController::open()
{
	if (m_bAlreadyOpen)
	{
		return true;
	}

	if (m_bUdpType)
	{
		m_PTZUdpCore.open("192.168.8.200", 6666); //��̨��udp��ip�Ͷ˿�

		static bool bFirst = true;
		if (bFirst)
		{
			bFirst = false;
			m_PTZUdpCore.cmd_On_Return_Angle_After_Reach(); //����̨�����Ƕ�������ָ��λ�ú�ش��Ĺ���

			//�����Ƕ�ʵʱ�ش�����
			m_PTZUdpCore.cmd_On_Angle_Return(0);	
		}
		//��ѯ��ǰ�ĽǶ�ֵ
		mCmdType = enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE;
		m_PTZUdpCore.cmd_Req_HorAngle();
		m_PTZUdpCore.cmd_Req_VecAngle();

		m_bAlreadyOpen = true;

	}
	else 
	{
		QString availPortName = "";
		if (!m_PTZCore.testSerialAllPort(availPortName) || availPortName.isEmpty())
		{
			//��ǰ����û�п��õĴ���
			QString error = QString("Serial Port Scan failed,No serial device available ");
			return false;
		}

		if (!m_PTZCore.openSerialPort(availPortName))
		{
			//��ǰ����û�п��õĴ���
			QString error = QString("Serial Port Scan failed, error code is : ") + QString::number(m_PTZCore.getErrorCode());
			return false;
		}

		m_PTZCore._cmd_OpenCBAngleInfo(true); //����̨�����Ƕ�������ָ��λ�ú�ش��Ĺ���

		//
		//m_PTZCore._cmd_On_Angle_Return(0); //����������Ч�ġ�


		//��ѯ��ǰ�ĽǶ�ֵ
		mCmdType = enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE;
		m_PTZCore._cmd_Req_HorAngle();
		m_PTZCore._cmd_Req_VecAngle();

		m_bAlreadyOpen = true;
		
	}
	return true;
	
}

void CPTZBaseController::registerRealAngleReturnCB(PTZRealAngleReturnCB ptzRealAngleRetCB)
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.registerRealAngleReturnCB(ptzRealAngleRetCB);
	}
}

void CPTZBaseController::unregisterRealAngleReturnCB()
{
	if (m_bUdpType)
	{
		m_PTZUdpCore.unregisterRealAngleReturnCB();
	}
}

#include <QDebug>
#include <QThread>
void CPTZBaseController::slotOpen()
{
	qDebug() << "=----slotOpen  " << QThread::currentThreadId() <<endl;
	open();
}

void CPTZBaseController::slotClose()
{
	close();
}

void CPTZBaseController::slotHorAndVecRotateToAngle(float fHorAngle, float fVecAngle)
{
	cmdHorAndVecRotateToAngle(fHorAngle, fVecAngle);
}

void CPTZBaseController::slotHorRotateToAngle(float fHorAngle)
{
	cmdHorRotateToAngle(fHorAngle);
}

void CPTZBaseController::slotVecRotateToAngle(float fVecAngle)
{
	cmdVecRotateToAngle(fVecAngle);
}

void CPTZBaseController::slotReadData(const QByteArray baReadData)
{
    //qDebug()<<"slotReadData";
#ifdef OLDPTZ
	parseReadInfo(baReadData);
#else 
	parseReadInfo_NewPTZ(baReadData);
#endif
}

void CPTZBaseController::_init_slots()
{
	//�ź�-��
	QObject::connect(&m_PTZCore, &CPTZCore::sigReadData, this, &CPTZBaseController::slotReadData);
	QObject::connect(&m_PTZUdpCore, &CPTZUdpCore::sigReadData, this, &CPTZBaseController::slotReadData);

}

void CPTZBaseController::parseReadInfo(QByteArray baReadInfo)
{
	int a = 0, b = 0;
	float fAngle = 0.0;
	float fMoveAngle = 0.0;


	switch (baReadInfo.at(3))
	{
	case 0x59:
		// ˮƽ�ǶȻش�����
		a = baReadInfo.at(4);
		b = baReadInfo.at(5);

		if (a < 0)
			a += 256;
		if (b < 0)
			b += 256;
		fAngle = (a * 256 + b) / 100.0;
		m_fCurHorAngle = fAngle;
		mHorFinish = true;
		if (enPTZ_CMD_TYPE::CMD_HOR_ROTATE == mCmdType)
		{
			emit sigMoveFinish(mCmdType, m_fCurHorAngle, 0);
		}
		else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
		{
			if (mVecFinish)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
		}
		else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
		{
			if (mVecFinish)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
		}


		break;
	case 0x5b:
		// �����ǶȻش�����
		a = baReadInfo.at(4);
		b = baReadInfo.at(5);

		if (a < 0)
			a += 256;
		if (b < 0)
			b += 256;
		fAngle = (a * 256 + b) / 100.0;
		m_fCurVecAngle = fAngle;
		mVecFinish = true;
		if (enPTZ_CMD_TYPE::CMD_VEC_ROTATE == mCmdType)
		{
			emit sigMoveFinish(mCmdType, 0, m_fCurVecAngle);
		}
		else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
		{
			if (mHorFinish)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
		}
		else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
		{
			if (mHorFinish)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
		}
		break;
	default:
		break;
	}
}
#include <QTime>
void CPTZBaseController::parseReadInfo_NewPTZ(QByteArray baReadInfo)
{
	int a = 0, b = 0;
	float fAngle = 0.0;
	float fMoveAngle = 0.0;

    //qDebug()<<"parseReadInfo_NewPTZ:"<<baReadInfo.toHex(':');
	if ((unsigned char)baReadInfo.at(2) == 0xC5)
	{
		if ((unsigned char)baReadInfo.at(3) == 0x02)
		{
			// ˮƽ�ǶȻش�����
			fAngle = m_PTZCore._transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), false);
			m_fCurHorAngle = fAngle;
			mHorFinish = true;
			if (enPTZ_CMD_TYPE::CMD_HOR_ROTATE == mCmdType)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
			else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
			{
				if (mVecFinish)
				{
					emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
			else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
			{
				if (mVecFinish)
				{
					emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
		}

		if ((unsigned char)baReadInfo.at(3) == 0x03)
		{
			// �����ǶȻش�����
			fAngle = m_PTZCore._transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), true);
			m_fCurVecAngle = fAngle;
			mVecFinish = true;
			if (enPTZ_CMD_TYPE::CMD_VEC_ROTATE == mCmdType)
			{
				emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
			else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
			{
				if (mHorFinish)
				{
					emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
			else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
			{
				if (mHorFinish)
				{
					emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}

		}
	}
	else
	{
		switch ((unsigned char)baReadInfo.at(3))
		{
		case 0x59:
			// ˮƽ�ǶȻش�����
			fAngle = m_PTZCore._transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), false);
			m_fCurHorAngle = fAngle;
			mHorFinish = true;
			if (enPTZ_CMD_TYPE::CMD_HOR_ROTATE == mCmdType)
			{
				//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				
			}
			else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
			{
				if (mVecFinish)
				{
					//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
					emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
			else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
			{
				if (mVecFinish)
				{
					//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
					emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}


			break;
		case 0x5b:
			// �����ǶȻش�����

			fAngle = m_PTZCore._transFromCharToFloat(baReadInfo.at(4), baReadInfo.at(5), true);
			m_fCurVecAngle = fAngle;
			mVecFinish = true;
			if (enPTZ_CMD_TYPE::CMD_VEC_ROTATE == mCmdType)
			{
				//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
			}
			else if (enPTZ_CMD_TYPE::CMD_RESET == mCmdType)
			{
				if (mHorFinish)
				{
					//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
					emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
			else if (enPTZ_CMD_TYPE::CMD_HOR_VEC_ROTATE == mCmdType)
			{
				if (mHorFinish)
				{
					//emit sigMoveFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
					emit sigQureyFinish(mCmdType, m_fCurHorAngle, m_fCurVecAngle);
				}
			}
			break;
		default:
			break;
		}
	}
}
