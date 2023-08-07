#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include "Neuvition_Auto.h"
#include "./Common/VTKModule/CVTKWidget.h"
#include "./Common/LidarModule/CLidarINeuvEvent.h"
#include "./Common/LidarModule/CNeuvLidarControl.h"

#include "CalibrationProcess.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

typedef struct Laser{
    float name018;
    float name110;
    float name111;
}LASER_T;

namespace Ui {
class MainWin;
}

class MainWin : public QWidget
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = nullptr);
    ~MainWin();

    void Connect();
    void RegFun();
    //void PTZ_getAngle(float fH,float fV,bool bMoveFinsh);

    void InitLidar();
    void ReadLidarIni();
    void ClearData();

    void startThread();
Q_SIGNALS:
    void signalMessageBox(const QString& title, const QString& text);
    void signalpbOriginCorrection();
    void signalpbDepthCorrection();
	void signalpbDepthCollect(float laserdistance);
    void signalpbXYPositionCorrection();
	void signalPTZMove(int direction, float Angle);//方向和角度 0左，1右，2上，3下
	void signalLaserInstrument();//单点激光测距开启
	void signalSliderMove();//滑轨移动

	void signalLaserEnd(float distance);
	void signalSlideDis(float distance);
	void signalAngle(float H_angle, float V_angle);
	void signalProgressBar(float num);

private slots:
    void m_ChooseFileBtn_slot();
    void m_StartBtn_slot();
	void m_PTZMove_slot(int direction, float Angle);
	void m_LaserInstrument_slot();
	void m_SliderMove_slot();

	void m_SaveObjPCL_slot();

	void slotLaserEnd(float distance);
	void slotSlideDis(float distance);
	void slotAngle(float H_angle, float V_angle);
	void slotProgressBar(float num);



//    void PTZMoveFinish(float fHorAngle, float fVecAngle);
//    void PTZQueryFinish(float fHorAngle, float fVecAngle);

//    void btnRight_clicked();
//    void btnLeft_clicked();
//    void btnDown_clicked();
//    void btnUp_clicked();
//    void btnReset_clicked();
//    void btnPTZSwitch_clicked();
//    void updateDistance(LASER_DATA_T m_LaserData);
//    void m_getDistanceBtn_clicked();
//    void m_runBtn_clicked();
//    void pushButton_2_clicked();
//    void pushButton_clicked();
    void timer_TimeOut();


    //lidar相关

//    void slotLidarFpsChanged(int nIndex);
//    void slotLidarRateChanged(int nIndex);
//    void slotLineeditFinished();

    //数据处理
    void slotOriginCorrection();
    void slotDepthCorrection();
	void slotDepthCollect(float laserdistance);
    void slotXYPosCorrection();
    void slotMessageBox(const QString& title, const QString& text);

    void threadOriginCorrection();
    void threadDepthCorrection();
	void threadDepthCollect(float laserdistance);
    void threadXYPosCorrection();


    void threadAutoCalibration();
	void threadAutoCalibration_();
private:
    Ui::MainWin *ui;

	bool m_isOrigin = true;//是否进行原点标定
	int m_collectDepthNum = 0;// 采集8次时，进行校正
	bool m_isDepth = true;//是否进行深度标定
	bool m_isXY = true;//是否进行xy轴校正

    Neuvition_Auto* m_pNeuvition_Auto;
    bool m_bSliderFileOpen;//判断是否加载文件
    int m_iSliderNum = 1;//文件里面的数量
    int m_iSliderCurrentNum = 0;//当前位置
    QStandardItemModel* m_SeqListModel;
    QTimer* m_pTimer;
    //float m_fSliderDistance;
    QVector<float> m_vPTZVAngle;//垂直旋转角度
    QVector<float> m_vPTZHAngle;//水平旋转角度
    QVector<LASER_T> m_vLaserDatas;//单点激光测距

    //lidar相关
    CVTKWidget*    m_pVtkWidget = nullptr;
    CNeuvLidarControl* m_pLidarControl = nullptr;
    bool           m_bConnLidar = false;
    std::vector<int>  m_vec_fps;
    std::map<int, std::string>  m_map_lidarRete;
    QString m_sLidarIP,m_sLidarPower,m_sLidarRepetition,m_sLidarFrame;


    QVBoxLayout *vLayout_VTK;
};

#endif // MAINWIN_H
