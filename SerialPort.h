#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    explicit SerialPort(QString SerialName);
    ~SerialPort();
    QStringList getAllSerialPortName();
    void setSerialPortName(QString SerialName);
    bool openSerialPort();
    int openSerialPort_laser();//º§π‚≤‚æ‡“«”√
    bool IsSerialOpen();
    QByteArray ReceiveInfo();
    void SendInfo(QByteArray sendBuff);


    std::vector<char> getReadData();//º§π‚≤‚æ‡“«”√
    void crcCodeCheck(char crcdata);//º§π‚≤‚æ‡“«”√
    void writeSerialport(char buf[]);//º§π‚≤‚æ‡“«”√
private:
    QSerialPort m_SerialPort;
    QString m_PTZSerialPortName;

    std::vector<char> rbuf;//º§π‚≤‚æ‡“«”√
    ushort crcCode = 0xffff;//º§π‚≤‚æ‡“«”√

private slots:
    void readyReadSlot();
signals:
    void dataReady();
};

#endif // SERIALPORT_H
