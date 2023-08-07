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
//void SlideRunReg(bool running)//是否在移动 但是内部采用0.5秒定时器读取，有一定延时
//{
//    qDebug()<<"SlideRunReg:running:"<<running;
//}
//void SlideDistance(float distance)//距离 单位mm 0.5秒更新一次
//{
//    qDebug()<<"SlideDistance:distance:"<<distance;
//}
//void PTZAngle(float fh,float fv,bool MoveEnd)//水平角度，垂直角度，true（移动完成）/false（实时回传）
//{
//    qDebug()<<"PTZAngle:fh,fv,MoveEnd:"<<fh<<","<<fv<<","<<MoveEnd;
//}
//void LaserDistance(LASER_DATA_T laserData)
//{
//    qDebug()<<"LaserDistance:laserData:"<<laserData.id<<laserData.distance;
//}
void Neuvition_Auto::demo()
{
    /////////////滑轨/////////////////////////////
//    //注册回调函数
    //this->m_pSlideControl->Slide_RegFun_Runing(SlideRunReg);
    //this->m_pSlideControl->Slide_RegFun_Distance(SlideDistance);
    ////选择要执行的PR设置的文件
    ////bool chooseFileRun(); 打开文件选择框选择文件 文件成功导入返回true
    ////滑轨运行需要导入文件
    //qDebug()<<this->m_pSlideControl->chooseFileRun
    //          ("D:/Qt/workspace/build-neu_auto_0713-Desktop_Qt_5_14_2_MinGW_64_bit-Debug/debug/SlideControl/20220712100120.txt");

    ////发送信号让机器执行PR命令，执行位置+1，执行位置超过最后一个会变回0，重头开始
    //emit this->m_pSlideControl->m_Sig_SubPlusPlus();//当前PR执行顺序下标++
    ////距离  也可以通过回调获取 单位mm
    //qDebug()<<this->m_pSlideControl->readLocationInfo();
    //emit this->m_pSlideControl->m_Sig_SubPlusPlus();//当前PR执行顺序下标++
    ////判断是否在运行，可以用回调代替
    //qDebug()<<this->m_pSlideControl->getISRuning();
    /*
     * QVector<PRSET_T> getPRSet();获取要执行的PR命令
    int getSeqListNum();获取当前将要执行的位置
    int getCurrentSeqPos();获取总数
    //filename
    QString getFileName();
    //Serial
    bool getSerialOpen();
    */

    //////////云台/////////////////////////////////
    //注册回调函数
    //this->m_pPTZPanel->PTZ_RegFun(PTZAngle);
    //云台开关
//    this->m_pPTZPanel->PTZ_Switch();
    //云台移动 存在默认参数 水平默认5度，垂直默认3度  垂直范围（-60-+60）
    //this->m_pPTZPanel->PTZ_Up();
//    this->m_pPTZPanel->PTZ_Down(30);
    //this->m_pPTZPanel->PTZ_Left();
//    this->m_pPTZPanel->PTZ_Right(30);
    //复位
    //this->m_pPTZPanel->PTZ_Reset();
    /*
     *
    void slotMoveFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
    void slotQueryFinish(enPTZ_CMD_TYPE ptzCmdType, float fHorAngle, float fVecAngle);
    这两个槽函数中分别传出信号
    void sigMoveFinish(float fHorAngle, float fVecAngle); 水平角度和垂直角度
    void sigQueryFinish(float fHorAngle, float fVecAngle);
    */

    ///////////激光测距仪////////////////////////////
    //注册回调函数
//    this->m_pLaserControl->Laser_RegFun(LaserDistance);
//    //读取距离  数据通过回调函数获取
//    this->m_pLaserControl->getDistanceByID(18);//这三个激光测距仪一定要18
    /*
     * void m_sigUpdatedLaserData(LASER_DATA_T m_LaserData);  也可以通过这个信号获取数据
    */

}
