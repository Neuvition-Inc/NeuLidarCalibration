#include "CPCLPainter.h"

CPCLPainter::CPCLPainter()
{

}

CPCLPainter::~CPCLPainter()
{

}

std::string CPCLPainter::drawLine(double x0, double y0, double z0, double x1, double y1, double z1, boost::shared_ptr<pcl::visualization::PCLVisualizer> pViewer, double r /*= 255.0*/, double g /*= 255.0*/, double b /*= 255.0*/)
{
	string line_id = getUuid();
	PointT point1;
	point1.x = x0;
	point1.y = y0;
	point1.z = z0;

	PointT point2;
	point2.x = x1;
	point2.y = y1;
	point2.z = z1;

	pViewer->addLine<PointT>(point1, point2, r, g, b, line_id);
	return line_id;
}

std::string CPCLPainter::drawPoint(double x, double y, double z, PCLViewer_Ptr& pViewer, int r, int g, int b, int nSize)
{
	//暂未实现
	return string();
}



std::string CPCLPainter::drawPlane(pcl::ModelCoefficients& plane_coeff, pcl::visualization::PCLVisualizer::Ptr pPCLViewer, float scale[2] /*= nullptr*/, int viewport /*= 0*/)
{
	//添加自定义的平面
	auto plane = createPlane(plane_coeff,scale);
	std::string sId = getUuid();
	pPCLViewer->addModelFromPolyData(plane, sId);
	pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0.9, 0.1, 0.1, sId, viewport);
	pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 0.6, sId, viewport);
	//viewer.setShapeRenderingProperties (pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, "plane_1", 0);

	return sId;
}



std::string CPCLPainter::drawPlane(pcl::ModelCoefficients& plane_coeff, pcl::visualization::PCLVisualizer::Ptr pPCLViewer, double x, double y, double z, float scale[2] /*= nullptr*/, int viewport /*= 0*/)
{

	//添加自定义的平面
	auto plane = createPlane(plane_coeff, x, y, z, scale);
	std::string sId = getUuid();
	pPCLViewer->addModelFromPolyData(plane, sId);
	pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 0.9, 0.1, 0.1, sId, viewport);
	pPCLViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 0.6, sId, viewport);
	//viewer.setShapeRenderingProperties (pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, "plane_1", 0);

	return sId;

}


std::string CPCLPainter::drawPlaneVerticalZAxisByCloud(
	float fx0, float fy0, float fx1, float fy1, float fz, 
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr pOutCloud,
	pcl::visualization::PCLVisualizer::Ptr pPCLViewer/*= nullptr*/,
	float fXPrec /*= 0.01*/, float fYPrec /*= 0.01*/, 
	int nR /*= 255*/, int nG /*= 0*/, int nB /*= 0 */,
	int viewport /*= 0 */
)
{
	//pcl::PointCloud<pcl::PointXYZRGBA>::Ptr  pDrawPoint(new pcl::PointCloud<pcl::PointXYZRGBA>);

	float fMaxX = fx1 > fx0 ? fx1 : fx0;
	float fMinX = fx1 > fx0 ? fx0 : fx1;
	float fMaxY = fy1 > fy0 ? fy1 : fy0;
	float fMinY = fy1 > fy0 ? fy0 : fy1;


	int nXStep = (fMaxX - fMinX) / fXPrec;
	int nYStep = (fMaxY - fMinY) / fXPrec;


	for (int i = 0; i <= nXStep; i++)
	{
		for (int j = 0; j <= nYStep; j++)
		{
			float fx = fMinX + i * fXPrec;
			float fy = fMinY + j * fYPrec;

			pcl::PointXYZRGBA point;
			point.x = fx;
			point.y = fy;
			point.z = fz;
			point.r = nR;
			point.g = nG;
			point.b = nB;

			pOutCloud->points.push_back(point);
		}
	}
	
	pOutCloud->resize(pOutCloud->points.size());

	std::string sId;
	if (pPCLViewer)
	{
		std::string sId = getUuid();
		pPCLViewer->addPointCloud(pOutCloud, sId, viewport);
	}
	

	return sId;
}


