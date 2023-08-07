#include "CMatObserver.h"
#include "CMatSubject.h"

#include <QDateTime>
#include <QDir>
#define _Prefix_Record_AVI_ "neu_record_"

#include "../CGlobalSignalSlot.h"

CMatObserver::CMatObserver(Subject* pSubject)
{
	m_pSubject = pSubject;
	m_pSubject->attach(this);
}

CMatObserver::~CMatObserver()
{
	
	if (nullptr != m_pSubject)
	{
		m_pSubject->detach(this);
		//delete m_pSubject;
		//m_pSubject = nullptr;
	}
}

void CMatObserver::update(Subject* pSubject)
{
	static int nPrintIndex = 0;
	nPrintIndex++;
	bool bPrint = false;
	if (nPrintIndex % 100 == 1)
	{
		bPrint = true;
	}
	if (bPrint)
	{
		std::cout << "************************CMatObserver::update(Subject* pSubject)  pSubject =  " << pSubject << std::endl;
	}
	
	if (nullptr == pSubject)
	{
		return;
	}

	CMatSubject* pMatSubject = static_cast<CMatSubject*>(pSubject);
	cv::Mat mat;
	pMatSubject->getData(mat);

	m_nCol = mat.cols;
	m_nRow = mat.rows;





	CGlobalSignalSlot::getInstance()->emit sigMat(mat);

	//保存jpg
	if (bPrint)
	{
		std::cout << "************************CMatObserver::update(Subject* pSubject)  m_bSaveJpg =  " << m_bSaveJpg << std::endl;
	}
	if (m_bSaveJpg)
	{
		bool bRet = false;
		if (!mat.empty())
		{
			bRet = cv::imwrite(m_sJpgPath, mat);
		}
		m_bSaveJpg = false;
		std::cout << "save jpg result nErrorCode =  " << !bRet << " =====" << std::endl;

	}
	
	//保存avi
	if (m_bOpenVideoWriter)
	{
		QMutexLocker   locker(&m_mutex);
		if (m_pVideoWriter)
		{
			m_pVideoWriter->write(mat);
		}
		
	}
}

void CMatObserver::saveJpg2Local(QString sDir, QString sFileName)
{
	QDir dir;
	if (!dir.exists(sDir))
	{
		dir.mkpath(sDir);
	}

	QString sPath = sDir + "/" + sFileName;
	m_sJpgPath = sPath.toLocal8Bit().toStdString();
	m_bSaveJpg = true;
}


bool CMatObserver::openVideoWriter(QString sDir, QString sFileName)
{
	m_pVideoWriter = new cv::VideoWriter;
	int nFps = 10;
	int cols = m_nCol;
	int rows = m_nRow;

	QDir dir;
	if (!dir.exists(sDir))
	{
		dir.mkpath(sDir);
	}


	QString sFilePath = sDir + "/" + sFileName;
	bool isOpened = m_pVideoWriter->open(sFilePath.toLocal8Bit().toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), nFps, cv::Size(cols, rows), true);
	m_bOpenVideoWriter = isOpened;

	return m_bOpenVideoWriter;
}



bool CMatObserver::closeVideoWriter()
{
	QMutexLocker   locker(&m_mutex);
	if (m_pVideoWriter)
	{
		if (m_pVideoWriter->isOpened()) {
			m_pVideoWriter->release();
		}
		delete m_pVideoWriter;
		m_pVideoWriter = nullptr;

	}

	return true;
}


