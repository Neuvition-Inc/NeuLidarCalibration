#include "test.h"
#include "ui_test.h"
#include <QVector>

#include "./Common/LidarModule/globalLidarCallbackFun.h"
#include "./Common/CGlobalSignalSlot.h"
#include "QTextStream"
#include "QFileDialog"


typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;
//#define M_PI       3.14159265358979323846   // pi

#if TEST_OPEN
CalibrationProcess *pCalibrationProcess = NULL;
extern bool getFrameDataEnabled;
extern neuvition::NeuvUnits frameDataPoints;
boost::mutex frameDataMutex;
#endif

const double lightspeed = 299552816.0;
const double milliunit = 1000.0;
const double scaleunit = 0.001;
const double dcoef = lightspeed / milliunit / milliunit / scaleunit / milliunit / milliunit / 2.0;


extern bool select_pixel_enabled;
extern bool data_collect_enabled;
extern bool showValidCube;

extern int minRow;
extern int maxRow;
extern int minCol;
extern int maxCol;
extern int lid;
extern std::vector<neuvition::NeuvUnit> selectPixelVec;



float test::mTargetDis = 0;
test::test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    m_pNeuvition_Auto = new Neuvition_Auto();
    m_SeqListModel = new QStandardItemModel();
#if TEST_OPEN
	pCalibrationProcess = new CalibrationProcess();
#endif
    ui->tableView->setModel(m_SeqListModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_SeqListModel->setColumnCount(7);
    m_SeqListModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("PR编号"));
    m_SeqListModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("CMD"));
    m_SeqListModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("ACC(ms)"));
    m_SeqListModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("DEC(ms)"));
    m_SeqListModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("SPD(rpm)"));
    m_SeqListModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("DLY(ms)"));
    m_SeqListModel->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("PUU"));
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView->verticalHeader()->setVisible(false);  //隐藏列表头
    ui->tableView->setColumnWidth(0,120);
    ui->tableView->setColumnWidth(1,120);
    ui->tableView->setColumnWidth(2,120);
    ui->tableView->setColumnWidth(3,120);
    ui->tableView->setColumnWidth(4,120);
    ui->tableView->setColumnWidth(5,120);
    ui->tableView->setColumnWidth(6,120);

    this->m_pTimer = new QTimer();
    if(this->m_pNeuvition_Auto->m_pSlideControl->getSerialOpen())
    {
        m_pTimer->start(1000);
    }
    Connect();
	InitLidar();
}

test::~test()
{
    delete ui;
    delete m_pNeuvition_Auto;
#if TEST_OPEN
	if (pCalibrationProcess) {
		delete pCalibrationProcess;
		pCalibrationProcess = NULL;
	}
#endif
}

