#pragma once

#include "Subject.h"

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>


class CMatSubject : public Subject
{


public:
	CMatSubject();
	~CMatSubject();

	void setData(cv::Mat mat);
	void getData(cv::Mat& mat);

private:
	cv::Mat  m_MatData;

	
};
