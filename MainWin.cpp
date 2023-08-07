#include "MainWin.h"
#include "ui_MainWin.h"

#include <QVector>
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QThread>
#include "./Common/LidarModule/globalLidarCallbackFun.h"
#include "./Common/CGlobalSignalSlot.h"

#define DepthNum 7    //2 4 6 8 10 15 20 m总共7个
boost::mutex OriginMutex;
boost::mutex DepthMutex;
boost::mutex XYMutex;
boost::mutex DistanceMutex;//读取距离
boost::condition DistanceCond;//读取距离
boost::condition OriginCond;
boost::condition DepthCond;
boost::condition XYCond;

MainWin* w;
#if MAIN_WIN 
CalibrationProcess *pCalibrationProcess = NULL;
extern bool getFrameDataEnabled;
extern neuvition::NeuvUnits frameDataPoints;
boost::mutex frameDataMutex;
#endif // TEST_OPEN

boost::mutex galDataMutex;//用于显示和处理的全局变量的锁


// 序列化
inline QDataStream &operator<<(QDataStream &o, const neuvition::NeuvUnit& data)
{
	return o << data.x << data.y << data.z << data.col << data.row << data.lid << data.tof;
}
// 反序列化
inline QDataStream &operator>>(QDataStream &o, neuvition::NeuvUnit& data)
{
	return o >> data.x >> data.y >> data.z >> data.col >> data.row >> data.lid >> data.tof;
}


MainWin::MainWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWin)
{
    ui->setupUi(this);
    m_pNeuvition_Auto = new Neuvition_Auto();
    //m_SeqListModel = new QStandardItemModel();
    //this->m_pTimer = new QTimer();
	//m_pTimer->start(100);
    vLayout_VTK = new QVBoxLayout(ui->frame);
    vLayout_VTK->setObjectName(QString::fromUtf8("vLayout_VTK"));
	w = this;
#if MAIN_WIN
    pCalibrationProcess = new CalibrationProcess();
#endif
    InitLidar();
    Connect();
    QDir dir(QCoreApplication::applicationDirPath()+"/Data");
    if(!dir.exists())
    {
        dir.mkpath(QCoreApplication::applicationDirPath()+"/Data");
    }

    QString filenameV = QCoreApplication::applicationDirPath()+"/PTZ/VerticalAngle.txt";
    QString filenameH = QCoreApplication::applicationDirPath()+"/PTZ/LevelAngle.txt";
    QFile fileV(filenameV);
    QFile fileH(filenameH);
    if(fileV.exists() && fileH.exists())
    {
        fileV.open(QIODevice::ReadOnly | QIODevice::Text);
        fileH.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data;
        QStringList data_list;
        if(fileV.size() != 0)//文件有数据
        {
            while(!fileV.atEnd())
            {
                data = fileV.readLine();//读取一行存到data里
                data_list = data.split(",");
                for(int i = 0; i < data_list.size(); i++)
                {
                    this->m_vPTZVAngle.push_back(data_list[i].toFloat());
                }
            }
        }
        if(fileH.size() != 0)//文件有数据
        {
            while(!fileH.atEnd())
            {
                data = fileH.readLine();//读取一行存到data里
                data_list = data.split(",");
                for(int i = 0; i < data_list.size(); i++)
                {
                    this->m_vPTZHAngle.push_back(data_list[i].toFloat());
                }
            }
        }
    }
	this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Switch();
	this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Reset();
	
}

MainWin::~MainWin()
{
    delete ui;ui = nullptr;
	this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Switch();
    delete m_pNeuvition_Auto;m_pNeuvition_Auto = nullptr;
    //delete m_pTimer;m_pTimer = nullptr;
    delete vLayout_VTK;vLayout_VTK = nullptr;
#if MAIN_WIN
    if (pCalibrationProcess) {
            delete pCalibrationProcess;
            pCalibrationProcess = NULL;
        }	
#endif
}

void MainWin::Connect()
{
    connect(ui->m_ChooseFileBtn,SIGNAL(clicked()),this,SLOT(m_ChooseFileBtn_slot()));
    connect(ui->m_StartBtn,SIGNAL(clicked()),this,SLOT(m_StartBtn_slot()));
	connect(ui->m_SaveObjPCL, SIGNAL(clicked()), this, SLOT(m_SaveObjPCL_slot()));
    //connect(this->m_pTimer,SIGNAL(timeout()),this,SLOT(timer_TimeOut()));

    connect(this, SIGNAL(signalpbOriginCorrection()), this, SLOT(slotOriginCorrection()));
    connect(this, SIGNAL(signalpbDepthCorrection()), this, SLOT(slotDepthCorrection()));
	connect(this, SIGNAL(signalpbDepthCollect(float)), this, SLOT(slotDepthCollect(float)));
    connect(this, SIGNAL(signalpbXYPositionCorrection()), this, SLOT(slotXYPosCorrection()));
    connect(this, &MainWin::signalMessageBox, this, &MainWin::slotMessageBox);

	connect(this, &MainWin::signalPTZMove, this, &MainWin::m_PTZMove_slot);
	connect(this, &MainWin::signalLaserInstrument, this, &MainWin::m_LaserInstrument_slot);
	connect(this, &MainWin::signalSliderMove, this, &MainWin::m_SliderMove_slot);


	connect(this, SIGNAL(signalLaserEnd(float)), this, SLOT(slotLaserEnd(float)));
	connect(this, SIGNAL(signalSlideDis(float)), this, SLOT(slotSlideDis(float)));
	connect(this, SIGNAL(signalProgressBar(float)), this, SLOT(slotProgressBar(float)));
	connect(this, SIGNAL(signalAngle(float,float)), this, SLOT(slotAngle(float, float)));
}

