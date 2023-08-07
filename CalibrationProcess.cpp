#include "CalibrationProcess.h"
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/impl/extract_clusters.hpp>
#include "Nlinfitsolve.h"



//#define PCL_NO_PRECOMPILE
#define OPEN_VIEW 0
using namespace pcl;
using namespace neuvition;
//using namespace pcl;
extern bool showValidCube;
extern float coreX;
extern float coreY;
extern float coreZ;
CalibrationProcess::CalibrationProcess()
{
	depthCount = 0;
	memset(&lidarSerialNumber, 0, 32);
	memset(&depthDataNodes[0], 0, sizeof(DepthDataNode) * 30);
	mSurfaceCorPoints.clear();


}

CalibrationProcess::~CalibrationProcess()
{
	if (ofStream.is_open()) {
		ofStream.close();
	}
// 	if (mBofs.is_open())
// 	{
// 		mBofs.close();
// 	}
}

QString CalibrationProcess::GetDeviceName() {
	int mDeviceType = neuvition::get_device_type();
	QString device_type;
	switch (mDeviceType) {
	case 0: {
		device_type.append("Titan M1");
		break;
	}
	case 1: {
		device_type.append("Titan M1-Pro");
		break;
	}
	case 2: {
		device_type.append("Titan M1-Plus");
		break;
	}
	case 3: {
		device_type.append("Titan M1-R");
		break;
	}
	case 4: {
		device_type.append("Titan S1");
		break;
	}
	case 5: {
		device_type.append("Titan M1-A");
		break;
	}
	case 6: {
		device_type.append("TItan M1-SL");
		break;
	}
	case 8: {
		device_type.append("TItan M1-M");
		break;
	}
	default: {
		device_type.append("Initial");
		break;
	}
	}
	return  device_type;
}

void CalibrationProcess::SetSerialNumber(const char* serialNumber) {
	if(serialNumber)
	  strcpy(lidarSerialNumber, serialNumber);
}

char* CalibrationProcess::GetSerialNumber()
{
	return lidarSerialNumber;
}

bool CalibrationProcess::CreateCalibrationLog(const char *fileName) {
	QString workPath, calibrateFolderPath, calibratePath;
	QDir dir;
	QDateTime current;
	QString snow;
	workPath = QApplication::applicationDirPath();
	calibrateFolderPath.append(workPath).append("/calibration/").append(lidarSerialNumber).append("/");
	std::cout << "CreateCalibrationLog:" << calibrateFolderPath.toStdString() << std::endl;
	if (!strcmp(lidarSerialNumber, "")) {
		std::cout << " error, serialNumber is empty." << std::endl;
		return false;
	}
	if (!dir.exists(calibrateFolderPath)) {
		dir.mkpath(calibrateFolderPath);
	}
	if (ofStream.is_open()) {
		ofStream.close();
	}
	current = QDateTime::currentDateTime();
	snow = current.toString("yyyyMMdd");
	calibratePath.append(calibrateFolderPath).append(fileName).append(snow).append(".log");
	ofStream.open(calibratePath.toStdString(), ios::out | ios::app);
	return true;
}

