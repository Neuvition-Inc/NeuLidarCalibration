#include "CVTKWidget.h"

#include <pcl/filters/filter.h>
#include <QMessageBox>
//#include "../calibrationDialog.h"
#include <vtkRenderWindow.h>


#define LOCALPCDNAME  "neuv_local_pcd"
//全局变量定义
double mPCLCamValue[33] = { 0 };
const double CAM_DIRECTION_ARRAY[] = { 0, 0, 1, 0, 0, -22.5795, 0, 1, 0, 0.374933, 374.933,
-3.67363,   3.13839,   59.8523,    -6.76314,  284.085,   53.9988,    0.00443089, 0.0208787, 0.999772, 266.658,  309.021,
31.794207, -5.979454,  84.093829, -17.029323, 8.127747,  -23.430791, 0.113900,   0.990408,  0.078223, 0.442249, 442.249090
};
//extern std::vector<DisLocation> dislocationVec;

//extern std::vector<std::vector<neuvition::NeuvUnit>> locationptsVec;
extern bool c_cloudcopy_enabled;


extern bool showValidCube;
extern float coreX;
extern float coreY;
extern float coreZ;

CVTKWidget::CVTKWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	//初始化PCLViewer
	_initPCLViewer();

	//配置QVTK属性
	_configQVTKWgt();

	//初始化点云
	_initRenderPointCloud();

	//初始化注册鼠标键盘事件的回调
	//_initViewerRegisterCB();

	m_pLocalCloud.reset(new pcl::PointCloud<pcl::PointXYZRGBA>);


	//??test
	//update_pcl_cam_direction(1);
	//m_pPCLViewer->addCoordinateSystem();
	
	ui.qvtkWidget->GetRenderWindow()->Render();
	setaddtextline();
	
	setaddtextline();
}

CVTKWidget::~CVTKWidget()
{
}

void CVTKWidget::copyLocalPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud)
{
	pcl::copyPointCloud(*m_pLocalCloud, *pCloud);
}

void CVTKWidget::updateRenderPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{

	//m_pPCLViewer->removePointCloud("neuvgram");

	//m_pPCLViewer->addPointCloud(pCloud, "neuvgram");
	if (showValidCube)
	{
		m_pPCLViewer->removeShape("1");

		m_pPCLViewer->addCube(coreX -1, coreX + 1, coreY -1 , coreY +1 , coreZ -1, coreZ +1, 1, 0, 0, "1");
		m_pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, "1");
	}
	else
	{
		m_pPCLViewer->removeShape("1");
	}

	m_pPCLViewer->updatePointCloud(pCloud, "neuvgram");

	ui.qvtkWidget->GetRenderWindow()->Render();
}

bool CVTKWidget::addRenderPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud, QString sCloudId, int nPointSize /*= 1*/)
{
	std::string sid = sCloudId.toLocal8Bit().toStdString();
	m_pPCLViewer->addPointCloud(pCloud, sid);
	m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, nPointSize, sid);
	ui.qvtkWidget->GetRenderWindow()->Render();

	return true;
}

bool CVTKWidget::addRenderPointCloudWithColor(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud, QString sCloudId, int nR /*= 255*/, int nG /*= 0*/, int nB /*= 0*/, int nPointSize /*= 1*/)
{
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZRGBA> single_color(pCloud, nR, nG, nB);

	std::string sid = sCloudId.toLocal8Bit().toStdString();

	m_pPCLViewer->addPointCloud<pcl::PointXYZRGBA>(pCloud, single_color, sid);

	m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, nPointSize, sid);
	ui.qvtkWidget->GetRenderWindow()->Render();

	return true;
}

bool CVTKWidget::addArrow(Eigen::Vector3f point, int nR /*= 255*/, int nG /*= 0*/, int nB /*= 0*/, QString sId /*= QString("arrow")*/, Eigen::Vector3f oripoint /*= Eigen::Vector3f()*/)
{
	pcl::PointXYZ oriPoint;
	pcl::PointXYZ targetPoint;
	oriPoint.x = oripoint[0]; oriPoint.y = oripoint[1]; oriPoint.z = oripoint[2];
	targetPoint.x = point[0]; targetPoint.y = point[1]; targetPoint.z = point[2];
	return m_pPCLViewer->addArrow(oriPoint,targetPoint,nR,nG,nB,sId.toLocal8Bit().toStdString());
}

bool CVTKWidget::addArrow(Eigen::VectorXf normal_coeff, int nR /*= 255*/, int nG /*= 0*/, int nB /*= 0*/, QString sId /*= QString("arrow")*/, Eigen::Vector3f oripoint /*= Eigen::Vector3f()*/)
{
	Eigen::Vector3f nor_plane(normal_coeff[0], normal_coeff[1], normal_coeff[2]);
	return addArrow(nor_plane, nR, nG, nB, sId, oripoint);
}

