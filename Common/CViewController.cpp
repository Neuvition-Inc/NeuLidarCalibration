#include "CViewController.h"

CViewController::CViewController()
{
}

CViewController::~CViewController()
{
}



void CViewController::initViewInfo(double dCenterX, double dCenterY, double dCenterZ, double dDisRadius)
{
	m_dCenterX = dCenterX;
	m_dCenterY = dCenterY;
	m_dCenterZ = dCenterZ;
	m_dDisRadius = dDisRadius;
}

st_PCL_CAMERA_POSITION CViewController::getTopView()
{
	/**
	俯视图：
	position: (center.x , center.y + disRadius , center.z )
	up: (0,0,1)
	lookat :  (center.x , center.y , center.z)
	**/

	st_PCL_CAMERA_POSITION  st_camera_pos;
	st_camera_pos.pos[0] = m_dCenterX;
	st_camera_pos.pos[1] = m_dCenterY + m_dDisRadius;
	st_camera_pos.pos[2] = m_dCenterZ;

	st_camera_pos.up[0] = 0;
	st_camera_pos.up[1] = 0;
	st_camera_pos.up[2] = 1;

	st_camera_pos.view[0] = m_dCenterX;
	st_camera_pos.view[1] = m_dCenterY;
	st_camera_pos.view[2] = m_dCenterZ;

	return st_camera_pos;

}

st_PCL_CAMERA_POSITION CViewController::getLeftView()
{
	/*
	左视图：
	position: (center.x + disRadius , center.y , center.z)
	up: (0,1,0)
	lookat :  (center.x , center.y , center.z)
	*/

	st_PCL_CAMERA_POSITION  st_camera_pos;
	st_camera_pos.pos[0] = m_dCenterX + m_dDisRadius;
	st_camera_pos.pos[1] = m_dCenterY;
	st_camera_pos.pos[2] = m_dCenterZ;

	st_camera_pos.up[0] = 0;
	st_camera_pos.up[1] = 1;
	st_camera_pos.up[2] = 0;

	st_camera_pos.view[0] = m_dCenterX;
	st_camera_pos.view[1] = m_dCenterY;
	st_camera_pos.view[2] = m_dCenterZ;

	return st_camera_pos;


}

st_PCL_CAMERA_POSITION CViewController::getFrontView()
{
	/*
	正视图：
	position: (center.x , center.y , center.z - disRadius)
	up: (0,1,0)
	lookat :  (center.x , center.y , center.z)
	*/

	st_PCL_CAMERA_POSITION  st_camera_pos;
	st_camera_pos.pos[0] = m_dCenterX ;
	st_camera_pos.pos[1] = m_dCenterY;
	st_camera_pos.pos[2] = m_dCenterZ - m_dDisRadius;

	st_camera_pos.up[0] = 0;
	st_camera_pos.up[1] = 1;
	st_camera_pos.up[2] = 0;

	st_camera_pos.view[0] = m_dCenterX;
	st_camera_pos.view[1] = m_dCenterY;
	st_camera_pos.view[2] = m_dCenterZ;

	return st_camera_pos;

}

void CViewController::getRotateAxisViews(
	Eigen::Vector3f normal, 
	Eigen::Vector3f center, 
	float fRadius, 
	Eigen::Vector3f up, 
	float fUnitAngle, 
	std::vector<st_PCL_CAMERA_POSITION>& vec_cam_positions
)
{
	double m_pi = 3.14159265358979;// 3.1415926;
	double nx = normal[0], ny = normal[1], nz = normal[2];
	double cx = center[0], cy = center[1], cz = center[2];
	double r = fRadius;

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
	std::vector<double> x, y, z;


	vec_cam_positions.clear();

	while (t < 360.0)
	{
		xi = cx + r * (ux_*cos(angle) + vx_ * sin(angle));
		yi = cy + r * (uy_*cos(angle) + vy_ * sin(angle));
		zi = cz + r * (uz_*cos(angle) + vz_ * sin(angle));
		x.push_back(xi);
		y.push_back(yi);
		z.push_back(zi);

		st_PCL_CAMERA_POSITION  campos;
		campos.pos[0] = xi;
		campos.pos[1] = yi;
		campos.pos[2] = zi;
		campos.up[0] = up[0];
		campos.up[1] = up[1];
		campos.up[2] = up[2];
		campos.view[0] = center[0];
		campos.view[1] = center[1];
		campos.view[2] = center[2];

		vec_cam_positions.push_back(campos);



		t = t + fUnitAngle;
		angle = (t / 180.0)*m_pi;
	}

}