bool CalibrationProcess::OriginCorrection(const neuvition::NeuvUnits& points) {
	QDateTime current;
	QString dataTime;
	bool IsCreateLogFile;
	int cloudSize, n;
	int i, index;
	int row, col;
	int x, y, z;
	int avgx, avgy, avgz;
	float radx, rady, radz;
	Vec3f eulerAnglesVec3f;
	char deviceParams[256] = { 0 };
	vector<float>  planeZVector;
	Vec3f srcNormal, dstNormal, crossNormal;

	cv::Mat planeCoeffMat(3, 1, CV_64F, cv::Scalar(0));
	cv::Mat rotationNormalMat(3, 1, CV_32F, cv::Scalar(0));
	cv::Mat rpMat(3, 1, CV_32F, cv::Scalar(0));
	cv::Mat pointMat(3, 1, CV_32F, cv::Scalar(0));
	cv::Mat rotationMat(3, 3, CV_32F, cv::Scalar(0));
	boost::shared_ptr<CAlgorithm> pCAlgorithm(new CAlgorithm);

	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer);
	PointCloud<NeuvPclPoint>::Ptr kdtreeCloud(new PointCloud<NeuvPclPoint>);
	PointCloud<NeuvPclPoint>::Ptr clusterCloud(new PointCloud<NeuvPclPoint>);
	PointCloud<NeuvPclPoint>::Ptr sortZCloud(new PointCloud<NeuvPclPoint>);
	PointCloud<NeuvPclPoint>::Ptr planeCloud(new PointCloud<NeuvPclPoint>);
	PointCloud<NeuvPclPoint>::Ptr rPlaneCloud(new PointCloud<NeuvPclPoint>);

	bool originCorEnabled;
	cv::Mat originCorMat;
	QString calibrationFilePath;
	calibrationFilePath = QApplication::applicationDirPath();
	calibrationFilePath.append("/calibration/").append(lidarSerialNumber).append("/c_config_params.json");

	neuvition::get_calibration_params(CAL_ORIGIN_COR, originCorEnabled, originCorMat);

	viewer->addCoordinateSystem();
	viewer->initCameraParameters();

	IsCreateLogFile = CreateCalibrationLog("OriginCorrection");
	if (!IsCreateLogFile) {
		std::cout << " Create Log File is Failed." << std::endl;
		return false;
	}
	current = QDateTime::currentDateTime();
	dataTime = current.toString("yyyyMMddhhmmss");
	sprintf(deviceParams, "%s\t%s\n", GetDeviceName().toStdString().c_str(), dataTime.toStdString().c_str());
	ofStream.write(deviceParams, strlen(deviceParams));

	cloudSize = points.size();
	if (cloudSize <= 0) {
		ofStream << " points size: " << cloudSize << std::endl;
		ofStream.close();
		return false;
	}

	index = 0;
	for (i = 0; i < cloudSize; i++) {
		const neuvition::NeuvUnit& pt = points.at(i);
		NeuvPclPoint ptl;
		ptl.x = pt.x / 1000.0; ptl.y = pt.y / 1000.0; ptl.z = pt.z / 1000.0;
		ptl.r = pt.r; ptl.g = pt.g; ptl.b = pt.b;
		ptl.label = i;
		if (ptl.x < 0.05 && ptl.x > -0.05 && ptl.y < 0.05 && ptl.y > -0.05) {
			index = ptl.label;
		}
		kdtreeCloud->push_back(ptl);
	}
	pCAlgorithm->kdtree_create(kdtreeCloud);
	pCAlgorithm->cloud_cluster_by_distance((PointCloud<NeuvPclPoint> *)kdtreeCloud.get(), index, 0.05, *clusterCloud);

	n = 0;
	avgx = 0; avgy = 0; avgz = 0;
	cloudSize = clusterCloud->size();
	for (PointCloud<NeuvPclPoint>::const_iterator iter = clusterCloud->begin(); iter != clusterCloud->end(); iter++) {
		const NeuvPclPoint& pt = *iter;
		int index = lower_bound(planeZVector.begin(), planeZVector.end(), pt.z) - planeZVector.begin();
		planeZVector.insert(planeZVector.begin() + index, pt.z);
		sortZCloud->insert(sortZCloud->begin() + index, pt);
	}
	for (i = sortZCloud->size() * 0.1; i <= sortZCloud->size() * 0.9; i++) {
		const NeuvPclPoint& pt = sortZCloud->points[i];
		planeCloud->push_back(pt);
	}
	pCAlgorithm->plane_fitting2(*planeCloud, planeCoeffMat);
	srcNormal[0] = planeCoeffMat.at<double>(0, 0);
	srcNormal[1] = planeCoeffMat.at<double>(1, 0);
	srcNormal[2] = -1;
	dstNormal[0] = 0;
	dstNormal[1] = 0;
	dstNormal[2] = -1;
	crossNormal = srcNormal.cross(dstNormal);
	ofStream << "srcNormal= " << srcNormal << std::endl;
	ofStream << "dstNormal= " << dstNormal << std::endl;
	ofStream << "crossNormal= " << crossNormal << std::endl;
	rotationNormalMat.at<float>(0, 0) = crossNormal[0];
	rotationNormalMat.at<float>(1, 0) = crossNormal[1];
	rotationNormalMat.at<float>(2, 0) = crossNormal[2];
	cv::Rodrigues(rotationNormalMat, rotationMat);
	ofStream << "rotationMat= " << rotationMat << std::endl;
	for (PointCloud<NeuvPclPoint>::const_iterator iter = planeCloud->begin(); iter != planeCloud->end(); iter++) {
		const NeuvPclPoint &pt = *iter;
		NeuvPclPoint rpt = pt;
		pointMat.at<float>(0, 0) = pt.x;
		pointMat.at<float>(1, 0) = pt.y;
		pointMat.at<float>(2, 0) = pt.z;
		rpMat = rotationMat * pointMat;
		rpt.x = rpMat.at<float>(0, 0);
		rpt.y = rpMat.at<float>(1, 0);
		rpt.z = rpMat.at<float>(2, 0);
		rpt.r = 255; rpt.g = 0; rpt.b = 0;
		rPlaneCloud->push_back(rpt);
	}
	Vec3f eulerAngles = pCAlgorithm->rotationMatrixToEulerAngles(rotationMat);
	ofStream << "eulerAngles= " << eulerAngles << std::endl;
	//viewer->addPointCloud<PointXYZRGBL>(planeCloud, "planeCloud");
	//viewer->addPointCloud<PointXYZRGBL>(rPlaneCloud, "rPlaneCloud");
	//viewer->spin();
	if (ofStream.is_open()) {
		ofStream.close();
	}
	originCorEnabled = true;
	originCorMat = rotationMat;
	neuvition::set_calibration_params(CAL_ORIGIN_COR, originCorEnabled, originCorMat);
	neuvition::set_calibration_params_by_http_json("192.168.1.101", calibrationFilePath.toStdString().c_str());
	neuvition::set_c_pos_cor_enabled(true);
	return true;
}