//void CVTKWidget::config_c_area_color(const neuvition::NeuvUnit & np, uint8_t & r, uint8_t & g, uint8_t & b)
//{
//	for (int i = 0; i < dislocationVec.size(); ++i)
//	{
//		int minRangePixel = dislocationVec[i].pixel - dislocationVec[i].radius;
//		int maxRangePixel = dislocationVec[i].pixel + dislocationVec[i].radius;
//		int minRangeLine = dislocationVec[i].line - dislocationVec[i].radius;
//		int maxRangeLine = dislocationVec[i].line + dislocationVec[i].radius;
//		if (np.col > minRangePixel && np.col < maxRangePixel && np.row > minRangeLine && np.row < maxRangeLine)
//		{
//			r = g = b = 255;
//			if (c_cloudcopy_enabled)
//			{
//				locationptsVec[i].push_back(np);
//			}
//		}
//	}
//}

void CVTKWidget::setaddtextline()
{
	int maxSize = 30000;
	int currentSize = 500;
	int multiple = 0;
	float fontSize;

	double r = 0.3;
	double b = 0.3;
	double g = 0.3;
	m_pPCLViewer->removeAllShapes();
	for (int x = -30; x <= 30; x++)
	{
		std::string lineidx = "line-x-";
		lineidx.append(boost::lexical_cast<std::string>(x * 0.5));
		m_pPCLViewer->addLine<pcl::PointXYZ, pcl::PointXYZ>(pcl::PointXYZ(x*0.5, 0, 0), pcl::PointXYZ(x * 0.5, 0, 60), 0.5, 0.5, 0.5, lineidx);
		std::string text;
		text.append(boost::lexical_cast<std::string>(x * 0.5));
		fontSize = 0.1;
		m_pPCLViewer->addText3D(text, pcl::PointXYZ(x * 0.5, 0, 0), fontSize, 0.5, 0.5, 0.5, text + "x");
	}
	for (int z = 0; z <= 120; z++)
	{
		std::string lineid = "line-z-";
		lineid.append(boost::lexical_cast<std::string>(z * 0.5));

		m_pPCLViewer->addLine<pcl::PointXYZ, pcl::PointXYZ>(pcl::PointXYZ(-15, 0, z * 0.5), pcl::PointXYZ(15, 0, z * 0.5), r, g, b, lineid);

		std::string text;
		text.append(boost::lexical_cast<std::string>(z * 0.5));
		fontSize = 0.1;
		m_pPCLViewer->addText3D(text, pcl::PointXYZ(-0.1, 0, z * 0.5), fontSize, 0.5, 0.5, 0.5, text);
// 		if ((z % (30 / 20)) == 0)
// 		{
// 			std::string text;
// 			text.append(boost::lexical_cast<std::string>(z*10));
// 			fontSize = 0.15;
// 			m_pPCLViewer->addText3D(text, pcl::PointXYZ(-0.3, 0, z * currentSize / 1000), fontSize, 0.5, 0.5, 0.5, text);
// 		}
	}
// 	for (int testx = -30; testx <= 30; testx++)
// 	{
// 		std::string testtext;
// 		testtext.append(boost::lexical_cast<std::string>(testx ));
// 		fontSize = 0.1;
// 		m_pPCLViewer->addText3D(testtext, pcl::PointXYZ(testx * 0.5, 0, 0), fontSize, 0.5, 0.5, 0.5, testtext + "x");
// 	}
// 	for (int testx1 = 1; testx1 <= 10; testx1++)
// 	{
// 		std::string testtext;
// 		testtext.append(boost::lexical_cast<std::string>(testx1*10));
// 		fontSize = 0.15;
// 		m_pPCLViewer->addText3D(testtext, pcl::PointXYZ(-(testx1 * currentSize / 1000), 0, 0), fontSize, 0.5, 0.5, 0.5, testtext + "xx");
// 	}
	ui.qvtkWidget->GetRenderWindow()->Render();
}

void CVTKWidget::addCube(const Eigen::Vector3f &translation, const Eigen::Quaternionf &rotation, double width, double height, double depth, const std::string &id /*= "cube"*/, int viewport /*= 0*/)
{
	if (m_pPCLViewer)
	{
		m_pPCLViewer->removeShape(id);

		m_pPCLViewer->addCube(translation, rotation, width,  height, depth, id, viewport);
		m_pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, id);
	
		ui.qvtkWidget->GetRenderWindow()->Render();
	}
}