void test::Connect()
{
    //滑轨
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pushButton_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_2_clicked()));
    connect(ui->m_runBtn,SIGNAL(clicked()),this,SLOT(m_runBtn_clicked()));
    connect(ui->m_getDistanceBtn,SIGNAL(clicked()),this,SLOT(m_getDistanceBtn_clicked()));
    //测距仪
    connect(m_pNeuvition_Auto->m_pLaserControl,&LaserInstrumentControl::m_sigUpdatedLaserData,
            this,&test::updateDistance);
    //云台
    connect(ui->btnPTZSwitch,SIGNAL(clicked()),this,SLOT(btnPTZSwitch_clicked()));
    connect(ui->btnReset,SIGNAL(clicked()),this,SLOT(btnReset_clicked()));
    connect(ui->btnUp,SIGNAL(clicked()),this,SLOT(btnUp_clicked()));
    connect(ui->btnDown,SIGNAL(clicked()),this,SLOT(btnDown_clicked()));
    connect(ui->btnLeft,SIGNAL(clicked()),this,SLOT(btnLeft_clicked()));
    connect(ui->btnRight,SIGNAL(clicked()),this,SLOT(btnRight_clicked()));
    connect(this->m_pNeuvition_Auto->m_pPTZPanel,&CPTZBasePanel::sigMoveFinish,
            this,&test::PTZMoveFinish);
    connect(this->m_pNeuvition_Auto->m_pPTZPanel,&CPTZBasePanel::sigQueryFinish,
            this,&test::PTZQueryFinish);
    //timer
    connect(this->m_pTimer,SIGNAL(timeout()),this,SLOT(timer_TimeOut()));

	//数据处理
	connect(ui->pbOriginCorrection, SIGNAL(clicked()), this, SLOT(slotOriginCorrection()));
	connect(ui->pbDepthCollection, SIGNAL(clicked()), this, SLOT(slotDepthCollection()));
	connect(ui->pbDepthCorrection, SIGNAL(clicked()), this, SLOT(slotDepthCorrection()));
	connect(ui->pbClearDepthData, SIGNAL(clicked()), this, SLOT(slotClearDepthData()));
	connect(ui->pbLidarYaw, SIGNAL(clicked()), this, SLOT(slotGetLidarYaw()));
	connect(ui->pbLidarPitch, SIGNAL(clicked()), this, SLOT(slotGetLidarPitch()));
	connect(this, &test::signalMessageBox, this, &test::slotMessageBox);
	//lidar相关的
	//connect(ui->)

	//平面采集矫正相关
	connect(ui->btn_surfaceCollection, SIGNAL(clicked()), this, SLOT(slotSurfaceCollection()));
	connect(ui->btn_surfaceCorrect, SIGNAL(clicked()), this, SLOT(slotSurfaceCalibration()));
	connect(ui->btn_clearSurface, SIGNAL(clicked()), this, SLOT(slotClearSurfaceData()));
	connect(ui->checkBox_showCube, SIGNAL(clicked()), this, SLOT(on_show_cube_checkbox_clicked()));
	connect(ui->btn_LoadLocalData, SIGNAL(clicked()), this, SLOT(slotLoadLocalData()));
	
	connect(ui->btnUpdataColRow, SIGNAL(clicked()), this, SLOT(btnUpdataColRow_clicked()));


	//	
	connect(ui->btn_closeCor, SIGNAL(clicked()), this, SLOT(slotCloseCorllection()));
	connect(ui->btn_selectPixel, SIGNAL(clicked()), this, SLOT(slotSelectPixel()));
	connect(ui->btn_showAllPixel, SIGNAL(clicked()), this, SLOT(slotShowAllPixel()));
	connect(ui->btn_DataCollect, SIGNAL(clicked()), this, SLOT(btnDataCollect_clicked()));
	connect(ui->btn_updataFitCoeff, SIGNAL(clicked()), this, SLOT(btnUpdataFitCoeff_clicked()));


	connect(ui->lineEditLid, SIGNAL(returnPressed()), this, SLOT(slotSelectLid()));
	connect(ui->btn_updataXY, SIGNAL(clicked()), this, SLOT(btnUpdataScale_XY_clicked()));
	connect(ui->lineEdit_bordCut, SIGNAL(returnPressed()), this, SLOT(slotSetBordCutNum())); 
	

}

//数据处理/////////////////////////////////////////////////////////////////////
void test::slotMessageBox(const QString& title, const QString& text) {
	QMessageBox::about(this, title, text);
}

void test::on_show_cube_checkbox_clicked()
{
	showValidCube = !showValidCube;
}


void test::slotSurfaceCollection()
{
	//mTargetDis = this->m_pNeuvition_Auto->m_pSlideControl->readLocationInfo() * 0.001;
	mTargetDis = ui->lineEditLaserDepth->text().toFloat();
	boost::thread createThread = boost::thread(boost::bind(&test::threadSurfaceCollection, this));
	createThread.detach();
}

void test::slotSurfaceCalibration()
{

	boost::thread createThread = boost::thread(boost::bind(&test::threadSurfaceCalibration, this));
	createThread.detach();
}

void test::slotClearSurfaceData()
{
	boost::thread createThread = boost::thread(boost::bind(&test::threadClearSurfaceData, this));
	createThread.detach();
}

