#include "LaserInstrumentControl.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include <QTextCodec>

extern boost::mutex DistanceMutex;//读取距离
extern boost::condition DistanceCond;//读取距离


LaserInstrumentControl::LaserInstrumentControl(QObject *parent) : QObject(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    CreateFile();
    SerialInit();
    VariableInit();
    ConnectInit();
    codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}

void LaserInstrumentControl::CreateFile()
{
    QDir dir(QCoreApplication::applicationDirPath()+"/LaserInstrumentControl");
    if(!dir.exists())
    {
        dir.mkpath(QCoreApplication::applicationDirPath()+"/LaserInstrumentControl");
    }
    this->m_IniFileName = QCoreApplication::applicationDirPath()+"/LaserInstrumentControl/LaserInstrumentControl.ini";
    this->m_IniSet = new QSettings(m_IniFileName,QSettings::IniFormat);
    QFileInfo fileinfo(this->m_IniFileName);
    if(fileinfo.size() == 0)
    {
        QFile file(m_IniFileName);
        file.open(QIODevice::WriteOnly|QIODevice::Append);
        file.close();
        this->m_IniSet->beginGroup("Serial");
        this->m_IniSet->setValue("name","");
        this->m_IniSet->endGroup();
        this->m_IniSet->beginGroup("NormallyON");
        this->m_IniSet->setValue("NormallyON","yes");
        this->m_IniSet->endGroup();
    }
}

void LaserInstrumentControl::VariableInit()
{
    if(this->m_IniSet->value("NormallyON/NormallyON").toString() == "yes")
    {
       m_Wdata[3] = 0x52;
       this->m_NormallyON = true;
    }
    else
    {
        this->m_NormallyON = false;
    }
}

void LaserInstrumentControl::ConnectInit()
{
    connect(&this->m_Serial, SIGNAL(dataReady()), this, SLOT(readySerialData()));
}

bool LaserInstrumentControl::SerialInit()
{
    //this->m_Serial = new SerialPort();
    this->m_Serial.setSerialPortName(this->m_IniSet->value("Serial/name").toString());
    bool ret = this->m_Serial.openSerialPort_laser();
    if(ret == false)
    {
        QMessageBox::information(NULL,"failed",this->m_IniSet->value("Serial/name").toString()+"测距仪串口打开失败");
        return false;
    }
    return true;
}

void LaserInstrumentControl::threadReadData()
{
	if (this->m_Serial.IsSerialOpen())
	{
		std::vector<char> rdata = this->m_Serial.getReadData();
		QString res;
		for (int i = 0; i < rdata.size(); ++i)
		{
			res += QString::number(rdata[i], 16);
			res += QString(" ");
		}
		LASER_DATA_T laserData;
		int id = rdata[0];
		//int distance = rdata[3] * 256 * 256 * 256 + rdata[4] * 256 * 256 + rdata[5] * 256 + rdata[6];
		int distance = (rdata[3] << 24) & 0xFF000000;
		distance |= (rdata[4] << 16) & 0x00FF0000;
		distance |= (rdata[5] << 8) & 0x0000FF00;
		distance |= rdata[6] & 0x000000FF;
		if (id == 18)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
			QThread::msleep(100);
			getDistanceByID(0x6E);
		}
		else if (id == 110)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
			QThread::msleep(100);
			getDistanceByID(0x6F);
		}
		else if (id == 111)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
		}
		if (this->m_bGetLaserData)
			this->m_fGetLaserData(laserData);
		emit m_sigUpdatedLaserData(laserData);
		QThread::msleep(100);
		DistanceCond.notify_one();
	}
	else
	{
		LASER_DATA_T laserData;
		laserData.id = -1;
		laserData.distance = -1;
		if (this->m_bGetLaserData)
			this->m_fGetLaserData(laserData);
		emit m_sigUpdatedLaserData(laserData);
		QThread::msleep(100);
		DistanceCond.notify_one();
		//ui->label_this->m_Serial.setText(QString("COM3 open failed!!!!"));
	}
}

void LaserInstrumentControl::readySerialData()
{
	qDebug() << "readySerialData";
	//boost::thread createThread = boost::thread(boost::bind(&LaserInstrumentControl::threadReadData, this));
	//createThread.detach();
	if (this->m_Serial.IsSerialOpen())
	{
		std::vector<char> rdata = this->m_Serial.getReadData();
		QString res;
		for (int i = 0; i < rdata.size(); ++i)
		{
			res += QString::number(rdata[i], 16);
			res += QString(" ");
		}
		LASER_DATA_T laserData;
		int id = rdata[0];
		//int distance = rdata[3] * 256 * 256 * 256 + rdata[4] * 256 * 256 + rdata[5] * 256 + rdata[6];
		int distance = (rdata[3] << 24) & 0xFF000000;
		distance |= (rdata[4] << 16) & 0x00FF0000;
		distance |= (rdata[5] << 8) & 0x0000FF00;
		distance |= rdata[6] & 0x000000FF;
		if (id == 18)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
			QThread::msleep(100);
			//getDistanceByID(0x6E);
			qDebug() << "DistanceCond.getDistanceByID(0x6E);";
		}
		else if (id == 110)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
			QThread::msleep(100);
			//getDistanceByID(0x6F);
			qDebug() << "DistanceCond.notify_one();";
			DistanceCond.notify_one();
			qDebug() << "DistanceCond.getDistanceByID(0x6E);";
		}
		else if (id == 111)
		{
			laserData.id = id;
			laserData.distance = distance / 10.0;
			QThread::msleep(100);
			
		}
		if (this->m_bGetLaserData)
			this->m_fGetLaserData(laserData);
		emit m_sigUpdatedLaserData(laserData);
	}
	else
	{
		LASER_DATA_T laserData;
		laserData.id = -1;
		laserData.distance = -1;
		if (this->m_bGetLaserData)
			this->m_fGetLaserData(laserData);
		emit m_sigUpdatedLaserData(laserData);
		QThread::msleep(100);
		DistanceCond.notify_one();
		//ui->label_this->m_Serial.setText(QString("COM3 open failed!!!!"));
	}
	
}

void LaserInstrumentControl::getDistanceByID(int id)
{
    qDebug()<<"getDistanceByID";
    if (this->m_Serial.IsSerialOpen())
    {
        this->m_Wdata[0] = id;
        qDebug()<<"getDistanceByID:id"<<id;
        this->m_Serial.writeSerialport(this->m_Wdata);
    }
}

void LaserInstrumentControl::Laser_RegFun(pLaserFun fun)
{
    this->m_fGetLaserData = fun;
    this->m_bGetLaserData = true;
}
