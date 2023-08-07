#include "SerialPort.h"
#include "SerialPort.h"
#include "SerialPort.h"
#include <QDebug>
SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

}

SerialPort::SerialPort(QString SerialName)
{
    //���ô�����
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
    if (!m_SerialPort.open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
    {
        qDebug() << m_SerialPort.portName() << QString::fromLocal8Bit("��ʧ��!") ;
        qDebug() << m_SerialPort.error() ;
        return false;
    }
    else
    {
        qDebug() << m_SerialPort.portName() << QString::fromLocal8Bit("�򿪳ɹ�!") ;
        //���ò�����
        m_SerialPort.setBaudRate(QSerialPort::Baud115200);
        //��������λ��
        m_SerialPort.setDataBits(QSerialPort::Data8);
        //����ֹͣλ
        m_SerialPort.setStopBits(QSerialPort::TwoStop);
        //������żУ��
        m_SerialPort.setParity(QSerialPort::NoParity);
        //����������
        m_SerialPort.setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "m_SerialPort.setBaudRate(QSerialPort::Baud115200); success" ;
    }
    return true;
}


//����������
int SerialPort::openSerialPort_laser()
{
    if (m_SerialPort.isOpen())
    {
        m_SerialPort.clear();
        m_SerialPort.close();
    }
    if (!m_SerialPort.open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
    {
        qDebug() << m_SerialPort.portName() << "��ʧ��!" ;
        qDebug() << m_SerialPort.error() ;
        return false;
    }
    else
    {
        qDebug() << m_SerialPort.portName() << "�򿪳ɹ�!" ;
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
        Read_buf.append(m_SerialPort.readAll());//�����������ݷ��뻺����
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


//����������
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

//����������
std::vector<char> SerialPort::getReadData()
{
    return rbuf;
}

//����������
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

//����������
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