void test::slotLoadLocalData()
{
	QString localDataPath = QApplication::applicationDirPath();
	localDataPath.append("/data_collect/");
	QString path = QFileDialog::getOpenFileName(this, QString("Open File"),localDataPath,QString("ALL(*.txt)"));
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream dataStream(&file);
	int dis = 0;
	neuvition::NeuvUnits oneFrameCloud;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);

	std::ofstream mBofs;
	QString dtfilepath = "";
	QString filePath = QApplication::applicationDirPath().append("/Barry");
	QDir dir;

	if (!dir.exists(filePath)) {
		dir.mkdir(filePath);

	}
	QDateTime current = QDateTime::currentDateTime();
	QString time = current.toString("yyyyMMdd");
	dtfilepath.append(filePath).append("/").append(time).append(".txt");
	mBofs.open(dtfilepath.toStdString(), ios::out | ios::app);



	int realZ = 0;
	while (!dataStream.atEnd()) {
		QString line = dataStream.readLine();
		QStringList list = line.split(" ");

		if (list.size() == 1)
		{
			continue;
		}
		neuvition::NeuvUnit poi;
		poi.lid = list.at(0).toInt();
		poi.row = list.at(1).toInt();
		poi.col = list.at(2).toInt();
		poi.intensity = (uint8_t)list.at(3).toInt();
		poi.tof = list.at(4).toInt();
		poi.time_sec = list.at(5).toInt();
		pCalibrationProcess->AppendSurfaceCorPoint(poi);
		mBofs << list.at(1).toInt() << " " << list.at(2).toInt() << " " << list.at(3).toInt() << " " << list.at(4).toInt() << " " << list.at(5).toInt() << "\n";
	}
	file.close();

	mBofs.close();
}

void test::threadSurfaceCollection()
{
#if TEST_OPEN
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

	
	if (mTargetDis < 1)
	{
		mTargetDis = 10;
	}
	IsCollectSucceed = pCalibrationProcess->SurfaceCollection(points, mTargetDis,lidarHorAngle,lidarVerAngle);
	if (IsCollectSucceed == 1) {
		ui->label_surfaceCollectStastus->setText("Surface Collected is Success!!!!!");
		//emit this->signalMessageBox("Surface Collection", "Surface Collected is Success.");
	}
	else {
		ui->label_surfaceCollectStastus->setText("Failed!!!!!");
		//emit this->signalMessageBox("Surface Collection", "Surface Collected is Failed.");
	}
#endif
}

void test::threadSurfaceCalibration()
{
	bool IsCalibrationSucceed = pCalibrationProcess->SurfaceCalibration();
	if (IsCalibrationSucceed) {
		emit this->signalMessageBox("Depth Correction", "Depth Corrected is Success.");
	}
	else {
		emit this->signalMessageBox("Depth Correction", "Depth Corrected is Failed.");
	}
}

void test::threadClearSurfaceData()
{
	pCalibrationProcess->ClearSurfaceData();
}

void test::btnUpdataColRow_clicked()
{
	int col = ui->lineEdit_originCol->text().toInt();
	int row = ui->lineEdit_originRow->text().toInt();
	pCalibrationProcess->SetOriginColRow(col, row);
}

void test::slotCloseCorllection()
{
	static bool enable = false;
	int nDeviceType = neuvition::get_device_type();
	if (enable)
	{
		neuvition::set_c_absol_dis_cor_enabled(true);
		neuvition::set_c_tof_cor_by_pw(true);
		neuvition::set_c_surface_correct_enabled(true);
		neuvition::set_c_remove_brushed_enabled(true);
		neuvition::set_c_interp_by_tof_enabled(true);
		neuvition::set_c_lens_cor_enabled(true);
		//neuvition::set_c_pos_cor_enabled(true);
		neuvition::set_c_reflectivity_cor_enabled(true);
		neuvition::set_c_move_average_enabled(false);
		neuvition::set_c_p1sl_frank_cor_enabled(true);
		enable = false;
		ui->btn_closeCor->setText("Close Cor");
	}
	else
	{
		neuvition::set_c_absol_dis_cor_enabled(false);
		neuvition::set_c_tof_cor_by_pw(false);
		neuvition::set_c_surface_correct_enabled(false);
		neuvition::set_c_remove_brushed_enabled(false);
		neuvition::set_c_interp_by_tof_enabled(false);
		neuvition::set_c_lens_cor_enabled(false);
		//neuvition::set_c_pos_cor_enabled(false);
		neuvition::set_c_reflectivity_cor_enabled(false);
		//neuvition::set_c_move_average_enabled(true);
		neuvition::set_c_p1sl_frank_cor_enabled(false);

// 		if (nDeviceType == neuvition::TITAN_P1_SL1550 || nDeviceType == neuvition::TITAN_P1_ML905 || nDeviceType == neuvition::TITAN_P1_SL)
// 		{
// 			neuvition::set_c_move_average_enabled(false);
// 		}
// 		else
// 		{
// 			neuvition::set_c_move_average_enabled(true);
// 		}
		enable = true;
		ui->btn_closeCor->setText("Open Cor");
	}

}