bool CVTKWidget::renderLocalPcd(QString sFileName)
{
	

	//待优化：是否可以直接用pCloud，而不要使用成员变量m_pLocalCloud，感觉没什么必要。不过，如果有加工处理的话，还是
	//需要m_pLocallCloud

	std::string sFile = sFileName.toLocal8Bit().toStdString();
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	if (pcl::io::loadPCDFile(sFile, *pCloud) == -1)
	{
		return false;
	}

	//清空原本点云
	m_pPCLViewer->removePointCloud(LOCALPCDNAME);
	

	//去除NAN 的点
	m_pLocalCloud->clear();

	std::vector<int> indices;
	pcl::removeNaNFromPointCloud(*pCloud, *m_pLocalCloud, indices);
	//pcl::copyPointCloud(*pCloud, *m_pLocalCloud);

	pcl::visualization::PointCloudColorHandlerRGBAField<pcl::PointXYZRGBA> rgb(m_pLocalCloud);
	

	//m_pPCLViewer->addPointCloud(m_pLocalCloud, rgb, LOCALPCDNAME);
	m_pPCLViewer->addPointCloud(m_pLocalCloud, LOCALPCDNAME);
	m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, m_nPointSizeValue, LOCALPCDNAME);
	ui.qvtkWidget->update();
	
	return true;
}

bool CVTKWidget::renderLocalPcd(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{
	if (pCloud->size() <= 0)
	{
		return false;
	}
	//清空原本点云
	m_pPCLViewer->removePointCloud(LOCALPCDNAME);
	

	//去除NAN 的点
	m_pLocalCloud->clear();

	std::vector<int> indices;
	pcl::removeNaNFromPointCloud(*pCloud, *m_pLocalCloud, indices);
	//pcl::copyPointCloud(*pCloud, *m_pLocalCloud);

	pcl::visualization::PointCloudColorHandlerRGBAField<pcl::PointXYZRGBA> rgb(m_pLocalCloud);
	

	//m_pPCLViewer->addPointCloud(m_pLocalCloud, rgb, LOCALPCDNAME);
	m_pPCLViewer->addPointCloud(m_pLocalCloud, LOCALPCDNAME);
	m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, m_nPointSizeValue, LOCALPCDNAME);
	ui.qvtkWidget->update();

	return true;
}

void CVTKWidget::addCoor()
{
	m_pPCLViewer->addCoordinateSystem();
}

void CVTKWidget::removeCoor()
{
	m_pPCLViewer->removeCoordinateSystem();
}

void CVTKWidget::update_pcl_cam_direction(int nIndex)
{
	switch (nIndex) {
	case 0: {
		mPCLCam.focal[0] = mPCLCamValue[0];
		mPCLCam.focal[1] = mPCLCamValue[1];
		mPCLCam.focal[2] = mPCLCamValue[2];
		mPCLCam.pos[0] = mPCLCamValue[3];
		mPCLCam.pos[1] = mPCLCamValue[4];
		mPCLCam.pos[2] = mPCLCamValue[5];
		mPCLCam.view[0] = mPCLCamValue[6];
		mPCLCam.view[1] = mPCLCamValue[7];
		mPCLCam.view[2] = mPCLCamValue[8];
		mPCLCam.window_size[0] = ui.qvtkWidget->width();
		mPCLCam.window_size[1] = ui.qvtkWidget->height();
		mPCLCam.clip[0] = mPCLCamValue[9];
		mPCLCam.clip[1] = mPCLCamValue[10];
		break;
	}
	case 1: {
		mPCLCam.focal[0] = mPCLCamValue[11];
		mPCLCam.focal[1] = mPCLCamValue[12];
		mPCLCam.focal[2] = mPCLCamValue[13];
		mPCLCam.pos[0] = mPCLCamValue[14];
		mPCLCam.pos[1] = mPCLCamValue[15];
		mPCLCam.pos[2] = mPCLCamValue[16];
		mPCLCam.view[0] = mPCLCamValue[17];
		mPCLCam.view[1] = mPCLCamValue[18];
		mPCLCam.view[2] = mPCLCamValue[19];
		mPCLCam.window_size[0] = ui.qvtkWidget->width();
		mPCLCam.window_size[1] = ui.qvtkWidget->height();
		mPCLCam.clip[0] = mPCLCamValue[20];
		mPCLCam.clip[1] = mPCLCamValue[21];
		break;
	}
	case 2: {
		mPCLCam.focal[0] = mPCLCamValue[22];
		mPCLCam.focal[1] = mPCLCamValue[23];
		mPCLCam.focal[2] = mPCLCamValue[24];
		mPCLCam.pos[0] = mPCLCamValue[25];
		mPCLCam.pos[1] = mPCLCamValue[26];
		mPCLCam.pos[2] = mPCLCamValue[27];
		mPCLCam.view[0] = mPCLCamValue[28];
		mPCLCam.view[1] = mPCLCamValue[29];
		mPCLCam.view[2] = mPCLCamValue[30];
		mPCLCam.window_size[0] = ui.qvtkWidget->width();
		mPCLCam.window_size[1] = ui.qvtkWidget->height();
		mPCLCam.clip[0] = mPCLCamValue[31];
		mPCLCam.clip[1] = mPCLCamValue[32];
		break;
	}
	}
	m_pPCLViewer->setCameraPosition(mPCLCam.pos[0], mPCLCam.pos[1], mPCLCam.pos[2], mPCLCam.focal[0], mPCLCam.focal[1], mPCLCam.focal[2], mPCLCam.view[0], mPCLCam.view[1], mPCLCam.view[2]);
	ui.qvtkWidget->GetRenderWindow()->Render();

	
}