int CalibrationProcess::DepthCollection(const neuvition::NeuvUnits& points, int laserDepth) {
	const double lightspeed = 0.2995528160;

	QDateTime current;
	QString dataTime;
	bool IsCreateLogFile;
	int cloudSize, n, index;
	int i, row, col;
	uint8_t lid;
	int avgz,avgx,avgy, avgtof,avgrow,avgcol, bestAvgz;
	int x, y, z;
	float radiusSearch;
#if OPEN_VIEW
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer);
	PointCloud<PointXYZRGB>::Ptr viewCloud(new PointCloud<PointXYZRGB>);
	viewer->addCoordinateSystem();
    viewer->initCameraParameters();
#endif

	boost::shared_ptr<CAlgorithm> pCAlgorithm(new CAlgorithm);
	PointCloud<NeuvPclPoint>::Ptr kdtreeCloud(new PointCloud<NeuvPclPoint>);
	PointCloud<NeuvPclPoint>::Ptr clusterCloud(new PointCloud<NeuvPclPoint>);
	

	IsCreateLogFile = CreateCalibrationLog("DepthCorrection");
	if (!IsCreateLogFile) {
		std::cout << " Create Log File is Failed." << std::endl;
		return false;
	}
	
	cloudSize = points.size();
	if (cloudSize <= 0) {
		ofStream << " points size: " << cloudSize << std::endl;
		ofStream.close();
		return false;
	}
	n = 0;
	avgz = 0, avgx = 0, avgy = 0, bestAvgz = 25000;
	lid = points.at(0).lid;
	avgrow = 0, avgcol = 0, avgtof = 0;
	for (i = 0; i < cloudSize; i++) {
		const neuvition::NeuvUnit& pt = points.at(i);
		NeuvPclPoint nppt;
		nppt.x = pt.x / 1000.0; nppt.y = pt.y / 1000.0; nppt.z = pt.z / 1000.0;
		nppt.r = pt.r; nppt.g = pt.g; nppt.b = pt.b; nppt.a = 255;
		nppt.label = i; nppt.row = pt.row; nppt.col = pt.col; nppt.tof = pt.tof;
		kdtreeCloud->push_back(nppt);
	}
	pCAlgorithm->kdtree_create(kdtreeCloud);
	radiusSearch = 0.05;
	for (i = 0; i < kdtreeCloud->size(); i++) {
		const NeuvPclPoint & nppt = kdtreeCloud->at(i);
		if (nppt.x < 0.25 && nppt.x > -0.25 && nppt.y < 0.25 && nppt.y > -0.25
			&& nppt.z < 25 && nppt.label != 1) {
			clusterCloud->clear();
			if (nppt.z > 10) {
				radiusSearch = 0.10;
			}
			pCAlgorithm->cloud_cluster_by_distance((PointCloud<NeuvPclPoint> *)kdtreeCloud.get(), i, radiusSearch, *clusterCloud);
			if (clusterCloud->size() <= 10)
				continue;
			n = clusterCloud->size();
			std::cout << "cluster, size= " << clusterCloud->size() << std::endl;
			int curAvgx = 0, curAvgy = 0, curAvgz = 0;
			int curAvgRow = 0, curAvgCol = 0, curAvgTof = 0;
			for (PointCloud<NeuvPclPoint>::const_iterator iter = clusterCloud->begin(); iter != clusterCloud->end(); iter++) {
				const NeuvPclPoint & pt = *iter;
				curAvgx += int(pt.x * 1000);
				curAvgy += int(pt.y * 1000);
				curAvgz += int(pt.z * 1000);
				curAvgRow += pt.row;
				curAvgCol += pt.col;
				curAvgTof += pt.tof;
#if OPEN_VIEW
				PointXYZRGB vpt;
				vpt.x = pt.x; vpt.y = pt.y; vpt.z = pt.z;
				vpt.r = 255; vpt.g = 0; vpt.b = 0;
				viewCloud->push_back(vpt);
#endif
			}	
			if (curAvgz / n < bestAvgz) {
				avgx = curAvgx / n; avgy = curAvgy / n; avgz = curAvgz / n;
				avgrow = curAvgRow / n; avgcol = curAvgCol / n; avgtof = curAvgTof / n;
				bestAvgz = avgz;
				ofStream << " bestAvgz= " << bestAvgz << std::endl;
			}
#if OPEN_VIEW
			viewer->addPointCloud(viewCloud, "cloud");
			viewer->spin();
#endif
			//break;
		}
	}

	if (n <= 0) {
		ofStream << " search point size is empty." << std::endl;
		ofStream.close();
		return false;
	}

	if (depthCount < 30) {
		depthDataNodes[depthCount].laserId = points.at(0).lid;
		depthDataNodes[depthCount].lidarDepth = avgz;
		depthDataNodes[depthCount].laserDepth = laserDepth;
		//depthDataNodes[depthCount].lidarDepth = avgz * 2 / lightspeed / neuvition::get_matrix_data(lid, col, row, 2);
		//depthDataNodes[depthCount].laserDepth = laserDepth * 2 / lightspeed / neuvition::get_matrix_data(lid, col, row, 2);	
		ofStream << " depthCount= " << depthCount << " avgrow= " << avgrow << " avgcol= " << avgcol << " avgx= " << avgx << " avgy= " << avgy 
			<<" avgz= " << avgz << " avgtof = "<< avgtof  << " laserDepth= " << laserDepth << std::endl;
		depthCount++;
	}
	if (ofStream.is_open()) {
		ofStream.close();
	}
	return true;
}