#include "./Common/LidarModule/lidar_common_defs.h"
void MainWin::InitLidar()
{

    if (m_pVtkWidget == nullptr)
    {
        m_pVtkWidget = new CVTKWidget;
        vLayout_VTK->addWidget(m_pVtkWidget);
    }

    CGlobalSignalSlot* pGlobalSignal = CGlobalSignalSlot::getInstance();

    //lidar 初始化
    m_pLidarControl = CNeuvLidarControl::getInstance();
    if (!m_pLidarControl->isInitINeuvEvent()) //必须注册Event后才能正常使用
    {
        m_pLidarControl->initINeuvEvent();
    }
    CLidarINeuvEvent* pLidarNeuvEvent = m_pLidarControl->getINeuvEventObject();
    if (pLidarNeuvEvent)
    {
        pLidarNeuvEvent->registerFunOnFrameData(g_cb_on_framedata);
        pLidarNeuvEvent->registerFunOnMjpgData(g_cb_on_mjpgdata);
        pLidarNeuvEvent->registerFunOnImuData(g_cb_on_imudata);
    }
    connect(pGlobalSignal, &CGlobalSignalSlot::sigUpdateRenderCloud, m_pVtkWidget, &CVTKWidget::slotUpdateRenderCloud);
    connect(CLidarConnState::getInstance(), &CLidarConnState::sigLidarConnState, [=](CLidarConnState::enLidarConnState  enConnState) {

        switch (enConnState)
        {
        case CLidarConnState::Lidar_Default:
            break;
        case CLidarConnState::Lidar_DisConnected:
            {
                m_bConnLidar = false;
                QString sText ="not connected";
                ui->label_8->setText(QString::fromLocal8Bit(sText.toStdString().c_str()));
            }
            break;
        case CLidarConnState::Lidar_Connecting_Setup:
            break;
        case CLidarConnState::Lidar_Connecting_Scan:
            break;
        case CLidarConnState::Lidar_Connecting_Stream:
            break;
        case CLidarConnState::Lidar_Connected:
            break;
        default:
            break;
        }

    });

    connect(ui->m_SwitchBtn, &QPushButton::clicked, [=]() {
        if (!m_bConnLidar) //连接lidar
        {
            ReadLidarIni();
            QString sText = "connecting...";
            ui->label_8->setText(QString::fromLocal8Bit(sText.toStdString().c_str()));
            this->update();
            enLidarErrorCode errCode =  m_pLidarControl->startLidar(this->m_sLidarIP.toStdString());
            if (errCode == enLidarErrorCode::LidarError_Normal)
            {
                m_bConnLidar = true;
                sText = "connected";
                ui->label_8->setText(QString::fromLocal8Bit(sText.toStdString().c_str()));
                ui->m_SwitchBtn->setStyleSheet("border-image:url(:/image/res/icon_connect_pressed.png);"
                                               "background-color:rgba(255, 255, 255, 0);");

                //_updateLidarInfo();
            }
            else
            {
                m_bConnLidar = false;
                sText = "not connected";
                ui->label_8->setText(QString::fromLocal8Bit(sText.toStdString().c_str()));
            }
        }
        else //断开lidar
        {
            enLidarErrorCode errCode = m_pLidarControl->stopLidar();
            if (errCode == enLidarErrorCode::LidarError_Normal)
            {
                m_bConnLidar = false;
                QString sText = "not connected";
                ui->label_8->setText(QString::fromLocal8Bit(sText.toStdString().c_str()));
                ui->m_SwitchBtn->setStyleSheet("border-image:url(:/image/res/icon_connect_nor.png);"
                                               "background-color:rgba(255, 255, 255, 0);");
            }
        }

    });

    if(m_pLidarControl)
    {
        m_pLidarControl->setLidarPower(m_sLidarPower.toInt());
        m_pLidarControl->setFrameFrequency(m_sLidarFrame.toInt());
        m_pLidarControl->setLidarInterval(m_sLidarPower.toInt());
    }
}