void CVTKWidget::set_pcl_cam_direction(const st_PCL_CAMERA_POSITION stPCLCamParam)
{
	m_pPCLViewer->setCameraPosition(stPCLCamParam.pos[0], stPCLCamParam.pos[1], stPCLCamParam.pos[2], stPCLCamParam.view[0], stPCLCamParam.view[1], stPCLCamParam.view[2], stPCLCamParam.up[0], stPCLCamParam.up[1], stPCLCamParam.up[2]);
}


void CVTKWidget::slotNeedRender()
{
	ui.qvtkWidget->GetRenderWindow()->Render();
}



void CVTKWidget::slotShowText(QString sText, QString sId, int nXPos, int nYPos)
{
	std::string sTextId = sId.toLocal8Bit().toStdString();
	std::string strText = sText.toLocal8Bit().toStdString();
	m_pPCLViewer->removeShape(sTextId, 0);
	m_pPCLViewer->addText(strText, nXPos, nYPos, 28, 1, 0, 0, sTextId, 0);

}

void CVTKWidget::slotAddArrow(pcl::PointXYZ a, pcl::PointXYZ b, int nR, int nG, int nB, QString sId)
{
	std::string strId = sId.toLocal8Bit().toStdString();

	m_pPCLViewer->removeShape(strId);

	m_pPCLViewer->addArrow<pcl::PointXYZ>(a, b, nR, nG, nB, strId);
}

void CVTKWidget::slotClearAllClouds()
{
	m_pPCLViewer->removeAllPointClouds();
	//会影响到使用updatecloud的点云生成,因为其把"neuvgram" 的点云也删除了。
	//临时解决方法：
	m_pPCLViewer->addPointCloud(m_pCloud, "neuvgram");
}

void CVTKWidget::slotClearCloudById(QString sId)
{
	std::string strId = sId.toLocal8Bit().toStdString();
	m_pPCLViewer->removePointCloud(strId);
}



void CVTKWidget::slotUpdateRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud)
{
	//渲染
	updateRenderPointCloud(pCloud);
}

void CVTKWidget::slotAddRenderCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId, int nR, int nG, int nB)
{
	//先删除同id点云
	m_pPCLViewer->removePointCloud(sCloudId.toStdString());
	//添加点云
	addRenderPointCloudWithColor(pRenderCloud, sCloudId, nR, nG, nB);
}

void CVTKWidget::slotAddRenderCloudWithSelfColor(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pRenderCloud, QString sCloudId, int nRenderSize)
{
	//先删除同id点云
	bool bRet = m_pPCLViewer->removePointCloud(sCloudId.toStdString());
	//添加点云
	addRenderPointCloud(pRenderCloud, sCloudId, nRenderSize);
}

void CVTKWidget::slotDrawCircleCloud(Eigen::VectorXf circle_coeff)
{
	static std::string s_sPreId;

	m_pPCLViewer->removeText3D(s_sPreId + "_text");
	m_pPCLViewer->removeShape(s_sPreId);

	CPCLPainter painter;
	//painter.draw3dCirclePointCloud(circle_coeff, getPCLViewerPtr(),255,0,0);
	s_sPreId = painter.draw3dCirclePointCloud(circle_coeff, m_pPCLViewer, 255, 0, 0);
}

