#pragma once

#include "Subject.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>

#include <QString>
#include <QMutex>
#include <QMutexLocker>

class CMatObserver : public Observer
{

public:
	CMatObserver(Subject* pSubject);
	~CMatObserver();

	void update(Subject* pSubject);
	void saveJpg2Local(QString sDir, QString sFileName);

	//±£´æavi
	bool  openVideoWriter(QString sDir,QString sFileName);
	bool  closeVideoWriter();


private:

	Subject*  m_pSubject = nullptr;

	int m_nRow = 640;
	int m_nCol = 360;

	//jpg
	std::string  m_sJpgPath;
	bool    m_bSaveJpg = false;

	//avi
	cv::VideoWriter * m_pVideoWriter = nullptr;
	bool          m_bOpenVideoWriter = false;

	QMutex        m_mutex;
	
};