std::string CPCLPainter::draw3dCirclePointCloud(Eigen::VectorXf& circle_coeff, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud, pcl::visualization::PCLVisualizer::Ptr pPCLViewer /*= nullptr*/, int nR /*= 255*/, int nG /*= 255*/, int nB /*= 255*/, int viewport /*= 0 */)
{
	double m_pi = M_PI;// 3.1415926;

	//std::cout << "圆心坐标（x, y, z）  r 平面的法向量 normal(x,y,z) = " << circle_coeff << std::endl;
	double nx = circle_coeff[4], ny = circle_coeff[5], nz = circle_coeff[6];
	double cx = circle_coeff[0], cy = circle_coeff[1], cz = circle_coeff[2];
	double r = circle_coeff[3];

	double ux = ny, uy = -nx, uz = 0;
	double vx = nx * nz,
		vy = ny * nz,
		vz = -nx * nx - ny * ny;

	double sqrtU = sqrt(ux*ux + uy * uy + uz * uz);
	double sqrtV = sqrt(vx*vx + vy * vy + vz * vz);

	double ux_ = (1 / sqrtU)*ux;
	double uy_ = (1 / sqrtU)*uy;
	double uz_ = (1 / sqrtU)*uz;

	double vx_ = (1 / sqrtV)*vx;
	double vy_ = (1 / sqrtV)*vy;
	double vz_ = (1 / sqrtV)*vz;

	double xi, yi, zi;
	double t = 0;
	double angle = (t / 180.0)*m_pi;
	vector<double> x, y, z;

	while (t < 360.0)
	{
		xi = cx + r * (ux_*cos(angle) + vx_ * sin(angle));
		yi = cy + r * (uy_*cos(angle) + vy_ * sin(angle));
		zi = cz + r * (uz_*cos(angle) + vz_ * sin(angle));
		x.push_back(xi);
		y.push_back(yi);
		z.push_back(zi);

		t = t + 1;
		angle = (t / 180.0)*m_pi;
	}
	//pcl::PointCloud<pcl::PointXYZ>::Ptr theroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);

	//定义cloudPoints大小,无序点云
	p3dCircleCloud->resize(x.size());
	for (int i = 0; i < x.size(); i++) {
		//将三维坐标赋值给PCL点云坐标
		(*p3dCircleCloud)[i].x = x[i];
		(*p3dCircleCloud)[i].y = y[i];
		(*p3dCircleCloud)[i].z = z[i];
		(*p3dCircleCloud)[i].r = nR;
		(*p3dCircleCloud)[i].g = nG;
		(*p3dCircleCloud)[i].b = nB;
		(*p3dCircleCloud)[i].a = 255;
	}


	//圆心的点也绘制上去
	pcl::PointXYZRGBA pointcircle;
	pointcircle.x = circle_coeff[0];
	pointcircle.y = circle_coeff[1];
	pointcircle.z = circle_coeff[2];
	pointcircle.r = nR;
	pointcircle.g = nG;
	pointcircle.b = nB;
	pointcircle.a = 255;
	p3dCircleCloud->push_back(pointcircle);


	std::string sId;
	if (pPCLViewer)
	{
		sId = getUuid();
		pPCLViewer->addPointCloud(p3dCircleCloud, sId, viewport);

		//显示坐标
		std::string sTextId = sId + "_text";
		QString sPoint = QString("(%1,%2,%3) r = %4").arg(pointcircle.x).arg(pointcircle.y).arg(pointcircle.z).arg(circle_coeff[3]);
		float fontSize = 0.15;
		pPCLViewer->addText3D(sPoint.toLocal8Bit().toStdString(), pcl::PointXYZ(pointcircle.x, pointcircle.y, pointcircle.z), fontSize,1,0,0, sTextId);
	}


	return sId;

}

std::string CPCLPainter::draw3dCirclePointCloud(Eigen::VectorXf& circle_coeff, pcl::visualization::PCLVisualizer::Ptr pPCLViewer /*= nullptr*/, int nR /*= 255*/, int nG /*= 255*/, int nB /*= 255*/, int viewport /*= 0 */)
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	return draw3dCirclePointCloud(circle_coeff, p3dCircleCloud, pPCLViewer, nR, nG, nB, viewport);
}

