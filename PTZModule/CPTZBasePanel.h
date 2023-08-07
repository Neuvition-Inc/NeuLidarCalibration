#pragma once

#define  NewPTZ
#define PTZ_HOR_STEP 5.00
#define PTZ_VEC_STEP 3.00

#include <QWidget>
//#include "ui_CPTZBasePanel.h"
#include "CPTZBaseController.h"

typedef void(*pPTZfun)(float fHorAngle, float fVecAngle,bool bMoveFinish);//ˮƽ�Ƕȣ���ֱ�Ƕȣ��Ƿ����ƶ���ɵķ���ֵ

class CPTZBasePanel : public QWidget
{
	Q_OBJECT

public:
	CPTZBasePanel(CPTZBaseController *pBasePTZController,QWidget *parent = Q_NULLPTR);
	~CPTZBasePanel();
    bool PTZ_Up(float fStep = PTZ_VEC_STEP);
    bool PTZ_Down(float fStep = PTZ_VEC_STEP);
    bool PTZ_Left(float fStep = PTZ_HOR_STEP);
    bool PTZ_Right(float fStep = PTZ_HOR_STEP);
    bool PTZ_Switch();
    void PTZ_Reset();
    void PTZ_RegFun(pPTZfun fun);//ע��ص�����

public Q_SLOTS:
	void slotMoveFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
	void slotQueryFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
private:
	void _init_slot();
private:

	CPTZBaseController*  m_pBasePTZController = nullptr;
	bool    m_bPTZClose = true;
    pPTZfun m_fGetAngle;//����ָ��
    bool m_bGetAngle = false;//����ָ���־λ
signals:
    void sigMoveFinish(float fHorAngle, float fVecAngle);
    void sigQueryFinish(float fHorAngle, float fVecAngle);
};