bool CalibrationProcess::DepthCorrection() {
	const double lightspeed = 0.2995528160;

	bool IsCalibrationSucceed;
	QDateTime current;
	QString dataTime;
	bool IsCreateLogFile;
	int i;
	int lidarDepth, laserDepth;
	std::vector<double> lidarDepthVec;
	std::vector<double> diffDepthVec;
	cv::Mat linearMat(2, 1, CV_64F, cv::Scalar(0));
	char deviceParams[256] = { 0 };
	int maxDiffDepth = INT_MIN;

	boost::shared_ptr<CAlgorithm> pCAlgorithm(new CAlgorithm);

	bool depthZCorEnabled;
	cv::Mat depthZCorMat;
	QString calibrationFilePath;
	calibrationFilePath = QApplication::applicationDirPath();
	calibrationFilePath.append("/calibration/").append(lidarSerialNumber).append("/c_config_params.json");

	neuvition::get_calibration_params(CAL_DEPTH_Z_COR, depthZCorEnabled, depthZCorMat);

	IsCalibrationSucceed = false;
	IsCreateLogFile = CreateCalibrationLog("DepthCorrection");
	if (!IsCreateLogFile) {
		std::cout << " Create Log File is Failed." << std::endl;
		IsCalibrationSucceed = false;
		goto end;
	}
	current = QDateTime::currentDateTime();
	dataTime = current.toString("yyyyMMddhhmmss");
	sprintf(deviceParams, "%s\t%s\n", GetDeviceName().toStdString().c_str(), dataTime.toStdString().c_str());
	ofStream.write(deviceParams, strlen(deviceParams));

	if (depthCount < 3) {
		ofStream << "depthCount less than 3. " << std::endl;
		IsCalibrationSucceed = false;
		goto end;
	}
	for (i = 0; i < depthCount; i++) {
		lidarDepth = depthDataNodes[i].lidarDepth;
		laserDepth = depthDataNodes[i].laserDepth;
		lidarDepthVec.push_back(lidarDepth);
		diffDepthVec.push_back(laserDepth - lidarDepth);
	}
	pCAlgorithm->curve_fitting(lidarDepthVec, diffDepthVec, linearMat, 1);
	ofStream << " linearMat: " << linearMat << std::endl;
	for (i = 0; i < depthCount; ++i)
	{
		lidarDepth = depthDataNodes[i].lidarDepth;
		laserDepth = depthDataNodes[i].laserDepth;
		int improLidarDepth = lidarDepth + linearMat.at<double>(0, 0) * lidarDepth + linearMat.at<double>(1, 0);
		int diffDepth = abs(laserDepth - improLidarDepth);
		if (diffDepth > maxDiffDepth) {
			maxDiffDepth = diffDepth;
		}
	}
	if (maxDiffDepth > 30) {
		ofStream << "failed, maxDiffDepth= " << maxDiffDepth << std::endl;
		IsCalibrationSucceed = false;
		goto end;
	}
	IsCalibrationSucceed = true;
	depthZCorMat.at<float>(depthDataNodes[0].laserId - 1, 1) = (float)linearMat.at<double>(0, 0);
	depthZCorMat.at<float>(depthDataNodes[0].laserId - 1, 2) = (float)linearMat.at<double>(1, 0);

end:
	depthCount = 0;
	memset(&depthDataNodes[0], 0, sizeof(DepthDataNode) * 30);
	if (ofStream.is_open()) {
		ofStream.close();
	}
	if (IsCalibrationSucceed) {
		depthZCorEnabled = true;
		neuvition::set_calibration_params(CAL_DEPTH_Z_COR, depthZCorEnabled, depthZCorMat);
		neuvition::set_calibration_params_by_http_json("192.168.1.101", calibrationFilePath.toStdString().c_str());
	}
	return IsCalibrationSucceed;
}

