#include "CPTZBasePanel.h"
#include <QDir>
#include <QCoreApplication>

CPTZBasePanel::CPTZBasePanel(CPTZBaseController *pBasePTZController, QWidget *parent /*= Q_NULLPTR*/)
	: QWidget(parent)
{
	m_pBasePTZController = pBasePTZController;
	_init_slot();
    //云台的垂直旋转角度和水平旋转角度
    QDir dir(QCoreApplication::applicationDirPath()+"/PTZ");
    if(!dir.exists())
    {
        dir.mkpath(QCoreApplication::applicationDirPath()+"/PTZ");
    }
    QString filenameV = QCoreApplication::applicationDirPath()+"/PTZ/VerticalAngle.txt";
    QString filenameH = QCoreApplication::applicationDirPath()+"/PTZ/LevelAngle.txt";
    QFile fileV(filenameV);
    QFile fileH(filenameH);
    if((!fileV.exists()) || (!fileH.exists()))
    {
        fileV.open(QIODevice::WriteOnly);
        fileH.open(QIODevice::WriteOnly);
        for(int i = 0; i <= 45; i += 5)
        {
            if(i == 45)
            {
                fileV.write(QString("%1\n").arg(i).toUtf8());
                fileH.write(QString("%1\n").arg(i).toUtf8());
            }
            else
            {
                fileV.write(QString("%1,").arg(i).toUtf8());
                fileH.write(QString("%1,").arg(i).toUtf8());
            }
        }
        for(int i = -5; i >= -45; i -= 5)
        {
            if(i == -45)
            {
                fileV.write(QString("%1,0\n").arg(i).toUtf8());
                fileH.write(QString("%1,0\n").arg(i).toUtf8());
            }
            else
            {
                fileV.write(QString("%1,").arg(i).toUtf8());
                fileH.write(QString("%1,").arg(i).toUtf8());
            }
        }
        fileV.close();
        fileH.close();
    }
}

CPTZBasePanel::~CPTZBasePanel()
{
}

bool CPTZBasePanel::PTZ_Up(float fStep)
{
    float fCurAngle = m_pBasePTZController->getCurVecAngle();
    float fFinalAngle = fCurAngle + fStep;
    bool bRet = true;
    //超过+60或-60按照极限处理
    if(fFinalAngle < -60 || fFinalAngle > 60)
    {
        fFinalAngle = 60;
        bRet = false;
    }
    if (m_pBasePTZController) {
        m_pBasePTZController->cmdVecRotateToAngle(fFinalAngle);
    }
    return bRet;
}

bool CPTZBasePanel::PTZ_Down(float fStep)
{
    float fCurAngle = m_pBasePTZController->getCurVecAngle();
    float fFinalAngle = fCurAngle - fStep;
    //超过+60或-60按照极限处理
    bool bRet = true;
    if(fFinalAngle < -60 || fFinalAngle >60){
        fFinalAngle = -60;
        bRet = false;
    }
    if (m_pBasePTZController) {
        m_pBasePTZController->cmdVecRotateToAngle(fFinalAngle);
    }
    return bRet;
}

bool CPTZBasePanel::PTZ_Left(float fStep)
{
    if(fStep > 360 || fStep < 0){
        return false;
    }
    float fCurAngle = m_pBasePTZController->getCurHorAngle();
    float fFinalAngle = fCurAngle - fStep;
    fFinalAngle = fFinalAngle >= 0.0 ? fFinalAngle : fFinalAngle + 360.0;
    if (m_pBasePTZController) {
        m_pBasePTZController->cmdHorRotateToAngle(fFinalAngle);
    }
    return true;
}

bool CPTZBasePanel::PTZ_Right(float fStep)
{
    if(fStep > 360 || fStep < 0){
        return false;
    }
    float fCurAngle = m_pBasePTZController->getCurHorAngle();
    float fFinalAngle = fCurAngle + fStep;
    fFinalAngle = fFinalAngle < 360.0 ? fFinalAngle : fFinalAngle - 360.0;
    if (m_pBasePTZController) {
        m_pBasePTZController->cmdHorRotateToAngle(fFinalAngle);
    }
    return true;
}

bool CPTZBasePanel::PTZ_Switch()
{
    qDebug()<<"PTZ_Switch:IN :"<<endl;
    if (m_bPTZClose)
    {
        bool bRet = m_pBasePTZController->open();
        qDebug()<<"PTZ_Switch:bRet:"<<bRet;
        if (!bRet)
        {
            m_bPTZClose = !m_bPTZClose;
        }
    }
    else
    {
        qDebug()<<"PTZ_Switch:m_bPTZClose:"<<m_bPTZClose;
        m_pBasePTZController->close();
    }
    m_bPTZClose = !m_bPTZClose;
    return m_bPTZClose;
}

void CPTZBasePanel::PTZ_Reset()
{
    m_pBasePTZController->cmdReset();
}

void CPTZBasePanel::PTZ_RegFun(pPTZfun fun)
{
    this->m_fGetAngle = fun;
    this->m_bGetAngle = true;
	qDebug()<< "CPTZBasePanel::PTZ_RegFun(pPTZfun fun)";

}

void CPTZBasePanel::slotMoveFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle)
{
    if(this->m_bGetAngle)
        this->m_fGetAngle(fHorAngle,fVecAngle,true);
    emit sigMoveFinish(fHorAngle, fVecAngle);
	qDebug() << "slotMoveFinish fHorAngle:" << fHorAngle<<"  fVecAngle:"<< fVecAngle;
}

void CPTZBasePanel::slotQueryFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle)
{
    if(this->m_bGetAngle)
        this->m_fGetAngle(fHorAngle,fVecAngle,false);
    emit sigQueryFinish(fHorAngle, fVecAngle);
	//qDebug() << "slotQueryFinish fHorAngle:" << fHorAngle << "  fVecAngle:" << fVecAngle;
}

void CPTZBasePanel::_init_slot()
{
	connect(m_pBasePTZController, &CPTZBaseController::sigQureyFinish, this, &CPTZBasePanel::slotQueryFinish);
	connect(m_pBasePTZController, &CPTZBaseController::sigMoveFinish, this, &CPTZBasePanel::slotMoveFinish);
}