void test::slotSelectPixel()
{
	minRow = ui->lineEditStartRow->text().toInt();
	maxRow = ui->lineEditEndRow->text().toInt();
	minCol = ui->lineEditStartCol->text().toInt();
	maxCol = ui->lineEditEndCol->text().toInt();
	select_pixel_enabled = true;

}
extern bool onlyShowSelectPicel;
void test::slotShowAllPixel()
{

// 	minRow = 0;
// 	maxRow = 719;
// 	minCol = 0;
// 	maxCol = 4096;
// 	select_pixel_enabled = false;
	onlyShowSelectPicel = !onlyShowSelectPicel;
}

void test::btnDataCollect_clicked()
{
#if TEST_OPEN
	neuvition::NeuvUnits points;
	std::ofstream ofs;
	std::ofstream ofs2;
	QString filePath = QApplication::applicationDirPath();
	filePath.append("/data_collect/");
	filePath.append(pCalibrationProcess->GetSerialNumber()).append("_").append(pCalibrationProcess->GetDeviceName());
	QDir dir;
	if (!dir.exists(filePath)) {
		dir.mkdir(filePath);
	}
	QDateTime current = QDateTime::currentDateTime();
	QString singleFilePath = filePath;
	QString power = 0;
	if (lid == 0 || lid  ==2)
	{
		power = ui->lePower->text();
	}
	else
	{
		power = ui->lePower_2->text();
	}
	singleFilePath.append("/").append(current.toString("yyyyMMdd")).append("_").append(ui->lineEdit_realDis->text()).append("_")
		.append(ui->lePower->text()).append("_").append(QString::number(lid)).append(".txt");
	filePath.append("/").append(current.toString("yyyyMMdd")).append(".txt");
	ofs.open(filePath.toStdString(), ios::out | ios::app);
	ofs2.open(singleFilePath.toStdString(), ios::out | ios::app);

	selectPixelVec.clear();
	int realdis = ui->lineEdit_realDis->text().toInt();
	ofs << "-------------------" << realdis << "-------------------" << power.toStdString() << "---" << lid << std::endl;
	data_collect_enabled = true;
	while (true) {
		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
		if (data_collect_enabled)
			continue;
		{
			boost::mutex::scoped_lock lockit(frameDataMutex);
			points.swap(selectPixelVec);
		}
		for (int i = 0; i < points.size(); i++)
		{
			neuvition::NeuvUnit unit = points[i];
			if (lid != 0 &&  unit.lid != lid)
			{
				continue;
			}
			int simtof;
			if (neuvition::get_device_type() == neuvition::TITAN_M1_A_SL)
			{
				unit.row = 360;
			}
			if (neuvition::get_device_type() == neuvition::TITAN_P1_SL || neuvition::get_device_type() == neuvition::TITAN_P1_SL1550) {
				double mt0, mt1;
				neuvition::get_matrix_data_for_P1(unit.lid, unit.col, 0, 2, &mt0, &mt1);
				simtof = int((realdis - mt1) / mt0 / dcoef);
			}
			else {
				simtof = int(realdis / neuvition::get_matrix_data(unit.lid, unit.col, unit.row, 2) / dcoef);
			}
			if (std::abs(unit.z - realdis) > 5000)
			{
				continue;	
			}
			ofs << (int)unit.lid << " " << unit.row << " " << unit.col << " " << (int)unit.intensity << " " << unit.tof << " " << simtof  << " " << unit.z << std::endl;
			ofs2 << (int)unit.lid << " " << unit.row << " " << unit.col << " " << (int)unit.intensity << " " << unit.tof << " " << simtof << " " << unit.z << std::endl;
		}
		ofs.close();
		ofs2.close();
		break;
	}

#endif
}