void CalibrationProcess::ClearDepthData() {
	bool depthZCorEnabled;
	cv::Mat depthZCorMat;
	//QString calibrationFilePath;
	//calibrationFilePath = QApplication::applicationDirPath();
	//calibrationFilePath.append("/calibration/").append(lidarSerialNumber).append("/c_config_params.json");

	neuvition::get_calibration_params(CAL_DEPTH_Z_COR, depthZCorEnabled, depthZCorMat);

	if (depthZCorEnabled) {
		depthZCorEnabled = false;
		neuvition::set_calibration_params(CAL_DEPTH_Z_COR, depthZCorEnabled, depthZCorMat);
		//neuvition::set_calibration_params_by_http_json("192.168.1.101", calibrationFilePath.toStdString().c_str());
	}

	depthCount = 0;
	memset(&depthDataNodes[0], 0, sizeof(DepthDataNode) * 30);
	if (ofStream.is_open()) {
		ofStream.close();
	}
}

int CalibrationProcess::SurfaceCollection(const neuvition::NeuvUnits& points, float laserDepth, float hAngle, float vAngle)
{
	float realH = getLidarPitchAngle(laserDepth, hAngle)* M_PI / 180;
	float realV = getLidarYawAngle(laserDepth, vAngle)* M_PI / 180;
	if (realH > 300)
	{
		realH = realH - 360;
	}
	float z = laserDepth;
	float x = laserDepth * sin(realH)*cos(realV);
	float y = laserDepth * sin(-realV);
	double lightspeed = 299552816.0;
	double  realTof = laserDepth * 2 / lightspeed * 1000 * 1000 * 1000 * 1000;

	float minX = (x - 0.5) * 1000;
	float maxX = (x + 0.5) * 1000;
	float minY = (y - 0.5) * 1000;
	float maxY = (y + 0.5) * 1000;
	float minZ = (z - 0.5) * 1000;
	float maxZ = (z + 0.5) * 1000;

//	pcl::PointCloud<PointXYZRGBATI>::Ptr pcd_a = mPviewer->mCloudTi;
	pcl::PointCloud<pcl::PointXYZL>::Ptr validPois(new pcl::PointCloud<PointXYZL>);
//	neuvition::NeuvUnits validPoi;
	for (int i = 0; i < points.size(); i++)
	{
		NeuvUnit np = points.at(i);
		if (np.x > minX && np.x <maxX && np.y > minY && np.y < maxY && np.z > minZ && np.z < maxZ)
		{
			PointXYZL poi;
			poi.x = np.x * 0.001;
			poi.y = np.y* 0.001;
			poi.z = np.z* 0.001;
			poi.label = i;
			validPois->push_back(poi);
		}
		else
		{
			continue;
		}
	}
// 	pcl::PointXYZL nearPOi;
// 	GetRotateOrigin(points, -realH, -realV, nearPOi);

//  	// 建立kd-tree对象用来搜索 .
	pcl::search::KdTree<pcl::PointXYZL>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZL>);
	kdtree->setInputCloud(validPois);

	// Euclidean 聚类对象.
	pcl::EuclideanClusterExtraction<pcl::PointXYZL> clustering;//NeuvPclPoint
	// 设置聚类的最小值 2cm (small values may cause objects to be divided
	// in several clusters, whereas big values may join objects in a same cluster).
	clustering.setClusterTolerance(0.05);
	// 设置聚类的小点数和最大点云数
	clustering.setMinClusterSize(50);
	clustering.setMaxClusterSize(20000);
	std::vector<pcl::PointIndices> clusters;
	clustering.setSearchMethod(kdtree);
	clustering.setInputCloud(validPois);


	clustering.extract(clusters);