void CPCLPainter::draw3dCirclePointCloudNoText(Eigen::VectorXf& circle_coeff, QString sId, pcl::visualization::PCLVisualizer::Ptr pPCLViewer /*= nullptr*/, int nR /*= 255*/, int nG /*= 255*/, int nB /*= 255*/, int viewport /*= 0 */)
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	return draw3dCirclePointCloudNoText(circle_coeff, p3dCircleCloud,sId, pPCLViewer, nR, nG, nB, viewport);
}

void CPCLPainter::draw3dCirclePointCloudNoText(
	Eigen::VectorXf& circle_coeff, 
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr p3dCircleCloud, 
	QString sId, 
	pcl::visualization::PCLVisualizer::Ptr pPCLViewer /*= nullptr*/, 
	int nR /*= 255*/, int nG /*= 255*/, int nB /*= 255*/, int viewport /*= 0 */)
{
	double m_pi = M_PI;// 3.1415926;

	//std::cout << "圆心坐标（x, y, z）  r 平面的法向量 normal(x,y,z) = " << circle_coeff << std::endl;
	double nx = circle_coeff[4], ny = circle_coeff[5], nz = circle_coeff[6];
	double cx = circle_coeff[0], cy = circle_coeff[1], cz = circle_coeff[2];
	double r = circle_coeff[3];

	double ux = ny, uy = -nx, uz = 0;
	double vx = nx * nz,
		vy = ny * nz,
		vz = -nx * nx - ny * ny;

	double sqrtU = sqrt(ux*ux + uy * uy + uz * uz);
	double sqrtV = sqrt(vx*vx + vy * vy + vz * vz);

	double ux_ = (1 / sqrtU)*ux;
	double uy_ = (1 / sqrtU)*uy;
	double uz_ = (1 / sqrtU)*uz;

	double vx_ = (1 / sqrtV)*vx;
	double vy_ = (1 / sqrtV)*vy;
	double vz_ = (1 / sqrtV)*vz;

	double xi, yi, zi;
	double t = 0;
	double angle = (t / 180.0)*m_pi;
	vector<double> x, y, z;

	while (t < 360.0)
	{
		xi = cx + r * (ux_*cos(angle) + vx_ * sin(angle));
		yi = cy + r * (uy_*cos(angle) + vy_ * sin(angle));
		zi = cz + r * (uz_*cos(angle) + vz_ * sin(angle));
		x.push_back(xi);
		y.push_back(yi);
		z.push_back(zi);

		t = t + 1;
		angle = (t / 180.0)*m_pi;
	}
	//pcl::PointCloud<pcl::PointXYZ>::Ptr theroyCirclePoints(new pcl::PointCloud<pcl::PointXYZ>);

	//定义cloudPoints大小,无序点云
	p3dCircleCloud->resize(x.size());
	for (int i = 0; i < x.size(); i++) {
		//将三维坐标赋值给PCL点云坐标
		(*p3dCircleCloud)[i].x = x[i];
		(*p3dCircleCloud)[i].y = y[i];
		(*p3dCircleCloud)[i].z = z[i];
		(*p3dCircleCloud)[i].r = nR;
		(*p3dCircleCloud)[i].g = nG;
		(*p3dCircleCloud)[i].b = nB;
		(*p3dCircleCloud)[i].a = 255;
	}


	//圆心的点也绘制上去
	pcl::PointXYZRGBA pointcircle;
	pointcircle.x = circle_coeff[0];
	pointcircle.y = circle_coeff[1];
	pointcircle.z = circle_coeff[2];
	pointcircle.r = nR;
	pointcircle.g = nG;
	pointcircle.b = nB;
	pointcircle.a = 255;
	p3dCircleCloud->push_back(pointcircle);



	if (pPCLViewer)
	{
		pPCLViewer->addPointCloud(p3dCircleCloud, sId.toLocal8Bit().toStdString(), viewport);
	}

}

#include <QUuid>

std::string getUuid()
{

	QUuid id = QUuid::createUuid();
	QString tmp = id.toString();
	/*qint8 i = 0;
	qint8 j = 0;
	QByteArray myuuid;
	while (tmp[i] != '\0') {
		if (tmp[i] != '{' && tmp[i] != '-' && tmp[i] != '}') {
			myuuid.push_back(tmp[i].toLatin1());
			j++;
		}
		i++;
	}*/

	return tmp.toLocal8Bit().toStdString();
}