void test::btnUpdataFitCoeff_clicked()
{
	pCalibrationProcess->updata();
}

void test::btnUpdataScale_XY_clicked()
{
	float scaleX = ui->lineEdit_scaleX->text().toFloat();
	float scaleY = ui->lineEdit_scaleY->text().toFloat();
	pCalibrationProcess->updataScaleXY(scaleX, scaleY);
}

void test::slotSelectLid()
{
	lid = ui->lineEditLid->text().toInt();
}

void test::slotSetBordCutNum()
{
	int num = ui->lineEdit_bordCut->text().toInt();
	pCalibrationProcess->SetBordCutNum(num);
}

void test::threadOriginCorrection() {
#if TEST_OPEN
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
#endif
}

void test::threadDepthCollection() {
#if TEST_OPEN
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
	int laserDepth = ui->lineEditLaserDepth->text().toInt();
	IsCollectSucceed = pCalibrationProcess->DepthCollection(points, laserDepth);
	if (IsCollectSucceed) {
		emit this->signalMessageBox("Depth Collection", "Depth Collected is Success.");
	}
	else {
		emit this->signalMessageBox("Depth Collection", "Depth Collected is Failed.");
	}
#endif
}

void test::threadDepthCorrection() {
#if TEST_OPEN
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
#endif
}

void test::threadClearDepthData() {
#if TEST_OPEN
	pCalibrationProcess->ClearDepthData();
	emit this->signalMessageBox("Depth Clear", "Depth Clear is Success.");
#endif
}

void test::slotOriginCorrection() {
	boost::thread createThread = boost::thread(boost::bind(&test::threadOriginCorrection, this));
	createThread.detach();
}

void test::slotDepthCollection() {
	boost::thread createThread = boost::thread(boost::bind(&test::threadDepthCollection, this));
	createThread.detach();
}

void test::slotDepthCorrection() {
	boost::thread createThread = boost::thread(boost::bind(&test::threadDepthCorrection, this));
	createThread.detach();
}

void test::slotClearDepthData() {
	boost::thread createThread = boost::thread(boost::bind(&test::threadClearDepthData, this));
	createThread.detach();
}

void test::slotGetLidarYaw() {
	int ptzYaw = ui->lineEditPtzYaw->text().toInt();
	int laserDepth = ui->lineEditLaserDepth->text().toInt();
// 	float lidarYaw = pCalibrationProcess->getLidarYawAngle(laserDepth * 0.001, ptzYaw * 1.0);
// 	std::cout << "laserDepth = " << laserDepth << " ptzYaw= " << ptzYaw << " lidarYaw= " << lidarYaw << std::endl;
}