// 	clustering.setSearchMethod(kdtree);
// 	clustering.setInputCloud(validPois);

// 	float R = 2;								//设置搜索半径大小
// 	vector<int> pointIdxRadiusSearch;			//存储近邻索引
// 	vector<float> pointRadiusSquaredDistance;	//存储近邻对应的平方距离
// 
// 	// 建立kd-tree对象用来搜索 .
// 	pcl::search::KdTree<pcl::PointXYZL>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZL>);
// 	kdtree->setInputCloud(validPois);
// 	pcl::PointCloud<pcl::PointXYZL>::Ptr surfacePois(new pcl::PointCloud<PointXYZL>);
// 	if ( kdtree->radiusSearch(nearPOi, R, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0)
// 	{
// 		
// 		for (int  t = 0; t < pointIdxRadiusSearch.size(); t++)
// 		{
// 			int  idx = pointIdxRadiusSearch[t];
// 			surfacePois->push_back(validPois->at(idx));
// 		}
// 		// 建立kd-tree对象用来搜索 .
// 		pcl::search::KdTree<pcl::PointXYZL>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZL>);
// 		kdtree->setInputCloud(surfacePois);
// 
// 		clustering.setSearchMethod(kdtree);
// 		clustering.setInputCloud(surfacePois);
// 	}
// 	else
// 	{
// 
// 		clustering.setSearchMethod(kdtree);
// 		clustering.setInputCloud(validPois);
// 	}
// 
// 	clustering.extract(clusters);

	double sumX = 0, sumY = 0, sumZ = 0;
	//添加所有的点云到一个新的点云中
	//pcl::PointCloud<pcl::PointXYZI>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZI>);
	if (clusters.size() >= 1)
	{
		//int surfaceIndex = 
		int avgNum = clusters[0].indices.size();
		for (int i = 0; i < clusters[0].indices.size(); i++)
		{
			int index = clusters[0].indices[i];

			sumX += validPois->at(index).x;
			sumY += validPois->at(index).y;
			sumZ += validPois->at(index).z;
		}
		if (avgNum > 0)
		{

			NeuvPclPoint centerPoi;
			
			centerPoi.x = sumX / avgNum;
			centerPoi.y = sumY / avgNum;
			centerPoi.z = sumZ / avgNum;

			coreX = centerPoi.x;
			coreY = centerPoi.y;
			coreZ = centerPoi.z;

			std::cout << "centerPoi.x :" << centerPoi.x << ", centerPoi.y: " << centerPoi.y << ",centerPoi.z: " << centerPoi.z << std::endl;
			float minDis = 100;
			int minDisIndex = 0;

			for (int i = 0; i < validPois->size();i++)
			{
				if (validPois->at(i).x > centerPoi.x + 0.2  || validPois->at(i).x < centerPoi.x - 0.2)
					continue;
				if (validPois->at(i).y > centerPoi.y + 0.2 || validPois->at(i).y < centerPoi.y - 0.2)
					continue;
				if (validPois->at(i).z > centerPoi.z + 0.2 || validPois->at(i).z < centerPoi.z - 0.2)
					continue;
				float dis = (validPois->at(i).x - centerPoi.x)* (validPois->at(i).x - centerPoi.x) + (validPois->at(i).y - centerPoi.y)* (validPois->at(i).y - centerPoi.y) +
					(validPois->at(i).z - centerPoi.z)* (validPois->at(i).z - centerPoi.z);
				if (minDis > dis)
				{
					minDisIndex = i;
				}
			}
			NeuvUnit un = points.at(validPois->at(minDisIndex).label);
			mSurfaceCorPoints.push_back(un);

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
		
			mBofs << un.x << "    "<< un.y << "    " << un.z << "    " << un.tof << "    " <<un.col << "    " <<un.row << "    " << (int)un.intensity << "    " << (int)realTof << "\n";
			mBofs.close();
		}
	}
	else
	{
		std::cout << "get center valid point failed !!!!!!!!!!" << std::endl;
		return -1;
	}

	return 1;
}
bool CalibrationProcess::SurfaceCalibration()
{ 
	if (mSurfaceCorPoints.size() < 100)
	{
		std::cout << "There are too few samples for correction, please continue to collect samples" << std::endl;
		return 0;
	}
	int lidNum = 1;
	int devic_type = neuvition::get_device_type();
	//devic_type = neuvition::TITAN_M1_A;
	if (devic_type == neuvition::TITAN_M1_PRO || devic_type == neuvition::TITAN_M1_PRO_SL)
	{
		lidNum = 3;
	}
	else if (devic_type == neuvition::TITAN_P1_SL || devic_type == neuvition::TITAN_P1_SL1550)
	{
		lidNum = 4;
	}
	else
	{
		lidNum = 1;
	}
	std::vector<std::vector<NeuvUnit>>  pois;
	pois.resize(lidNum);
	for (int i = 0; i < mSurfaceCorPoints.size(); i++)
	{
		int tof = mSurfaceCorPoints[i].tof;
		int realTof = mSurfaceCorPoints[i].time_sec;
		int  intensity = (int)mSurfaceCorPoints[i].intensity;
		int lid = (int)mSurfaceCorPoints[i].lid - 1;
		int z = mSurfaceCorPoints[i].z;
// 		if (devic_type == neuvition::TITAN_P1_SL)
// 		{
// 
// 			if (tof > 60000 || realTof > 60000 || intensity > 100 || intensity < 5 || tof < 10000)
// 			{
// 				continue;
// 			}
// 		}
		if (devic_type == neuvition::TITAN_M1_A_SL)
		{
			mSurfaceCorPoints[i].row = 360;
		}
		int maxDiff = 10000;
		if (z > 10000)
		{
			maxDiff = 40000;
		}
		if (tof < 3000 || tof > 500000|| ((int)std::abs((tof - realTof)) > maxDiff))
		{
			continue;
		}
// 		if (devic_type == neuvition::TITAN_M1_A/* && tof > 60000*/)
// 		{
// 			if (tof < 60000)
// 			{
// 				if (mSurfaceCorPoints[i].row < mBordCutNum || mSurfaceCorPoints[i].row > (719 - mBordCutNum) || mSurfaceCorPoints[i].col < mBordCutNum || mSurfaceCorPoints[i].col > (1799 - mBordCutNum))
// 				{
// 					continue;
// 				}
// 			}
// 			else
// 			{
// 				if (mSurfaceCorPoints[i].row < 330 || mSurfaceCorPoints[i].row > 450 || mSurfaceCorPoints[i].col < 550 || mSurfaceCorPoints[i].col > 1000)
// 				{
// 					continue;
// 				}
// 			}
//
//		}
		pois[lid].push_back(mSurfaceCorPoints[i]);	
	}
	int poiNum = 0;
	neuvition::get_calibration_params(CAL_TOF_COR, tofCorEnabled, tofCorCoeffsMat);
	std::cout << "tofCorCoeffsMat   ----------=" << tofCorCoeffsMat.at<float>(4, 1) << tofCorCoeffsMat << std::endl;
	for (int i = 0; i< lidNum; i++)
	{
		int poiNum = pois[i].size();
		std::cout << "----------------CalculatFitCoeff poi num = " << poiNum << std::endl;
		if (poiNum < 100)
		{

			continue;
		}
		CalculatFitCoeff(poiNum, i, pois[i]);
	}
	
	return true;
}

