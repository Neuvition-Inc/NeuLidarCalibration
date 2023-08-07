#ifndef NEUVITION_AUTO_H
#define NEUVITION_AUTO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "SlideControl.h" //������
#include "LaserInstrumentControl.h"//��������
#include "PTZModule/CPTZBasePanel.h"//��̨��

class Neuvition_Auto : public QObject
{
    Q_OBJECT
public:
    explicit Neuvition_Auto(QObject *parent = nullptr);
    ~Neuvition_Auto();
    void demo();

public:
    SlideControl* m_pSlideControl;
    LaserInstrumentControl* m_pLaserControl;
    CPTZBasePanel* m_pPTZPanel;

signals:

};

#endif // NEUVITION_AUTO_H