void test::slotGetLidarPitch() {
	int ptzPitch = ui->lineEditPtzPitch->text().toInt();
	int laserDepth = ui->lineEditLaserDepth->text().toInt();
// 	float lidarPitch = pCalibrationProcess->getLidarPitchAngle(laserDepth * 0.001, ptzPitch * 1.0);
// 	std::cout << "laserDepth = " << laserDepth << " ptzPitch= " << ptzPitch << " lidarPitch= " << lidarPitch << std::endl;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


void test::PTZ_getAngle(float fH, float fV, bool bMoveFinsh)
{
    ui->lbCurHorAngle->setText(QString::number(fH));
    ui->lbCurVecAngle->setText(QString::number(fV));
}


void test::startThread()
{
	m_pNeuvition_Auto->m_pSlideControl->startThread();
}

#include "./Common/LidarModule/lidar_common_defs.h"
void test::InitLidar()
{
	if (m_pVtkWidget == nullptr)
	{
		m_pVtkWidget = new CVTKWidget;
		ui->vLayout_VTK->addWidget(m_pVtkWidget);
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
				QString sText = QString::fromLocal8Bit("未连接");
				ui->pbConnLidar->setText(sText);
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

	connect(ui->pbConnLidar, &QPushButton::clicked, [=]() {
		
		if (!m_bConnLidar) //连接lidar
		{
			QString sText = QString::fromLocal8Bit("正在连接...");
			ui->pbConnLidar->setText(sText);

			this->update();

			QString sIp = ui->leIp->text();
			enLidarErrorCode errCode =  m_pLidarControl->startLidar(sIp.toStdString());
			if (errCode == enLidarErrorCode::LidarError_Normal)
			{
				m_bConnLidar = true;
				//sText = QString::fromLocal8Bit("已连接");
				sText = QString("disconnect");
				ui->pbConnLidar->setText(sText);

				_updateLidarInfo();
			}
			else 
			{
				m_bConnLidar = false;
				//sText = QString::fromLocal8Bit("未连接");
				sText = QString("failed,Reconnect");
				ui->pbConnLidar->setText(sText);
			}
			


		}
		else //断开lidar 
		{
			enLidarErrorCode errCode = m_pLidarControl->stopLidar();
			if (errCode == enLidarErrorCode::LidarError_Normal)
			{
				m_bConnLidar = false;
				QString sText = QString::fromLocal8Bit("未连接");
				ui->pbConnLidar->setText(sText);
			}
			
		}

	});


	ui->cbLidarFps->clear();
	ui->cbLidarRate->clear();

	connect(ui->cbLidarFps, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarFpsChanged(int)));
	connect(ui->cbLidarRate, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarRateChanged(int)));
	connect(ui->lePower, &QLineEdit::editingFinished, this, &test::slotLineeditFinished);
	connect(ui->lePower_2, &QLineEdit::editingFinished, this, &test::slotLineeditFinished_2);
}

void test::RegFun()
{
//    pPTZfun fun = (pPTZfun)&test::PTZ_getAngle;
//    this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_RegFun(fun);
}





void test::pushButton_clicked()
{
    bool bRet = m_pNeuvition_Auto->m_pSlideControl->chooseFileRun();
    if(bRet)
    {
        ui->lineEdit->setText(m_pNeuvition_Auto->m_pSlideControl->getFileName());
    }
}

void test::timer_TimeOut()
{
    bool bRet = this->m_pNeuvition_Auto->m_pSlideControl->getISRuning();
    if(bRet)
    {
        ui->label_3->setText("Run");
    }
    else
    {
        ui->label_3->setText("Stop");
        ui->m_runBtn->setEnabled(true);
    }
    float distance = this->m_pNeuvition_Auto->m_pSlideControl->readLocationInfo();
    //qDebug()<<"timer_TimeOut:distance"<<distance;
    ui->label->setText(QString("%1mm").arg(distance));
}

void test::slotLidarFpsChanged(int nIndex)
{
	 int nFps = m_vec_fps[nIndex];
	 if (m_pLidarControl)
	 {
		 m_pLidarControl->setFrameFrequency(nFps);
	 }

}

void test::slotLidarRateChanged(int nIndex)
{
	if (m_pLidarControl && nIndex >= 0)
	{
		int nLidarRate = m_pLidarControl->transLidarInterval(m_map_lidarRete[nIndex]);
		m_pLidarControl->setLidarInterval(nLidarRate);
	}
	else if(nIndex == -1){
		std::cout << "n == - 1" << std::endl;
	}
}