void CalibrationProcess::ClearSurfaceData()
{
	mSurfaceCorPoints.clear();
}

void CalibrationProcess::SetOriginColRow(int col, int row)
{
	mOriginCol = col;
	mOriginRow = row;
}

float CalibrationProcess::getLidarYawAngle(float z, float ptzYaw) {
	const float fd = 0.0457;
	float x, d, lidarYaw;
	d = fd + z;
	x = d * tan(ptzYaw * P_A);
	lidarYaw = A_P * atan(x / z);
	return lidarYaw;
}

float CalibrationProcess::getLidarPitchAngle(float z, float ptzPitch) {
	const float fd = 0.0457;
	const float hd = 0.123;
	const float c = sqrtf(fd * fd + hd * hd);
	const float A = A_P * atan(hd / fd);
	float x, y, lx, ly;
	float OA, lidarPitch;
	x = c * cos((A - ptzPitch) * P_A);
	y = c * sin((A - ptzPitch) * P_A);
	lx = z - (x - fd);
	ly = hd - y;
	OA = A_P * atan(ly / lx);
	lidarPitch = ptzPitch + OA;
	return lidarPitch;
}

void CalibrationProcess::GetRotateOrigin(const neuvition::NeuvUnits& points, float horAngle, float vorAngle, pcl::PointXYZL& poi)
{
	float hFov = neuvition::get_hfov();
	float vFov = neuvition::get_vfov();
	
	int offsetCol = mOriginCol - 900;
	int offsetRow = mOriginRow - 360;
	float originColOffsetAngle = atan(tan(hFov / 2 * M_PI / 180) * offsetCol /900);
	float originRowOffsetAngle = atan(tan(vFov / 2 * M_PI / 180) * offsetRow / 900);

	int col = (tan(horAngle *M_PI / 180 + originColOffsetAngle) * 900) / tan(hFov / 2 * M_PI / 180) + 900;
	int row = (tan(vorAngle *M_PI / 180 + originRowOffsetAngle) * 360) / tan(hFov / 2 * M_PI / 180) + 360;


	int nearIndex = 0;
	int minDis = 1000;
	int minRowDis = 50;
	for (int i = 0; i <points.size(); i++)
	{
		int dis = (points.at(i).col - col)* (points.at(i).col - col) + (points.at(i).row - row)* (points.at(i).row - row);
		if (dis < minDis)
		{
			minDis = dis;
			nearIndex = i;
		}
	}
	poi.x =points.at(nearIndex).x * 0.001;
	poi.y =points.at(nearIndex).y * 0.001;
	poi.z =points.at(nearIndex).z * 0.001;
	poi.label = nearIndex;
}

