#include "SerialPort.h"
#include "SerialPort.h"
#include "SerialPort.h"
#include <QDebug>
SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

}

SerialPort::SerialPort(QString SerialName)
{
    //设置串口名
    setSerialPortName(SerialName);
}

SerialPort::~SerialPort()
{
    this->m_SerialPort.close();
}

void SerialPort::setSerialPortName(QString SerialName)
{
    this->m_SerialPort.setPortName(SerialName);
    qDebug()<<"setSerialPortName"<<SerialName;
}

bool SerialPort::openSerialPort()
{
    if (m_SerialPort.isOpen())
    {
        m_SerialPort.clear();
        m_SerialPort.close();
    }
    if (!m_SerialPort.open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        qDebug() << m_SerialPort.portName() << QString::fromLocal8Bit("打开失败!") ;
        qDebug() << m_SerialPort.error() ;
        return false;
    }
    else
    {
        qDebug() << m_SerialPort.portName() << QString::fromLocal8Bit("打开成功!") ;
        //设置波特率
        m_SerialPort.setBaudRate(QSerialPort::Baud115200);
        //设置数据位数
        m_SerialPort.setDataBits(QSerialPort::Data8);
        //设置停止位
        m_SerialPort.setStopBits(QSerialPort::TwoStop);
        //设置奇偶校验
        m_SerialPort.setParity(QSerialPort::NoParity);
        //设置流控制
        m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "m_SerialPort.setBaudRate(QSerialPort::Baud115200); success" ;
    }
    return true;
}


//激光测距仪用
int SerialPort::openSerialPort_laser()
{
    if (m_SerialPort.isOpen())
    {
        m_SerialPort.clear();
        m_SerialPort.close();
    }
    if (!m_SerialPort.open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        qDebug() << m_SerialPort.portName() << "打开失败!" ;
        qDebug() << m_SerialPort.error() ;
        return false;
    }
    else
    {
        qDebug() << m_SerialPort.portName() << "打开成功!" ;
        m_SerialPort.setBaudRate(QSerialPort::Baud9600);
        m_SerialPort.setDataBits(QSerialPort::Data8);
        m_SerialPort.setParity(QSerialPort::NoParity);
        m_SerialPort.setStopBits(QSerialPort::OneStop);
        m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
        rbuf.clear();
        connect(&m_SerialPort, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
        return true;
    }
}

bool SerialPort::IsSerialOpen()
{
    return this->m_SerialPort.isOpen();
}

QByteArray SerialPort::ReceiveInfo()
{
    bool Readstatus = m_SerialPort.waitForReadyRead(50);
    if (Readstatus)
    {
        QByteArray Read_buf;
        Read_buf.append(m_SerialPort.readAll());//将读到的数据放入缓冲区
        //qDebug()<<"ReceiveInfo:"<<Read_buf.toHex(':');
        if (Read_buf.length() == 0)
        {
            qDebug() << "ERROR:Receive Szie = 0" << endl;
            return nullptr;
        }
        if (Read_buf.size()<8)
        {
            m_SerialPort.waitForReadyRead(50);
            Read_buf.append(m_SerialPort.readAll());
        }
        return Read_buf;
    }
    else
    {
        //qDebug() << "Read Fail" << endl;
        return nullptr;
    }
}

void SerialPort::SendInfo(QByteArray sendBuff)
{
    if (sendBuff.isEmpty())
    {
        qDebug() << "Write to serial: IsEmpty";
        return;
    }
    else
    {
        int ret = m_SerialPort.write(sendBuff);
       // qDebug() <<"Write to ret: "<< ret<<" sendBuff:"<< sendBuff.toHex(':');
    }
}


//激光测距仪用
void SerialPort::readyReadSlot()
{
    QByteArray buf;
    if (m_SerialPort.isOpen()) {
        buf = m_SerialPort.readAll();
        for (int i = 0; i < buf.size(); ++i)
        {
            rbuf.push_back(buf.at(i));
        }
        if (rbuf.size() >= 9) {
            emit dataReady();
        }
    }
}

//激光测距仪用
std::vector<char> SerialPort::getReadData()
{
    return rbuf;
}

//激光测距仪用
void SerialPort::crcCodeCheck(char crcdata)
{
    crcCode = crcCode ^ crcdata;
    for (int i = 0; i < 8; i++)
    {
        char Lb;
        Lb = crcCode & 1;
        crcCode = crcCode >> 1;
        crcCode = crcCode & 0x7fff;
        if (Lb) {
            crcCode = crcCode ^ 0xa001;
            crcCode = crcCode & 0xffff;
        }
    }
}

//激光测距仪用
void SerialPort::writeSerialport(char buf[])
{
    if (rbuf.size() > 0)
        rbuf.clear();
    char wbuf[8];
    if (m_SerialPort.isOpen()) {
        crcCode = 0xffff;
        for (int i = 0; i < 6; i++) {
            wbuf[i] = buf[i];
            crcCodeCheck(wbuf[i]);
        }
        wbuf[6] = crcCode & 0xff;
        wbuf[7] = crcCode / 0x100;
        m_SerialPort.write(wbuf, 8);
    }
}