void test::slotLineeditFinished()
{
	QLineEdit* pLineEdit = static_cast<QLineEdit*>(sender());
	if (pLineEdit == ui->lePower)
	{
		bool bRet = false;
		int nPower = pLineEdit->text().toInt(&bRet);
		if (bRet)
		{
			m_pLidarControl->setLidarPower(nPower);
		}
		
	}
}
void test::slotLineeditFinished_2()
{
	QLineEdit* pLineEdit = static_cast<QLineEdit*>(sender());
	if (pLineEdit == ui->lePower_2)
	{
		bool bRet = false;
		int nPower = pLineEdit->text().toInt(&bRet);
		if (bRet)
		{
			neuvition::set_debug_temp_data(neuvition::NEUV_CMD_TEMP8, nPower);
			//m_pLidarControl->(nPower);
		}

	}
}
void test::_updateLidarInfo()
{
	//CNeuvLidarControl*  pLidarControl = CNeuvLidarControl::getInstance();
	
	//功率
	int nPower = m_pLidarControl->getLidarPower();
	ui->lePower->setText(QString::number(nPower));


	std::vector<int>  vec_fps = m_pLidarControl->getVecFps();
	vec_fps.swap(m_vec_fps);
	std::map<int, std::string> map_lidarRete = m_pLidarControl->getMapLidarInterval();

	std::cout << "map_lidarRete  size = " << map_lidarRete.size() << std::endl;
	std::cout << "m_map_lidarRete swap begin size = " << m_map_lidarRete.size() << std::endl;
	map_lidarRete.swap(m_map_lidarRete);
	std::cout << "m_map_lidarRete swap end size = " << m_map_lidarRete.size() << std::endl;

	disconnect(ui->cbLidarFps, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarFpsChanged(int)));
	disconnect(ui->cbLidarRate, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarRateChanged(int)));

	ui->cbLidarFps->clear();
	ui->cbLidarRate->clear();

	int nFps = m_pLidarControl->getFrameFrequency();
	int nIndexFps = -1;
	if (!m_vec_fps.empty())
	{
		for (int i = 0; i < m_vec_fps.size(); i++)
		{
			ui->cbLidarFps->addItem(QString::number(m_vec_fps[i]));
			if (nFps == m_vec_fps[i])
			{
				nIndexFps = i;
			}
		}
	}
	if (!m_map_lidarRete.empty())
	{
		std::cout << "m_map_lidarRete swap 2222 end size = " << m_map_lidarRete.size() << std::endl;
		int nSize = m_map_lidarRete.size();
		std::cout << "m_map_lidarRete swap 333 end size = " << nSize << std::endl;
		for (int i = 0; i < nSize; i++)
		{
			ui->cbLidarRate->addItem(QString::fromStdString(m_map_lidarRete[i]));
		}
	}

	connect(ui->cbLidarFps, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarFpsChanged(int)));
	connect(ui->cbLidarRate, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLidarRateChanged(int)));
	//
	int nLidarRate = m_pLidarControl->getLidarInterval();
	if (nLidarRate >= 0)
	{
		ui->cbLidarRate->setCurrentIndex(nLidarRate);
	}
	else
	{
		std::cout << "failed  nLidarRate = " << nLidarRate << std::endl;
	}
	


	ui->cbLidarFps->setCurrentIndex(nIndexFps);



}

void test::pushButton_2_clicked()
{
    this->m_SeqListModel->removeRows(0,this->m_SeqListModel->rowCount());
    QVector<PRSET_T> QPRSet = this->m_pNeuvition_Auto->m_pSlideControl->getPRSet();
    for (int i = 0 ;i < this->m_pNeuvition_Auto->m_pSlideControl->getSeqListNum();i++)
    {
       PRSET_T  PRSet = QPRSet.at(i);
       this->m_SeqListModel->setItem(i,0,new QStandardItem(QString("%1").arg(PRSet.PRNumber)));
       this->m_SeqListModel->setItem(i,1,new QStandardItem(QString("%1").arg(PRSet.PRCMD)));
       this->m_SeqListModel->setItem(i,2,new QStandardItem(QString("%1").arg(PRSet.PRACC)));
       this->m_SeqListModel->setItem(i,3,new QStandardItem(QString("%1").arg(PRSet.PRDEC)));
       this->m_SeqListModel->setItem(i,4,new QStandardItem(QString("%1").arg(PRSet.PRSPD)));
       this->m_SeqListModel->setItem(i,5,new QStandardItem(QString("%1").arg(PRSet.PRDLY)));
       this->m_SeqListModel->setItem(i,6,new QStandardItem(QString("%1").arg(PRSet.PRPUU)));

    }
}

