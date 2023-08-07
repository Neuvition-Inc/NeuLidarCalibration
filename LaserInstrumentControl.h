#ifndef LASERINSTRUMENTCONTROL_H
#define LASERINSTRUMENTCONTROL_H

#include <QObject>
#include <QSettings>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include "SerialPort.h"
typedef struct laser_data
{
    int id;
    float distance;
}LASER_DATA_T;

typedef void(*pLaserFun)(LASER_DATA_T laser_data);


class LaserInstrumentControl : public QObject
{
    Q_OBJECT
public:
    explicit LaserInstrumentControl(QObject *parent = nullptr);

    void getDistanceByID(int id);
    void Laser_RegFun(pLaserFun fun);
private:
    void CreateFile();
    void VariableInit();
    void ConnectInit();
    bool SerialInit();
	void threadReadData();
private:
    SerialPort m_Serial;
    char m_LaserID[3] = {0x12,0x6E,0x6F};
    char m_Wdata[8] = { 0x12, 0x03, 0x00, 0x50,0x00, 0x02,  0x00, 0x00 };
    int m_LaserIndex;

    QString m_IniFileName;//INI文件名
    QSettings *m_IniSet;//INI文件
    bool m_NormallyON;//是否常亮

    pLaserFun m_fGetLaserData;//函数指针
    bool m_bGetLaserData = false;//函数指针的标志位

    //LASER_DATA_T m_LaserData[3];
private slots:
    void readySerialData();
signals:
    void m_sigUpdatedLaserData(LASER_DATA_T m_LaserData);//数据已经更新

};

#endif // LASERINSTRUMENTCONTROL_H