void CalibrationProcess::AppendSurfaceCorPoint(neuvition::NeuvUnit poi)
{
	mSurfaceCorPoints.push_back(poi);
}

void CalibrationProcess::CalculatFitCoeff(int poiNum,int lid, neuvition::NeuvUnits pois)
{
	const int size = poiNum * 5;
	double *xd = new double[size];
	for (int i = 0; i < poiNum; i++)
	{
		*(xd + i) = pois[i].row;
		*(xd + i + poiNum) = pois[i].col;
		*(xd + i + poiNum * 2) = pois[i].intensity;

		*(xd + i + poiNum * 3) = pois[i].tof;
		*(xd + i + poiNum * 4) = pois[i].time_sec;

	}
	bool isSuccess = false;
	std::cout << "NlinfitSolveInitialize() = " << NlinfitSolveInitialize() << std::endl;
	if (NlinfitSolveInitialize()) {
		mwArray X(poiNum, 5, mxDOUBLE_CLASS);
		mwArray bt(11, 1, mxDOUBLE_CLASS);   //四元二次方程系数 f(x,y,z,w) = x^2 + y^2 + z^2 + w^2 + xy + xz + xw + yz + yw + zw + x + y + z + w + c;
		mwArray MSE(1, 1, mxDOUBLE_CLASS);   // 均方误差

		X.SetData(xd, size);
		//std::cout << X << std::endl;
		NlinfitSolve(2, bt, MSE, X);
	
		double btData[11] = { 0 };
		bt.GetData(btData, 11);


// 		bool tofCorEnabled;
// 		cv::Mat  tofCorCoeffsMat;
// 
// 		neuvition::get_calibration_params(CAL_TOF_COR, tofCorEnabled, tofCorCoeffsMat);
		int index = 0;
		for (int i = 0; i < 11; i++)
		{
			std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << btData[i] << "    " << std::endl;
			
			if (index == 3)
			{
				tofCorCoeffsMat.at<float>(lid, 3) = 0;
				index++;
			}
			if (index == 7)
			{
				tofCorCoeffsMat.at<float>(lid, 7) = 0;
				index++;
			}
			tofCorCoeffsMat.at<float>(lid, index) = btData[i];
			index++;
		}
		std::cout << "MSE: " << MSE  << " , tofCorCoeffsMat:" << tofCorCoeffsMat << std::endl;

		
	}
	delete xd;
}

void CalibrationProcess::updata()
{
	QString calibrationFilePath;
	calibrationFilePath = QApplication::applicationDirPath();
	calibrationFilePath.append("/calibration/").append("/c_config_params.json");
	tofCorEnabled = true;
	neuvition::set_calibration_params(CAL_TOF_COR, tofCorEnabled, tofCorCoeffsMat);
	neuvition::set_calibration_params_by_http_json("192.168.1.101", calibrationFilePath.toStdString().c_str());
}

void CalibrationProcess::updataScaleXY(float x, float y)
{
	tofCorCoeffsMat.setTo(0);
	neuvition::get_calibration_params(CAL_TOF_COR, tofCorEnabled, tofCorCoeffsMat);
	tofCorCoeffsMat.at<float>(4, 0) = x;
	tofCorCoeffsMat.at<float>(4, 1) = y;

	QString calibrationFilePath;
	calibrationFilePath = QApplication::applicationDirPath();
	calibrationFilePath.append("/calibration/").append("/c_config_params.json");
	tofCorEnabled = true;
	neuvition::set_calibration_params(CAL_TOF_COR, tofCorEnabled, tofCorCoeffsMat);
	neuvition::set_calibration_params_by_http_json("192.168.1.101", calibrationFilePath.toStdString().c_str());
}

void CalibrationProcess::SetBordCutNum(int num)
{
	mBordCutNum = num;
}