void CVTKWidget::slotDrawCircleNoTextWithId(Eigen::VectorXf circle_coeff, QString sId)
{
	std::string strId = sId.toLocal8Bit().toStdString();
	m_pPCLViewer->removeShape(strId);

	CPCLPainter painter;
	//painter.draw3dCirclePointCloud(circle_coeff, getPCLViewerPtr(),255,0,0);
	painter.draw3dCirclePointCloudNoText(circle_coeff, sId, m_pPCLViewer, 255, 0, 0);
}

void CVTKWidget::slotAddCloudWithText(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pCloud, QString sText, QString sId, int nRenderSize, int nFontSize)
{
	std::string sCloudId = sId.toLocal8Bit().toStdString();
	std::string sTextId = sCloudId + "_text";
	m_pPCLViewer->removeShape(sCloudId);
	m_pPCLViewer->removeText3D(sTextId);
	//添加点云
	addRenderPointCloud(pCloud, sId, nRenderSize);

	Eigen::Vector4f centroid;
	pcl::compute3DCentroid(*pCloud, centroid);

	std::cout << "x,y,z = " << centroid[0] << " , " << centroid[1] << " , " << centroid[2] << std::endl;
	m_pPCLViewer->addText3D(sText.toLocal8Bit().toStdString(), pcl::PointXYZ(centroid[0], centroid[1], centroid[2]), nFontSize / 100.0, 1, 0, 0, sTextId);

}

//void CVTKWidget::slotDrawCube(CCube cube, QString sCubeId)
//{
//	if (m_pPCLViewer)
//	{
//		std::string sid = sCubeId.toLocal8Bit().toStdString();
//		m_pPCLViewer->removeShape(sid);
//
//		Eigen::Vector3f translation = cube.getTranslation();
//		Eigen::Quaternionf rotation = cube.getQuaternionf();
//		float fWidth, fHeigth, fDepth;
//		cube.getWHD(fWidth, fHeigth, fDepth);
//		m_pPCLViewer->addCube(translation, rotation, fWidth, fHeigth, fDepth, sid, 0);
//		m_pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, sid);
//
//	}
//}
//
//void CVTKWidget::slotDrawCubeWithColor(CCube cube, QString sCubeId, int nR, int nG, int nB)
//{
//	if (m_pPCLViewer)
//	{
//		std::string sid = sCubeId.toLocal8Bit().toStdString();
//		m_pPCLViewer->removeShape(sid);
//
//		Eigen::Vector3f translation = cube.getTranslation();
//		Eigen::Quaternionf rotation = cube.getQuaternionf();
//		float fWidth, fHeigth, fDepth;
//		cube.getWHD(fWidth, fHeigth, fDepth);
//		m_pPCLViewer->addCube(translation, rotation, fWidth, fHeigth, fDepth, sid, 0);
//		m_pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, sid);
//		m_pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, nR, nG, nB, sid);
//	}
//}

void CVTKWidget::slotCameraPosition(st_PCL_CAMERA_POSITION stCameraPos)
{
	m_pPCLViewer->setCameraPosition(stCameraPos.pos[0], stCameraPos.pos[1], stCameraPos.pos[2],
		stCameraPos.view[0], stCameraPos.view[1], stCameraPos.view[2],
		stCameraPos.up[0], stCameraPos.up[1], stCameraPos.up[2]);
}

void CVTKWidget::_initPCLViewer()
{
	m_pPCLViewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	
	ui.qvtkWidget->SetRenderWindow(m_pPCLViewer->getRenderWindow());
	m_pPCLViewer->setupInteractor(ui.qvtkWidget->GetInteractor(), ui.qvtkWidget->GetRenderWindow());

	m_pPCLViewer->setBackgroundColor(0, 0, 0);
	//m_pPCLViewer->setBackgroundColor(255, 255, 255);
	m_pPCLViewer->initCameraParameters();


	m_pPCLViewer->setShowFPS(false);
}

void CVTKWidget::_configQVTKWgt()
{
	ui.qvtkWidget->setMouseTracking(true);
}

void CVTKWidget::_initRenderPointCloud()
{
	m_pCloud.reset(new pcl::PointCloud<pcl::PointXYZRGBA>);
	m_pPCLViewer->addPointCloud(m_pCloud, "neuvgram");
	m_pPCLViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, m_nPointSizeValue, "neuvgram");
	m_pPCLViewer->resetCamera();
}

//void CVTKWidget::_initViewerRegisterCB()
//{
	//m_pPCLViewerRegCB = CPCLViewerRegisterCB::getInstance();

	//connect(m_pPCLViewerRegCB, &CPCLViewerRegisterCB::sigNeedRender, this, &CVTKWidget::slotNeedRender);
	//connect(m_pPCLViewerRegCB, &CPCLViewerRegisterCB::sigSelFinished, this, &CVTKWidget::slotSelFinished);

//}
