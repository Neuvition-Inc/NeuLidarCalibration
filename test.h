#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include "Neuvition_Auto.h"
#include "./Common/VTKModule/CVTKWidget.h"
#include "./Common/LidarModule/CLidarINeuvEvent.h"
#include "./Common/LidarModule/CNeuvLidarControl.h"
#include "CalibrationProcess.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();
    void Connect();
    void RegFun();
    void PTZ_getAngle(float fH,float fV,bool bMoveFinsh);
	void startThread();
	void InitLidar();
Q_SIGNALS:
	void signalMessageBox(const QString& title, const QString& text);
private slots:
    void PTZMoveFinish(float fHorAngle, float fVecAngle);
    void PTZQueryFinish(float fHorAngle, float fVecAngle);

    void btnRight_clicked();
    void btnLeft_clicked();
    void btnDown_clicked();
    void btnUp_clicked();
    void btnReset_clicked();
    void btnPTZSwitch_clicked();
    void updateDistance(LASER_DATA_T m_LaserData);
    void m_getDistanceBtn_clicked();
    void m_runBtn_clicked();
    void pushButton_2_clicked();
    void pushButton_clicked();
    void timer_TimeOut();


	//lidar相关
	void slotLidarFpsChanged(int nIndex);
	void slotLidarRateChanged(int nIndex);
	void slotLineeditFinished();
	void slotLineeditFinished_2();

	//数据处理
	void slotOriginCorrection();
	void slotDepthCollection();
	void slotDepthCorrection();
	void slotClearDepthData();
	void slotGetLidarYaw();
	void slotGetLidarPitch();
	void slotMessageBox(const QString& title, const QString& text);
	void on_show_cube_checkbox_clicked();

	//平面采集处理
	void slotSurfaceCollection();
	void slotSurfaceCalibration();
	void slotClearSurfaceData();
	void slotLoadLocalData();
	void threadSurfaceCollection();
	void threadSurfaceCalibration();
	void threadClearSurfaceData();
	void btnUpdataColRow_clicked();


	void slotCloseCorllection();
	void slotSelectPixel();
	void slotShowAllPixel();
	void btnDataCollect_clicked();
	void btnUpdataFitCoeff_clicked();
	void btnUpdataScale_XY_clicked();

	void slotSelectLid();
	void slotSetBordCutNum();

	void threadOriginCorrection();
	void threadDepthCollection();
	void threadDepthCorrection();
	void threadClearDepthData();

private:
	void _updateLidarInfo();

private:
    Ui::test *ui;
    Neuvition_Auto* m_pNeuvition_Auto;
    QStandardItemModel* m_SeqListModel;
    QTimer* m_pTimer;

	//lidar相关
	CVTKWidget*    m_pVtkWidget = nullptr;
	CNeuvLidarControl* m_pLidarControl = nullptr;
	bool           m_bConnLidar = false;
	std::vector<int>  m_vec_fps;
	std::map<int, std::string>  m_map_lidarRete;
	float plane_dis = 0;
	float lidarHorAngle = 0;
	float lidarVerAngle = 0;
	static float mTargetDis;
};


#endif // TEST_H