void MainWin::ReadLidarIni()
{
    QDir dir(QCoreApplication::applicationDirPath()+"/Lidar");
    if(!dir.exists())
    {
        dir.mkpath(QCoreApplication::applicationDirPath()+"/Lidar");
    }
    QString m_IniFileName = QCoreApplication::applicationDirPath()+"/Lidar/Lidar.ini";
    QSettings m_IniSet(m_IniFileName,QSettings::IniFormat);
    QFileInfo fileinfo(m_IniFileName);
    if(fileinfo.size() == 0)
    {
        QFile file(m_IniFileName);
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        file.close();
        m_IniSet.beginGroup("IP");
        m_IniSet.setValue("IP","192.168.1.101");
        m_IniSet.endGroup();
        m_IniSet.beginGroup("Power");
        m_IniSet.setValue("Power","");
        m_IniSet.endGroup();
        m_IniSet.beginGroup("RepetitionRate");
        m_IniSet.setValue("Repetition","");
        m_IniSet.endGroup();
        m_IniSet.beginGroup("FrameRate");
        m_IniSet.setValue("Frame","10");
        m_IniSet.endGroup();
    }
    this->m_sLidarIP = m_IniSet.value("IP/IP").toString();
    this->m_sLidarPower = m_IniSet.value("Power/Power").toString();
    this->m_sLidarRepetition = m_IniSet.value("Repetition/Repetition").toString();
    this->m_sLidarFrame = m_IniSet.value("Frame/Frame").toString();
}
//数据处理/////////////////////////////////////////////////////////////////////

void MainWin::slotMessageBox(const QString& title, const QString& text) {
    QMessageBox::about(this, title, text);
}
void MainWin::threadOriginCorrection() {
#if MAIN_WIN
    neuvition::NeuvUnits points;
    bool IsCalibrationSucceed;
    getFrameDataEnabled = true;
    while (true) {
        boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
        if (getFrameDataEnabled)
            continue;
        {
            boost::mutex::scoped_lock lockit(frameDataMutex);
            points.swap(frameDataPoints);
        }
        break;
    }
    IsCalibrationSucceed = pCalibrationProcess->OriginCorrection(points);
    if (IsCalibrationSucceed) {
        emit this->signalMessageBox("Origin Correction", "Origin Corrected is Success.");
    }
    else {
        emit this->signalMessageBox("Origin Correction", "Origin Corrected is Failed.");
    }
	OriginCond.notify_one();
	qDebug() << "OriginCond.notify_one();";
#endif
}

void MainWin::threadDepthCorrection() {
#if MAIN_WIN
	neuvition::NeuvUnits points;
	bool IsCalibrationSucceed;
	getFrameDataEnabled = true;
	while (true) {
		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
		if (getFrameDataEnabled)
			continue;
		{
			boost::mutex::scoped_lock lockit(frameDataMutex);
			points.swap(frameDataPoints);
		}
		break;
	}
	IsCalibrationSucceed = pCalibrationProcess->DepthCorrection();
	if (IsCalibrationSucceed) {
		emit this->signalMessageBox("Depth Correction", "Depth Corrected is Success.");
	}
	else {
		emit this->signalMessageBox("Depth Correction", "Depth Corrected is Failed.");
	}
	DepthCond.notify_one();
	qDebug() << "DepthCond.notify_one();";
#endif
}

void MainWin::threadDepthCollect(float laserdistance)
{
#if MAIN_WIN
	neuvition::NeuvUnits points;
	bool IsCollectSucceed;
	getFrameDataEnabled = true;
	while (true) {
		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
		if (getFrameDataEnabled)
			continue;
		{
			boost::mutex::scoped_lock lockit(frameDataMutex);
			points.swap(frameDataPoints);
		}
		break;
	}
	IsCollectSucceed = pCalibrationProcess->DepthCollection(points, laserdistance * 1000/1000);
	if (IsCollectSucceed) {
		//emit this->signalMessageBox("Depth Collection", "Depth Collected is Success.");
	}
	else {
		emit this->signalMessageBox("Depth Collection", "Depth Collected is Failed.");
	}
	DepthCond.notify_one();
	qDebug() << "DepthCond.notify_one();";
#endif
}

void MainWin::threadXYPosCorrection() {

	XYCond.notify_one();
}


void MainWin::slotOriginCorrection() {
    boost::thread createThread = boost::thread(boost::bind(&MainWin::threadOriginCorrection, this));
    createThread.detach();
}

void MainWin::slotDepthCorrection() {
    boost::thread createThread = boost::thread(boost::bind(&MainWin::threadDepthCorrection, this));
    createThread.detach();
}

void MainWin::slotDepthCollect(float laserdistance)
{
	boost::thread createThread = boost::thread(boost::bind(&MainWin::threadDepthCollect, this, laserdistance));
	createThread.detach();
}

