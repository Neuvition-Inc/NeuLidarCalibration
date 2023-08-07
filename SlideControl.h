#ifndef SLIDECONTROL_H
#define SLIDECONTROL_H

#include <QObject>
#include <QStandardItemModel>
#include <QSettings>
#include "SerialPort.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

typedef struct PRset
{
    int PRNumber;//PR号
    QString PRCMD;//相对位置或绝对位置
    int PRCMD_int;
    float PRACC;//加速时间
    float PRDEC;//减速时间
    int PRSPD;//速度索引
    int PRDLY;//延时时间
    int PRPUU;//数据
    QString PRInstructions;//指令
}PRSET_T;//用于列表显示

typedef void(*pSLideFun_Runing)(bool isRuning);
typedef void(*pSLideFun_Distance)(float distance);


class SlideControl : public QObject
{
    Q_OBJECT
public:
    explicit SlideControl(QObject *parent = nullptr);
    //读取位置信息 
    float readLocationInfo();//转换成单位m
    //选择要执行的PR设置的文件
    bool chooseFileRun();
    bool chooseFileRun(QString filename);
    void startThread();

public:
    QVector<PRSET_T> getPRSet();
    int getSeqListNum();
    int getCurrentSeqPos();
    //判断是否在运行
    bool getISRuning();
    //filename
    QString getFileName();
    //Serial
    bool getSerialOpen();
    //
    int getiIsOK();
    void setiIsOK(int iIsOK = 0);

    void Slide_RegFun_Runing(pSLideFun_Runing fun);
    void Slide_RegFun_Distance(pSLideFun_Distance fun);

private:
    void VariableInit();
    void ConnectInit();
    void CreateFile();
    bool SerialInit();
    int readPUU();//获取伺服器里面的PUU数值
    //执行PR
    void runPRSet();

private slots:
    //设置执行顺序
    void SubBecomeZero();//当前PR执行顺序下标变为零
    void SubPlusPlus();//当前PR执行,PR顺序下标++

    //void runPRSet();//当前PR执行,下标++

    //QTimer timeout
    void ReadCLocation();//当前位置获取
	void threadEmitSig();

Q_SIGNALS:
    void m_Sig_SubBecomeZero();//当前PR执行顺序下标变为零
    void m_Sig_SubPlusPlus();//当前PR执行顺序下标++
	void m_Sig_ReadPUU();

private:
    SerialPort m_Serial;
    bool m_SerialOpen;
    QVector<PRSET_T> m_PRSets;
    int m_SeqListNum;//执行列表的总数
    int m_CurrentSeqPos;//当前执行的位置
    bool m_LocationRead;//如果第一次，就往p0.017写入000
    QString m_IniFileName;//INI文件名
    QString m_RunFileName;//PR设置的文件名
    QSettings *m_IniSet;//INI文件

    int m_ELocation;//预期位置
    int m_CLocation;//当前位置
    int m_LSLocation;//上一秒位置  用来和当前位置判断，如果相同，就代表停止，如果不同，就代表电机还在运转
    bool m_IsRuning;//判断是否在运行
    //QTimer *m_ReadCLocation;//当前位置获取

    int m_StartPUU;//起点;
    QString m_MoveDirection;//移动方向
    bool m_Servo_on;//伺服开启标志位

    pSLideFun_Runing m_fGetisRuning;//函数指针
    bool m_bGetisRuning = false;//函数指针标志位

    pSLideFun_Distance m_fGetDistance;//函数指针
    bool m_bGetDistance = false;//函数指针标志位

    int m_iIsOK = 0;//判断跑完的次数


};

#endif // SLIDECONTROL_H