void test::m_runBtn_clicked()
{
    ui->m_runBtn->setEnabled(false);
    emit this->m_pNeuvition_Auto->m_pSlideControl->m_Sig_SubPlusPlus();
//    this->m_pNeuvition_Auto->m_pSlideControl->runPRSet();
    for(int i = 0 ;i < this->m_pNeuvition_Auto->m_pSlideControl->getCurrentSeqPos();i++)
    {
        for (int j = 0;j < 7; j++)
        {
            this->m_SeqListModel->item(i,j)->setBackground(QColor(Qt::green));
        }
    }
    for(int i = this->m_pNeuvition_Auto->m_pSlideControl->getCurrentSeqPos() ;i < this->m_pNeuvition_Auto->m_pSlideControl->getSeqListNum();i++)
    {
        for (int j = 0;j < 7; j++)
        {
            this->m_SeqListModel->item(i,j)->setBackground(QColor(Qt::white));
        }
    }
}

bool m_getDistanceBtn_flag = false;
void test::m_getDistanceBtn_clicked()
{
    qDebug()<<"m_getDistanceBtn_clicked";
    this->m_pNeuvition_Auto->m_pLaserControl->getDistanceByID(0x6E);
}

void test::updateDistance(LASER_DATA_T m_LaserData)
{
    if(m_LaserData.id == 18){
        ui->m_lID018->setText(QString::number(m_LaserData.distance)+"mm");
    }else if (m_LaserData.id == 110) {
        ui->m_lID110->setText(QString::number(m_LaserData.distance)+"mm");
		ui->lineEdit_realDis->setText(QString::number((int)m_LaserData.distance));
		
    }else if (m_LaserData.id == 111) {
        ui->m_lID111->setText(QString::number(m_LaserData.distance)+"mm");
    } else {
        ui->m_lCom->setText(QString("COM3 open failed!!!!"));
    }
}

int i = 0;
void test::btnPTZSwitch_clicked()
{
    bool bRet;
    i++;
    qDebug()<<"btnPTZSwitch_clicked:i:"<<i;
    bRet = this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Switch();
    if(bRet)
    {
        ui->btnPTZSwitch->setText("Open");
    }
    else
    {
        ui->btnPTZSwitch->setText("Close");
    }
}

void test::btnReset_clicked()
{
    this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Reset();
}

void test::btnUp_clicked()
{
    bool bRet;
    float fStep = ui->leVecStep->text().toFloat(&bRet);
    if(bRet)
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Up(fStep);
    }
    else
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Up();
    }
}

void test::btnDown_clicked()
{
    bool bRet;
    float fStep = ui->leVecStep->text().toFloat(&bRet);
    if(bRet)
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Down(fStep);
    }
    else
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Down();
    }
}

void test::btnLeft_clicked()
{
    bool bRet;
    float fStep = ui->leHorStep->text().toFloat(&bRet);
    if(bRet)
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Left(fStep);
    }
    else
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Left();
    }
}

void test::btnRight_clicked()
{
    bool bRet;
    float fStep = ui->leHorStep->text().toFloat(&bRet);
    if(bRet)
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Right(fStep);
    }
    else
    {
        this->m_pNeuvition_Auto->m_pPTZPanel->PTZ_Right();
    }
}

void test::PTZMoveFinish(float fHorAngle, float fVecAngle)
{
    ui->lbCurHorAngle->setText(QString::number(fHorAngle));
    ui->lbCurVecAngle->setText(QString::number(fVecAngle));
	lidarHorAngle = fHorAngle;
	lidarVerAngle = fVecAngle;
}

void test::PTZQueryFinish(float fHorAngle, float fVecAngle)
{
    ui->lbCurHorAngle->setText(QString::number(fHorAngle));
    ui->lbCurVecAngle->setText(QString::number(fVecAngle));
	lidarHorAngle = fHorAngle;
	lidarVerAngle = fVecAngle;
}


