#pragma once

#include <QObject>

#include "CPTZCore.h"
#include "CPTZUdpCore.h"
enum class enPTZ_CMD_TYPE {
	CMD_HOR_ROTATE = 0,
	CMD_VEC_ROTATE,
	CMD_HOR_VEC_ROTATE,
	CMD_RESET,
	CMD_QUERY_HOR_ANGLE,
	CMD_QUERY_VEC_ANGLE
};



class CPTZBaseController : public QObject
{
	Q_OBJECT

public:
	CPTZBaseController(bool bUdpType = true, QObject *parent = nullptr);
	~CPTZBaseController();

	

	//ֹͣ����
	void cmdStop();

	void cmdUp(int nSpeed = 10);
	void cmdDown(int nSpeed = 10);
	void cmdLeft(int nSpeed = 10);
	void cmdRight(int nSpeed = 10);

	//ˮƽ��ת��ָ���Ƕ�
	void cmdHorRotateToAngle(float fAngle = 0.00); // fAngle = 0.00 -360.00
	void cmdVecRotateToAngle(float fAngle = 0.00); // -60.00 - 60.00
	void cmdHorAndVecRotateToAngle(float fHorAngle = 0.0, float fVecAngle = 0.0);

	void cmdReset();

	//���󷵻ص�ǰˮƽ�Ƕ�
	void cmdReqHorAngle();
	//���󷵻ص�ǰ��ֱ�Ƕ�
	void cmdReqVecAngle();

	//���ڿ���
	void close();
	bool open();

	//��ȡ�Ƕ�
	float getCurHorAngle() {
		return m_fCurHorAngle;
	}
	float getCurVecAngle() {
		return m_fCurVecAngle;
	}

	//ע��ص�����
	void registerRealAngleReturnCB(PTZRealAngleReturnCB ptzRealAngleRetCB);
	//ע���ص�����
	void unregisterRealAngleReturnCB();

public Q_SLOTS:
	void slotOpen();
	void slotClose();
	void slotHorAndVecRotateToAngle(float fHorAngle, float fVecAngle);
	void slotHorRotateToAngle(float fHorAngle);
	void slotVecRotateToAngle(float fVecAngle);


private Q_SLOTS:
	void slotReadData(const QByteArray baReadData);

Q_SIGNALS:
	void sigMoveFinish(enPTZ_CMD_TYPE ptzCmdType, float  fHorAngle, float fVecAngle);
	void sigQureyFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
private:
	void _init_slots();
	//������Ϣ����
	void parseReadInfo(QByteArray baReadInfo);
	void parseReadInfo_NewPTZ(QByteArray baReadInfo);
private:
	bool      m_bUdpType = true;// false: ����ͨ�ţ� true : udp ͨ��
	bool      m_bAlreadyOpen = false;//�����Ƿ��
	CPTZCore   m_PTZCore;
	CPTZUdpCore  m_PTZUdpCore;


	enPTZ_CMD_TYPE    mCmdType = enPTZ_CMD_TYPE::CMD_HOR_ROTATE;
	bool            mHorFinish = false;
	bool            mVecFinish = false;

	float           m_fCurHorAngle = 0.0;
	float           m_fCurVecAngle = 0.0;

};
