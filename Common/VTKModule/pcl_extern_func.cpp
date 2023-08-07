#include "pcl_extern_func.h"

vtkSmartPointer<vtkPolyData> createPlane(const pcl::ModelCoefficients& coefficients, float scale[2] /*= nullptr*/)
{
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();

	plane->SetNormal(coefficients.values[0], coefficients.values[1], coefficients.values[2]);
	double norm_sqr = coefficients.values[0] * coefficients.values[0]
		+ coefficients.values[1] * coefficients.values[1]
		+ coefficients.values[2] * coefficients.values[2];


	plane->Push(-coefficients.values[3] / sqrt(norm_sqr));
	plane->SetResolution(200, 200);
	plane->Update();

	double pt1[3], pt2[3], orig[3], center[3];
	plane->GetPoint1(pt1);
	plane->GetPoint2(pt2);
	plane->GetOrigin(orig);
	plane->GetCenter(center);

	double _pt1[3], _pt2[3];
	float scale1 = 3.0;
	float scale2 = 3.0;
	if (scale != nullptr)
	{
		scale1 = scale[0];
		scale2 = scale[1];
	}
	for (int i = 0; i < 3; i++) {
		_pt1[i] = scale1 * (pt1[i] - orig[i]);
		_pt2[i] = scale2 * (pt2[i] - orig[i]);
	}
	for (int i = 0; i < 3; ++i)
	{
		pt1[i] = orig[i] + _pt1[i];
		pt2[i] = orig[i] + _pt2[i];
	}
	plane->SetPoint1(pt1);
	plane->SetPoint2(pt2);



	//    //延长origin
	//    double _origin[3];
	//    for(int i=0; i<3;++i)
	//    {
	//        _origin[i] = scale*(orig[i]-pt1[i]);
	//    }
	//    for(int i=0; i<3;++i)
	//    {
	//        orig[i] = pt1[i] + _origin[i];
	//    }
	//    plane->SetOrigin(orig);

	plane->Update();
	return (plane->GetOutput());
}

vtkSmartPointer<vtkPolyData> createPlane(const pcl::ModelCoefficients &coefficients, double x, double y, double z, float scale[2] /*= nullptr*/)
{
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();


	double norm_sqr = 1.0 / (coefficients.values[0] * coefficients.values[0] +
		coefficients.values[1] * coefficients.values[1] +
		coefficients.values[2] * coefficients.values[2]);

	plane->SetNormal(coefficients.values[0], coefficients.values[1], coefficients.values[2]);
	double t = x * coefficients.values[0] + y * coefficients.values[1] + z * coefficients.values[2] + coefficients.values[3];
	x -= coefficients.values[0] * t * norm_sqr;
	y -= coefficients.values[1] * t * norm_sqr;
	z -= coefficients.values[2] * t * norm_sqr;

	plane->SetCenter(x, y, z);

	{
		double pt1[3], pt2[3], orig[3], center[3];
		plane->GetPoint1(pt1);
		plane->GetPoint2(pt2);
		plane->GetOrigin(orig);
		plane->GetCenter(center);

		float scale1 = 3.0;
		float scale2 = 3.0;
		if (scale != nullptr)
		{
			scale1 = scale[0];
			scale2 = scale[1];
		}
		//延长pt1,pt2. 延长origin到pt1连线的方向向量
		double _pt1[3], _pt2[3];
		for (int i = 0; i < 3; i++) {
			_pt1[i] = scale1 * (pt1[i] - orig[i]);
			_pt2[i] = scale2 * (pt2[i] - orig[i]);
		}
		//pt1相对于原坐标系下的坐标值
		for (int i = 0; i < 3; ++i)
		{
			pt1[i] = orig[i] + _pt1[i];
			pt2[i] = orig[i] + _pt2[i];
		}
		plane->SetPoint1(pt1);
		plane->SetPoint2(pt2);


		//        //延长origin
		//        double _origin[3];
		//        for(int i=0; i<3;++i)
		//        {
		//            _origin[i] = scale*(orig[i]-pt1[i]);
		//        }
		//        for(int i=0; i<3;++i)
		//        {
		//            orig[i] = pt1[i] + _origin[i];
		//        }
		//        plane->SetOrigin(orig);
	}
	plane->Update();

	return (plane->GetOutput());
}
