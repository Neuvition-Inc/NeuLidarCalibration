#include "CMatSubject.h"

CMatSubject::CMatSubject()
{
}

CMatSubject::~CMatSubject()
{
}

void CMatSubject::setData(cv::Mat mat)
{
	std::cout << "CMatSubject::setData  begin" << std::endl;
	if (!mat.empty())
	{
		mat.copyTo(m_MatData);
		std::cout << "CMatSubject::setData  Middern" << std::endl;
	}
	this->notify();
	std::cout << "CMatSubject::setData  end" << std::endl;
}

void CMatSubject::getData(cv::Mat& mat)
{
	if (!m_MatData.empty())
	{
		m_MatData.copyTo(mat);
	}
}
