#include "Neuvition_Auto.h"
#include <QDebug>
#include <iostream>


#include<QThread>
Neuvition_Auto::Neuvition_Auto(QObject *parent) : QObject(parent)
{

	std::cout << "Neuvition_Auto  threadid = " << QThread::currentThreadId << std::endl;
    this->m_pSlideControl = new SlideControl();
    this->m_pLaserControl = new LaserInstrumentControl();
    CPTZBaseController *pBasePTZController = new CPTZBaseController;
    this->m_pPTZPanel = new CPTZBasePanel(pBasePTZController);

}

Neuvition_Auto::~Neuvition_Auto()
{
    delete m_pSlideControl;
    delete m_pLaserControl;
    delete m_pPTZPanel;
}




////////////////////////demo/////////////////////////////////
//void SlideRunReg(bool running)//�Ƿ����ƶ� �����ڲ�����0.5�붨ʱ����ȡ����һ����ʱ
//{
//    qDebug()<<"SlideRunReg:running:"<<running;
//}
//void SlideDistance(float distance)//���� ��λmm 0.5�����һ��
//{
//    qDebug()<<"SlideDistance:distance:"<<distance;
//}
//void PTZAngle(float fh,float fv,bool MoveEnd)//ˮƽ�Ƕȣ���ֱ�Ƕȣ�true���ƶ���ɣ�/false��ʵʱ�ش���
//{
//    qDebug()<<"PTZAngle:fh,fv,MoveEnd:"<<fh<<","<<fv<<","<<MoveEnd;
//}
//void LaserDistance(LASER_DATA_T laserData)
//{
//    qDebug()<<"LaserDistance:laserData:"<<laserData.id<<laserData.distance;
//}
void Neuvition_Auto::demo()
{
    /////////////����/////////////////////////////
//    //ע��ص�����
    //this->m_pSlideControl->Slide_RegFun_Runing(SlideRunReg);
    //this->m_pSlideControl->Slide_RegFun_Distance(SlideDistance);
    ////ѡ��Ҫִ�е�PR���õ��ļ�
    ////bool chooseFileRun(); ���ļ�ѡ���ѡ���ļ� �ļ��ɹ����뷵��true
    ////����������Ҫ�����ļ�
    //qDebug()<<this->m_pSlideControl->chooseFileRun
    //          ("D:/Qt/workspace/build-neu_auto_0713-Desktop_Qt_5_14_2_MinGW_64_bit-Debug/debug/SlideControl/20220712100120.txt");

    ////�����ź��û���ִ��PR���ִ��λ��+1��ִ��λ�ó������һ������0����ͷ��ʼ
    //emit this->m_pSlideControl->m_Sig_SubPlusPlus();//��ǰPRִ��˳���±�++
    ////����  Ҳ����ͨ���ص���ȡ ��λmm
    //qDebug()<<this->m_pSlideControl->readLocationInfo();
    //emit this->m_pSlideControl->m_Sig_SubPlusPlus();//��ǰPRִ��˳���±�++
    ////�ж��Ƿ������У������ûص�����
    //qDebug()<<this->m_pSlideControl->getISRuning();
    /*
     * QVector<PRSET_T> getPRSet();��ȡҪִ�е�PR����
    int getSeqListNum();��ȡ��ǰ��Ҫִ�е�λ��
    int getCurrentSeqPos();��ȡ����
    //filename
    QString getFileName();
    //Serial
    bool getSerialOpen();
    */

    //////////��̨/////////////////////////////////
    //ע��ص�����
    //this->m_pPTZPanel->PTZ_RegFun(PTZAngle);
    //��̨����
//    this->m_pPTZPanel->PTZ_Switch();
    //��̨�ƶ� ����Ĭ�ϲ��� ˮƽĬ��5�ȣ���ֱĬ��3��  ��ֱ��Χ��-60-+60��
    //this->m_pPTZPanel->PTZ_Up();
//    this->m_pPTZPanel->PTZ_Down(30);
    //this->m_pPTZPanel->PTZ_Left();
//    this->m_pPTZPanel->PTZ_Right(30);
    //��λ
    //this->m_pPTZPanel->PTZ_Reset();
    /*
     *
    void slotMoveFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
    void slotQueryFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
    �������ۺ����зֱ𴫳��ź�
    void sigMoveFinish(float fHorAngle, float fVecAngle); ˮƽ�ǶȺʹ�ֱ�Ƕ�
    void sigQueryFinish(float fHorAngle, float fVecAngle);
    */

    ///////////��������////////////////////////////
    //ע��ص�����
//    this->m_pLaserControl->Laser_RegFun(LaserDistance);
//    //��ȡ����  ����ͨ���ص�������ȡ
//    this->m_pLaserControl->getDistanceByID(18);//��������������һ��Ҫ18
    /*
     * void m_sigUpdatedLaserData(LASER_DATA_T m_LaserData);  Ҳ����ͨ������źŻ�ȡ����
    */

}