void MainWin::slotXYPosCorrection() {
    boost::thread createThread = boost::thread(boost::bind(&MainWin::threadXYPosCorrection, this));
    createThread.detach();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWin::ClearData()
{
    //清理数据
    m_pNeuvition_Auto->m_pSlideControl->setiIsOK();//变成0
    m_pNeuvition_Auto->m_pPTZPanel->PTZ_Reset();
    emit this->m_pNeuvition_Auto->m_pSlideControl->m_Sig_SubBecomeZero();//当前PR执行顺序下标置0
    this->m_vLaserDatas.clear();
    this->m_iSliderCurrentNum = 0;
}


void MainWin::startThread()
{
    m_pNeuvition_Auto->m_pSlideControl->startThread();
    //if(this->m_pNeuvition_Auto->m_pSlideControl->getSerialOpen())
    //{
    //    boost::thread createThread = boost::thread(boost::bind(&MainWin::timer_TimeOut, this));
    //    //std::cout << "********************* MainWin::startThread **********" << std::endl;
    //    createThread.detach();
    //}
}

void MainWin::m_ChooseFileBtn_slot()
{
    if(m_bConnLidar == false)
    {
        QMessageBox::information(this,"fail","not connected");
        return;
    }
    m_bSliderFileOpen = m_pNeuvition_Auto->m_pSlideControl->chooseFileRun();
    if(m_bSliderFileOpen)
    {
        this->m_iSliderNum =  m_pNeuvition_Auto->m_pSlideControl->getSeqListNum();//获取数量，用来作进度更新
        QMessageBox::information(this,"success"," File opened successfully ");
    }
    else
    {
        QMessageBox::information(this,"fail"," File opened failed ");
    }
}
//回调函数
bool gla_bStop = false;
void SlideRunReg(bool running)
{
    static int stop_num = 0;
    if(running == false)//是否在移动 但是采用1秒定时器读取，有一定延时,所以需要读取两次
    {
        stop_num++;
		qDebug() << "SlideRunReg  running == false  stopnum = " << stop_num;
        if(stop_num >= 2)//确定停止了
        {
			qDebug() << "stop_num == 2" << stop_num;
            gla_bStop = true;
            //stop_num = 0;
        }
        if(stop_num > 1000)
        {
            stop_num = 0;
        }
    }
    else // run
    {
        qDebug()<<"SlideRunReg  running == true  gla_bStop = false;";
        gla_bStop = false;
        stop_num = 0;
    }
}
float gla_fSliderDistance = 0;
//boost::mutex
void SlideDistance(float distance)//距离 单位mm 0.5秒更新一次
{
    //qDebug()<<"SlideDistance :"<<distance;
    gla_fSliderDistance = distance;
	emit w->signalSlideDis(gla_fSliderDistance);
    //ui->label_19.setText(QString("%1mm").arg(distance));
}

float gla_fHAngle = 0,gla_fVAngle = 0;
int gla_iMoveEnd = 0;
void PTZAngle(float fh,float fv,bool MoveEnd)//水平角度，垂直角度，true（移动完成）/false（实时回传）
{
    gla_fHAngle = fh;
    gla_fVAngle = fv;
	emit w->signalAngle(fh,fv);
	//qDebug() << "PTZAngle gla_fHAngle:" << gla_fHAngle << "  gla_fVAngle:" << gla_fVAngle;
    if(MoveEnd)
    {
        gla_iMoveEnd++;/*移动完成的次数*/
    }
	//boost::thread::sleep(boost::system_time() + boost::posix_time::milliseconds(10));
}

float n018 = -1,n110 = -1,n111 = -1;
void LaserDistance(LASER_DATA_T laserData)
{
    qDebug()<<"LaserDistance:laserData:"<<laserData.id<<laserData.distance;
    //boost::mutex::scoped_lock lockit(galDataMutex);
    if(laserData.id == 18)
    {
        n018 = laserData.distance;
    }
    else if(laserData.id == 110)
    {
        n110 = laserData.distance;
		emit w->signalLaserEnd(laserData.distance);
    }
    else if(laserData.id == 111)
    {
        n111 = laserData.distance;
    }
}

void MainWin::RegFun()//注册回调函数
{
    this->m_pNeuvition_Auto->m_pSlideControl->Slide_RegFun_Runing(SlideRunReg);
    this->m_pNeuvition_Auto->m_pSlideControl->Slide_RegFun_Distance(SlideDistance);
    this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_RegFun(PTZAngle);
	this->m_pNeuvition_Auto->m_pLaserControl->Laser_RegFun(LaserDistance);
}

void MainWin::m_StartBtn_slot()
{
	if (m_bConnLidar == false)
	{
		QMessageBox::information(this, "fail", "雷达未连接");
		return;
	}
	if (!m_bSliderFileOpen)
	{
		QMessageBox::information(this, "fail", "滑轨移动文件未加载");
		return;
	}
    static int clicknum = 0;
    //创建多线程
    if(clicknum == 0)
    {
        clicknum++;
        boost::thread createThread = boost::thread(boost::bind(&MainWin::threadAutoCalibration_, this));
        createThread.detach();
        clicknum = 0;
    }
}

void MainWin::m_PTZMove_slot(int direction, float Angle)
{
	qDebug() << "m_PTZMove_slot direction:" << direction << "  Angle:" << Angle;
	//std::cout << "m_PTZMove_slot :  threadid : " <<QThread::currentThreadId() << std::endl;
	if (0 == direction)
	{
		this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Left(Angle);
	}
	else if (1 == direction)
	{
		this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Right(Angle);
	}
	else if (2 == direction)
	{
		this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Up(Angle);
	}
	else if (3 == direction)
	{
		this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Down(Angle);
	}
}

void MainWin::m_LaserInstrument_slot()
{
	this->m_pNeuvition_Auto->m_pLaserControl->getDistanceByID(110);//这三个激光测距仪一定要18
}

void MainWin::m_SliderMove_slot()
{
	//发送信号让机器执行PR命令，执行位置+1，执行位置超过最后一个会变回0，重头开始
	emit this->m_pNeuvition_Auto->m_pSlideControl->m_Sig_SubPlusPlus();//当前PR执行顺序下标++ 移动
}

void MainWin::m_SaveObjPCL_slot()
{
#if MAIN_WIN 
	neuvition::NeuvUnits points;
	getFrameDataEnabled = true;
	while (true) {
		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
		if (getFrameDataEnabled)
			continue;
		{
			boost::mutex::scoped_lock lockit(frameDataMutex);
			points.swap(frameDataPoints);
		}
		break;
	}
	char fileName[100] = "";

	static int objIndex = 1;
	sprintf(fileName, ".//obj_%d.dat", objIndex);
	objIndex++;
	QFile dataFile(fileName);
	if (dataFile.open(QIODevice::ReadWrite))
	{
		QDataStream stream(&dataFile);
		for (int i = 0; i < points.size(); i++)
		{
			stream << points.at(i);
		}
	}

	dataFile.close();
#endif
}
void MainWin::slotLaserEnd(float distance)
{
	ui->label_20->setText(QString("%1mm").arg(distance));
}
void MainWin::slotSlideDis(float distance)
{
	ui->label_19->setText(QString("%1mm").arg(distance));

}
void MainWin::slotAngle(float H_angle, float V_angle)
{
	ui->label_22->setText(QString::fromLocal8Bit(QString("%1 angle").arg(H_angle).toStdString().c_str()));
	ui->label_23->setText(QString::fromLocal8Bit(QString("%1 angle").arg(V_angle).toStdString().c_str()));
}
void MainWin::slotProgressBar(float num)
{
	//m_iSliderCurrentNum*1.0 / m_iSliderNum * 100
	ui->progressBar->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(m_iSliderCurrentNum*1.0 / m_iSliderNum * 100, 'f', 1)));

}
void MainWin::threadAutoCalibration_()
{
	//初始化
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString filename = QCoreApplication::applicationDirPath() + "/Data/" + current_date_time.toString("yyyy_MM_dd_hhmmss") + ".txt";
	QFile file(filename);
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	emit signalLaserInstrument();
	boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(5));
	emit signalLaserInstrument();
	boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(5));
	emit signalLaserInstrument();
	qDebug() << "m_pNeuvition_Auto->m_pSlideControl->getiIsOK() == 0 gla_bStop:" << gla_bStop;
	LASER_T LaserDistance[10];//单点激光测距的数据
	while (m_pNeuvition_Auto->m_pSlideControl->getiIsOK() == 0)
	{
		if (gla_bStop == true)//滑轨移动停止
		{
			emit signalSliderMove();//滑轨移动
			emit signalLaserInstrument();//获取距离 有可能保留上一次的数据，所以先读一次
			gla_bStop = false;
			boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(2));
			while (gla_bStop == false)//滑轨还在动就循环
			{
				boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
			}
			qDebug() << "m_pNeuvition_Auto   gla_bStop:" << gla_bStop;
			boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(10));
			////判断滑轨的牌子是否停止，没有停下就在循环里
			//float MaxData = -1000000, MinData = 1000000;
			//LASER_T laserData_z;
			//bool isStop = false;
			//int i = 0;
			//while(i < 5)
			//{
			//	emit signalLaserInstrument();//获取距离
			//	boost::mutex::scoped_lock lock(DistanceMutex);
			//	DistanceCond.wait(lock);//等待条件变量DistanceCond解锁
			//	//boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
			//	//记录数据
			//	LaserDistance[i].name018 = n018;
			//	LaserDistance[i].name110 = n110;
			//	LaserDistance[i].name111 = n111;
			//	if (false == isStop)
			//	{
			//		if (LaserDistance[i].name110 < 0 || LaserDistance[i].name110 > 1000000)
			//		{
			//			continue;
			//		}
			//		//最大值最小值
			//		if (LaserDistance[i].name110 > MaxData)
			//		{
			//			MaxData = LaserDistance[i].name110;
			//		}
			//		if (LaserDistance[i].name110 < MinData)
			//		{
			//			MinData = LaserDistance[i].name110;
			//		}
			//		i++;
			//		//判断是否没有剧烈震动
			//		if (i == 4)
			//		{
			//			qDebug() << "MaxData:" << MaxData << " MinData:" << MinData;
			//			//判断数据不可用，重新记录
			//			if (MaxData - MinData > 1)
			//			{
			//				i = 0;
			//			}
			//			else//判断为停止时，重新记录五次数据求平均值
			//			{
			//				i = 0;
			//				isStop = true;
			//			}
			//		}
			//	}
			//	else if (true == isStop)
			//	{
			//		//求和
			//		laserData_z.name018 += LaserDistance[i].name018;
			//		laserData_z.name110 += LaserDistance[i].name110;
			//		laserData_z.name111 += LaserDistance[i].name111;
			//		i++;
			//		if (i == 4)//求均值
			//		{
			//			laserData_z.name018 = laserData_z.name018 / 10;
			//			laserData_z.name110 = laserData_z.name110 / 10;
			//			laserData_z.name111 = laserData_z.name111 / 10;
			//			QString disData = QString("name018:%1,name110:%2,name111:%3;\n").
			//				arg(laserData_z.name018).arg(laserData_z.name110).arg(laserData_z.name111);
			//			qDebug() << "disData:" << disData;
			//		}
			//	}
			//}
			//判断原点标定是否完成
			//if (true == this->m_isOrigin)
			//{
			//	emit signalpbOriginCorrection();
			//	//boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(3));
			//	boost::mutex::scoped_lock lock(OriginMutex);
			//	qDebug()<< "boost::mutex::scoped_lock lock(OriginMutex)";
			//	OriginCond.wait(lock);//等待条件变量OriginCond解锁
			//	qDebug() << "OriginCond.wait(lock)";
			//	m_iSliderCurrentNum++;//用于显示进度
			//	this->m_isOrigin = false;
			//	file.write(QString("H:%1 V:%2 :").arg(gla_fHAngle).arg(gla_fVAngle).toUtf8());
			//	QString disData = QString("018:%1,110:%2,111:%3;\n").arg(n018).arg(n110).arg(n111);
			//	qDebug() << "m_isOrigin disData:" << disData;
			//	file.write(disData.toUtf8());
			//}
			//else if (false == this->m_isOrigin && true == m_isDepth)
			if ( true == m_isDepth)
			{
				qDebug() << "false == this->m_isOrigin && true == m_isDepth";
				if (this->m_collectDepthNum < DepthNum)//2 4 6 8 10 15 20m
				{
					//云台移动完成测距
					emit signalLaserInstrument();
					//boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(500));
					boost::mutex::scoped_lock lock(DistanceMutex);
					DistanceCond.wait(lock);//等待条件变量DistanceCond解锁
					boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(5));
					qDebug() << "/////////////////////////////////////////////////////////////////////////";
					LASER_T laserData;
					//laserData.name018 = n018;
					laserData.name018 = 0;
					laserData.name110 = n110;
					laserData.name111 = 0;
					this->m_vLaserDatas.push_back(laserData);
					QString disData = QString("018:%1,110:%2,111:%3;\n").arg(n018).arg(n110).arg(n111);
					qDebug() << "disData:" << disData;
					file.write(disData.toUtf8());
					//收集z
					emit signalpbDepthCollect(laserData.name110);
					boost::mutex::scoped_lock lock1(DepthMutex);
					qDebug() << "boost::mutex::scoped_lock lock(DepthMutex);";
					DepthCond.wait(lock1);//等待条件变量DepthCond解锁
					m_collectDepthNum++;
					m_iSliderCurrentNum++;//用于显示进度
				}
				if (this->m_collectDepthNum == DepthNum)
				{
					//进行校正
					emit signalpbDepthCorrection();
					m_isDepth = false;
					m_collectDepthNum = 0;
					boost::mutex::scoped_lock lock(DepthMutex);
					DepthCond.wait(lock);//等待条件变量DepthCond解锁
					boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(3));
				}
			}
			//else if ()
			//{

			//}
			emit signalProgressBar(0);
		}
	}
	m_vLaserDatas.clear();
	file.close();
	ClearData();
	this->m_isOrigin = true;
	m_isDepth = true;
}
void MainWin::threadAutoCalibration()
{
// 	初始化
// 	    QDateTime current_date_time =QDateTime::currentDateTime();
// 	    QString filename = QCoreApplication::applicationDirPath()+"/Data/"+current_date_time.toString("yyyy_MM_dd_hhmmss")+".txt";
// 	    QFile file(filename);
// 	    file.open(QIODevice::WriteOnly|QIODevice::Append);
// 		LASER_T LaserDistance[10];//单点激光测距的数据
// 	    while(m_pNeuvition_Auto->m_pSlideControl->getiIsOK() == 0)
// 	    {
// 	        qDebug()<<"m_pNeuvition_Auto->m_pSlideControl->getiIsOK() == 0 gla_bStop:"<<gla_bStop;
// 	        if(gla_bStop == true)//滑轨移动停止
// 	        {
// 				emit signalSliderMove();//滑轨移动
// 				//emit signalLaserInstrument();//获取距离 有可能保留上一次的数据，所以先读一次
// 				gla_bStop = false;
// 				boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
// 	            while(gla_bStop == false)//滑轨还在动就循环
// 	            {
// 	                boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
// 	            }
// 				qDebug() << "m_pNeuvition_Auto   gla_bStop:" << gla_bStop;
// 	            boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(15));
// 	            //判断滑轨的牌子是否停止，没有停下就在循环里
// 				float MaxData = -1000000, MinData = 1000000;
// 				LASER_T laserData_z;
// 				bool isStop = false;
// 				for (int i = 0; i < 5; i++)
// 				{
// 					emit signalLaserInstrument();//获取距离
// 					boost::mutex::scoped_lock lock(DistanceMutex);
// 					DistanceCond.wait(lock);//等待条件变量DistanceCond解锁
// 					//记录数据
// 					LaserDistance[i].name018 = n018;
// 					LaserDistance[i].name110 = n110;
// 					LaserDistance[i].name111 = n111;
// 					boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(5));
// 					if (false == isStop)
// 					{
// 						if (LaserDistance[i].name110<0|| LaserDistance[i].name110 > 1000000)
// 						{
// 							i--;
// 							continue;
// 						}
// 						//最大值最小值
// 						if (LaserDistance[i].name110 > MaxData)
// 						{
// 							MaxData = LaserDistance[i].name110;
// 						}
// 						if (LaserDistance[i].name110 < MinData)
// 						{
// 							MinData = LaserDistance[i].name110;
// 						}
// 						//判断是否没有剧烈震动
// 						if (i == 4)
// 						{
// 							qDebug() << "MaxData:" << MaxData << " MinData:" << MinData;
// 							//判断数据不可用，重新记录
// 							if (MaxData - MinData > 1)
// 							{
// 								i = 0;
// 							}
// 							else//判断为停止时，重新记录五次数据求平均值
// 							{
// 								i = 0;
// 								isStop = true;
// 							}
// 						}
// 					}
// 					else if (true == isStop)
// 					{
// 						//求和
// 						laserData_z.name018 += LaserDistance[i].name018;
// 						laserData_z.name110 += LaserDistance[i].name110;
// 						laserData_z.name111 += LaserDistance[i].name111;
// 						if (i == 9)//求均值
// 						{
// 							laserData_z.name018 = laserData_z.name018 / 10;
// 							laserData_z.name110 = laserData_z.name110 / 10;
// 							laserData_z.name111 = laserData_z.name111 / 10;
// 							QString disData = QString("name018:%1,name110:%2,name111:%3;\n").
// 								arg(laserData_z.name018).arg(laserData_z.name110).arg(laserData_z.name111);
// 							qDebug() << "disData:" << disData;
// 						}
// 					}	
// 				}
// 				////判断原点标定是否完成
// 				//if (true == this->m_isOrigin)
// 				//{
// 				//	emit signalpbOriginCorrection();
// 				//	boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(3));
// 				//	m_iSliderCurrentNum++;//用于显示进度
// 				//	this->m_isOrigin = false;
// 				//	file.write(QString("H:%1 V:%2 :").arg(gla_fHAngle).arg(gla_fVAngle).toUtf8());
// 				//	QString disData = QString("018:%1,110:%2,111:%3;\n").arg(n018).arg(n110).arg(n111);
// 				//	qDebug() << "m_isOrigin disData:" << disData;
// 				//	file.write(disData.toUtf8());
// 				//}
// 				//else if(false == this->m_isOrigin && true == m_isDepth)
// 				if ( true == m_isDepth)
// 				{
// 					qDebug() << "false == this->m_isOrigin && true == m_isDepth";
// 					if (this->m_collectDepthNum < DepthNum)//2 4 6 8 10 15 20m
// 					{
// 						//云台移动完成测距
// 						emit signalLaserInstrument();
// 						//boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
// 						boost::mutex::scoped_lock lock(DistanceMutex);
// 						DistanceCond.wait(lock);//等待条件变量DistanceCond解锁
// 						LASER_T laserData;
// 						laserData.name018 = n018;
// 						laserData.name110 = n110;
// 						laserData.name111 = n111;
// 						this->m_vLaserDatas.push_back(laserData);
// 						QString disData = QString("018:%1,110:%2,111:%3;\n").arg(n018).arg(n110).arg(n111);
// 						qDebug() << "disData:" << disData;
// 						file.write(disData.toUtf8());
// 						//收集z
// 						emit signalpbDepthCollect(laserData.name110);
// 						m_collectDepthNum++;
// 						m_iSliderCurrentNum++;//用于显示进度
// 					}
// 					if(this->m_collectDepthNum == DepthNum)
// 					{
// 						//进行校正
// 						emit signalpbDepthCorrection();
// 						m_isDepth = false;
// 						m_collectDepthNum = 0;
// 						boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(3));
// 					}
// 					
// 				}
// 				//else if (false == this->m_isOrigin && false == m_isDepth && true == m_isXY)
// 				//{
// 				//	for (int i = 0; i < m_vPTZVAngle.size() + m_vPTZHAngle.size(); )//云台移动次数，i在循环里加加
// 				//	{
// 				//		file.write(QString("H:%1 V:%2 :").arg(gla_fHAngle).arg(gla_fVAngle).toUtf8());
// 				//		qDebug() << "QString : " << QString("H:%1 V:%2 :").arg(m_vPTZHAngle[i]).arg(m_vPTZVAngle[i]);
// 				//		if (i < m_vPTZHAngle.size())//水平移动先
// 				//		{
// 				//			//云台移动
// 				//			float current = gla_fHAngle;
// 				//			if (current > 180)current -= 360;
// 				//			float moveAngle = m_vPTZHAngle[i] - current;
// 				//			qDebug() << "shuiping  moveAngle = m_vPTZHAngle[i] - current = " << moveAngle;
// 				//			if (moveAngle < 0.5 && moveAngle > -0.5)
// 				//			{
// 				//				gla_iMoveEnd++;
// 				//			}
// 				//			else if (moveAngle > 0)//右转
// 				//			{
// 				//				//this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Right(moveAngle);
// 				//				emit signalPTZMove(1, moveAngle);
// 				//			}
// 				//			else if (moveAngle < 0)//左转
// 				//			{
// 				//				//this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Left(-moveAngle);
// 				//				emit signalPTZMove(0, -moveAngle);
// 				//			}
// 				//		}
// 				//		else if (i >= m_vPTZHAngle.size())//垂直移动
// 				//		{
// 				//			int j = i - m_vPTZHAngle.size();
// 				//			//云台移动
// 				//			float current = gla_fVAngle;
// 				//			float moveAngle = m_vPTZVAngle[j] - current;
// 				//			qDebug() << "chuizhi  moveAngle = m_vPTZVAngle[j] - current" << moveAngle;
// 				//			if (moveAngle < 0.5 && moveAngle > -0.5)
// 				//			{
// 				//				gla_iMoveEnd++;
// 				//			}
// 				//			else if (moveAngle > 0)//上
// 				//			{
// 				//				emit signalPTZMove(2, moveAngle);
// 				//			}
// 				//			else if (moveAngle < 0)//下
// 				//			{
// 				//				emit signalPTZMove(3, -moveAngle);
// 				//			}
// 				//		}
// 	
// 				//		i++;
// 				//		qDebug() << "gla_iMoveEnd(1):" << gla_iMoveEnd << "  i:" << i;
// 				//		boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(20));
// 				//		while (gla_iMoveEnd != i)//移动完成gla_iMoveEnd应该等于i
// 				//		{
// 				//			boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(10));
// 				//			//qDebug() << "while(gla_iMoveEnd != i)";
// 				//		}
// 				//		qDebug() << "gla_iMoveEnd(2):" << gla_iMoveEnd << "  i:" << i;
// 				//		//云台移动完成测距
// 				//		emit signalLaserInstrument();
// 				//		boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
// 				//		LASER_T laserData;
// 				//		laserData.name018 = n018;
// 				//		laserData.name110 = n110;
// 				//		laserData.name111 = n111;
// 				//		this->m_vLaserDatas.push_back(laserData);
// 				//		QString disData = QString("018:%1,110:%2,111:%3;\n").arg(n018).arg(n110).arg(n111);
// 				//		qDebug() << "disData:" << disData;
// 				//		file.write(disData.toUtf8());
// 				//	}
// 				//	gla_iMoveEnd = 0;
// 				//	m_iSliderCurrentNum++;//用于显示进度
// 				//}
// 			}
// 	        boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
// 	        qDebug()<<"threadAutoCalibration boost::get_system_time() + boost::posix_time::milliseconds(50)";
// 	    }
// 		m_vLaserDatas.clear();
// 	    file.close();
// 	    ClearData();
// 		this->m_isOrigin = true;
// 		m_isDepth = true;
}

//void MainWin::threadAutoCalibration_()
//{
//	emit signalpbOriginCorrection();
//	boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(3));
//}

void MainWin::timer_TimeOut()
{
 //   while(1)
 //   {
 //       ui->label_19->setText(QString("%1mm").arg(gla_fSliderDistance));
 //       ui->label_22->setText(QString::fromLocal8Bit(QString("%1 angle").arg(gla_fHAngle).toStdString().c_str()));
	//	ui->label_23->setText(QString::fromLocal8Bit(QString("%1 angle").arg(gla_fVAngle).toStdString().c_str()));
 //       ui->label_20->setText(QString("%1mm").arg(n110));
	//	ui->progressBar->setStyleSheet("QProgressBar{font:9pt;border-radius:5px;text-align:center;border:1px solid #E8EDF2;\
	//	background-color: rgb(255, 255, 255);border-color: rgb(180, 180, 180);}\
	//	QProgressBar:chunk{border-radius:5px;background-color:#1ABC9C;}");
 //       ui->progressBar->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(m_iSliderCurrentNum*1.0/m_iSliderNum*100,'f', 1)));
	//	
	//	boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(50));
	//}
}