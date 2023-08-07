#include "SlideControl.h"
#include <QMessageBox>
#include <string>
#include <QDateTime>
#include <QThread>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>



void Sleep(int msec)
{

	QThread::msleep(msec);
    /*qint64 dieTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while( QDateTime::currentDateTime().toMSecsSinceEpoch()-dieTime < msec )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        QThread::msleep(1);
    }*/

}

//此函数将回传 unsigned integer 型态的 CRC 值。
unsigned int crc_chk(unsigned char* data, unsigned char length)
{
    int j;
    unsigned int reg_crc=0xFFFF;
    while( length-- )
    {
        reg_crc^= *data++;
        for (j=0; j<8; j++ )
        {
            if( reg_crc & 0x01 )
            { /*LSB(bit 0 ) = 1 */
                reg_crc = (reg_crc >> 1)^0xA001;
            }
            else
            {
                reg_crc = (reg_crc>>1);
            }
        }
    }
    return reg_crc;
}

QByteArray Hexstring2QByteArray(QString str)
{
    QByteArray Hexstr;
    bool ok;
    for (int i = 0; i < str.size(); i += 2)
    {
        Hexstr.append((char)str.mid(i, 2).toInt(&ok, 16));
    }
    return Hexstr;
}

QByteArray Int2QByteArray(int number)
{
    QByteArray abyte0;
    abyte0.resize(2);
    abyte0[0] = (uchar)  (0x000000ff & number);
    abyte0[1] = (uchar) ((0x0000ff00 & number) >> 8);
//    abyte0[2] = (uchar) ((0x00ff0000 & number) >> 16);
//    abyte0[3] = (uchar) ((0xff000000 & number) >> 24);
    return abyte0;
}

void Doc2Hex(unsigned int  value, char buffer[], int length)
{
    unsigned int i=(sizeof(unsigned  int)*2);
    unsigned int temp;
    int j=0;
    while(i--)
    {
        temp = (value&(0xf<<(4*i)))>>(4*i);
        if(temp>9)
            buffer[j] = 'A'+temp-10;
        else
            buffer[j] = '0'+temp;
        j++;
    }
    buffer[length] = '\0';
}

SlideControl::SlideControl(QObject *parent) : QObject(parent)
{
    CreateFile();//创造文件夹和ini文件
    this->m_SerialOpen = SerialInit();//串口初始化
    VariableInit();//变量初始化
    ConnectInit();//连接和槽
}

void SlideControl::VariableInit()
{
    //执行相关计数
    this->m_SeqListNum = 0;
    this->m_CurrentSeqPos = 0;
    //第一次读取时转为true
    this->m_LocationRead = false;
    //伺服开启标志位
    this->m_Servo_on = false;
    //INI
    this->m_MoveDirection = this->m_IniSet->value("Moving_Direction/Moving_Direction").toString();
    this->m_StartPUU = this->m_IniSet->value("Starting_Point/PUU").toInt();
    //location
    this->m_CLocation = 0;
    this->m_LSLocation = 0;
    this->m_ELocation = -1;
    this->m_IsRuning = true;
    //QTimer
    //this->m_ReadCLocation = new QTimer();

}

void SlideControl::ConnectInit()
{
    //执行
    connect(this,SIGNAL(m_Sig_SubPlusPlus()),this,SLOT(SubPlusPlus()));
    connect(this,SIGNAL(m_Sig_SubBecomeZero()),this,SLOT(SubBecomeZero()));
    //QTimer
    connect(this,SIGNAL(m_Sig_ReadPUU()),this,SLOT(ReadCLocation()));
}

void SlideControl::CreateFile()
{
    QDir dir(QCoreApplication::applicationDirPath()+"/SlideControl");
    if(!dir.exists())
    {
        dir.mkpath(QCoreApplication::applicationDirPath()+"/SlideControl");
    }
    this->m_IniFileName = QCoreApplication::applicationDirPath()+"/SlideControl/SlideControl.ini";
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
        this->m_IniSet->beginGroup("Length");
        this->m_IniSet->setValue("length",26);
        this->m_IniSet->endGroup();
        this->m_IniSet->beginGroup("Starting_Point");
        this->m_IniSet->setValue("PUU","0");
        this->m_IniSet->endGroup();
        this->m_IniSet->beginGroup("Moving_Direction");
        this->m_IniSet->setValue("Moving_Direction","-");
        this->m_IniSet->endGroup();
    }
}

#include <QTextCodec>
bool SlideControl::SerialInit()
{
    this->m_Serial.setSerialPortName(this->m_IniSet->value("Serial/name").toString());
    bool ret = this->m_Serial.openSerialPort();
    if(ret == false)
    {
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);
        QMessageBox::information(NULL,"failed",QString::fromLocal8Bit("滑轨串口打开失败"));
        codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        return false;
    }
    return true;
}

void SlideControl::runPRSet()
{
    if(this->m_SeqListNum == 0)
    {
        return;
    }
    //强制伺服开启
    if(this->m_Servo_on == false)
    {
        QString str = QString("0110023C0001020001");
        QByteArray send = Hexstring2QByteArray(str);
        unsigned int crc = crc_chk((unsigned char*)send.data(),send.length());
        send += Int2QByteArray(crc);
        //qDebug()<<"SendInfo: runPRSet this->m_Servo_on == false";
        //通信
        this->m_Serial.SendInfo(send);
        //Sleep(10);
        QByteArray rece = this->m_Serial.ReceiveInfo();
        this->m_Servo_on = true;
    }
    if(this->m_CurrentSeqPos < 0)
    {
        this->m_CurrentSeqPos = 0;
    }
    if(this->m_CurrentSeqPos >= this->m_SeqListNum)
    {
        this->m_CurrentSeqPos = 0;
        this->m_iIsOK++;
    }
    int PRsetNum = this->m_PRSets.at(this->m_CurrentSeqPos).PRNumber;
    QString str = QString("0110050E00010200%1").arg(PRsetNum,2,16,QLatin1Char('0'));
    QByteArray send = Hexstring2QByteArray(str);
    unsigned int crc = crc_chk((unsigned char*)send.data(),send.length());
    send += Int2QByteArray(crc);
    //qDebug()<<"SendInfo: runPRSet";
    //通信
    this->m_Serial.SendInfo(send);
    Sleep(10);
    QByteArray rece = this->m_Serial.ReceiveInfo();
    Sleep(10);
}

int SlideControl::readPUU()
{
    if(this->m_LocationRead == false)
    {
        //如果第一次，就往p0.017写入000
        QString str = QString("011000220001020000");
        QByteArray send = Hexstring2QByteArray(str);
        unsigned int crc = crc_chk((unsigned char*)send.data(),send.length());
        send += Int2QByteArray(crc);
        //通信
        this->m_Serial.SendInfo(send);
        Sleep(10);
        QByteArray rece = this->m_Serial.ReceiveInfo();
        Sleep(10);
        this->m_LocationRead = true;
    }
    QString str = "010300120002";
    QByteArray send = Hexstring2QByteArray(str);
    unsigned int crc = crc_chk((unsigned char*)send.data(),send.length());
    send += Int2QByteArray(crc);
    //通信
    this->m_Serial.SendInfo(send);
    //qDebug()<<"SendInfo： readPUU send:"<<send.toHex(':');
    Sleep(10);
    QByteArray rece = this->m_Serial.ReceiveInfo();
    Sleep(10);
    int PUU_int = rece.at(4) & 0x000000FF;
    PUU_int |= (rece.at(3) << 8) & 0x0000FF00;
    PUU_int |= (rece.at(6) << 16) & 0x00FF0000;
    PUU_int |= (rece.at(5) << 24) & 0xFF000000;
    //qDebug()<<"SendInfo： readPUU :"<<PUU_int;
    return PUU_int;//未转换,最好不直接使用
}

float SlideControl::readLocationInfo()
{
    int PUU_int = readPUU();
    float location = -(PUU_int - this->m_StartPUU)/1000.0; // 初始位置是0,移动放向为负方向 1 000 PUU = 1 mm
    if(this->m_bGetDistance)
        this->m_fGetDistance(location);
    return location;
}

bool SlideControl::chooseFileRun()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    this->m_RunFileName = QFileDialog::getOpenFileName(
                NULL, QString::fromLocal8Bit("选择要执行的文件"),
                QCoreApplication::applicationDirPath()+"/SlideControl",
                "*.txt");
    if(m_RunFileName.isEmpty())
    {
        codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        return false;
    }
    QFile file(this->m_RunFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data;
    QStringList data_list;

    //判断文件是否打开成功
    if(file.isOpen())
    {
        if(file.size() != 0)//文件有数据
        {
            //this->m_AllPRSets.clear();
            this->m_PRSets.clear();
            this->m_CurrentSeqPos = 0;
            this->m_SeqListNum = 0;
            //this->m_SeqListModel->removeRows(0,this->m_SeqListModel->rowCount());
            //一行一行一直读，直至读取失败
            while(!file.atEnd())
            {
                // readAll();全部取出来
                data = file.readLine();//读取一行存到data里
                data_list = data.split(",");
                if(data_list.size() != 9)
                {
                    QMessageBox::information(NULL,QString::fromLocal8Bit("failed"),QString::fromLocal8Bit("执行文件数据错误"));
                    codec = QTextCodec::codecForName("UTF-8");
                    QTextCodec::setCodecForLocale(codec);
                    return false;
                }
                //写入伺服系统，确保该PR点正确
                QByteArray send_data = Hexstring2QByteArray(data_list[0]);
                //qDebug()<<"SendInfo: chooseFileRun";
                this->m_Serial.SendInfo(send_data);
                Sleep(10);
                //保存信息到结构体
                PRSET_T set;
                set.PRInstructions = data_list[0];
                set.PRNumber = data_list[1].toInt();
                set.PRCMD = data_list[2];
                set.PRCMD_int = data_list[3].toInt();
                set.PRACC = data_list[4].toFloat();
                set.PRDEC = data_list[5].toFloat();
                set.PRSPD = data_list[6].toInt();
                set.PRDLY = data_list[7].toInt();
                set.PRPUU = data_list[8].toInt();
                this->m_PRSets.push_back(set);
                //总数
                ++this->m_SeqListNum;

            }
            // 读取完后关闭文件
            file.close();
            codec = QTextCodec::codecForName("UTF-8");
            QTextCodec::setCodecForLocale(codec);
            return true;
        }
    }
    else
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("failed"),QString::fromLocal8Bit("执行文件打开失败"));
        codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        return false;
    }
    return false;
}

bool SlideControl::chooseFileRun(QString filename)
{
    this->m_RunFileName = filename;
    if(m_RunFileName.isEmpty())
    {
        return false;
    }
    QFile file(this->m_RunFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data;
    QStringList data_list;

    //判断文件是否打开成功
    if(file.isOpen())
    {
        if(file.size() != 0)//文件有数据
        {
            //this->m_AllPRSets.clear();
            this->m_PRSets.clear();
            this->m_CurrentSeqPos = 0;
            this->m_SeqListNum = 0;
            //this->m_SeqListModel->removeRows(0,this->m_SeqListModel->rowCount());
            //一行一行一直读，直至读取失败
            while(!file.atEnd())
            {
                // readAll();全部取出来
                data = file.readLine();//读取一行存到data里
                data_list = data.split(",");
                if(data_list.size() != 9)
                {
                    QMessageBox::information(NULL,QString::fromLocal8Bit("failed"),QString::fromLocal8Bit("执行文件数据错误"));
                    return false;
                }
                //写入伺服系统，确保该PR点正确
                QByteArray send_data = Hexstring2QByteArray(data_list[0]);
                //qDebug()<<"SendInfo: chooseFileRun(QString filename)";
                this->m_Serial.SendInfo(send_data);
                Sleep(10);
                //保存信息到结构体
                PRSET_T set;
                set.PRInstructions = data_list[0];
                set.PRNumber = data_list[1].toInt();
                set.PRCMD = data_list[2];
                set.PRCMD_int = data_list[3].toInt();
                set.PRACC = data_list[4].toFloat();
                set.PRDEC = data_list[5].toFloat();
                set.PRSPD = data_list[6].toInt();
                set.PRDLY = data_list[7].toInt();
                set.PRPUU = data_list[8].toInt();
                this->m_PRSets.push_back(set);
                //总数
                ++this->m_SeqListNum;

            }
            // 读取完后关闭文件
            file.close();
            return true;
        }
        else
        {
            QMessageBox::information(NULL,QString::fromLocal8Bit("failed"),QString::fromLocal8Bit("执行文件 file.size() == 0"));
            return false;
        }
    }
    else
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("failed"),QString::fromLocal8Bit("执行文件打开失败"));
        return false;
    }
    return false;
}

void SlideControl::startThread()
{
    if(m_SerialOpen)
    {
        boost::thread createThread = boost::thread(boost::bind(&SlideControl::threadEmitSig, this));
        createThread.detach();
        //this->m_ReadCLocation->start(900);
    }
}

bool SlideControl::getISRuning()
{
    return this->m_IsRuning;
}

QString SlideControl::getFileName()
{
    return this->m_RunFileName;
}

bool SlideControl::getSerialOpen()
{
    return this->m_SerialOpen;
}

int SlideControl::getiIsOK()
{
    return this->m_iIsOK;
}

void SlideControl::setiIsOK(int iIsOK)
{
    this->m_iIsOK = iIsOK;
}

void SlideControl::Slide_RegFun_Runing(pSLideFun_Runing fun)
{
    this->m_fGetisRuning = fun;
    this->m_bGetisRuning = true;
}

void SlideControl::Slide_RegFun_Distance(pSLideFun_Distance fun)
{
    this->m_fGetDistance = fun;
    this->m_bGetDistance = true;
}

QVector<PRSET_T> SlideControl::getPRSet()
{
    return this->m_PRSets;
}

int SlideControl::getSeqListNum()
{
    return this->m_SeqListNum;
}

int SlideControl::getCurrentSeqPos()
{
    return this->m_CurrentSeqPos;
}

void SlideControl::SubBecomeZero()
{
    this->m_CurrentSeqPos = 0;

}

void SlideControl::SubPlusPlus()
{
    runPRSet();
    this->m_CurrentSeqPos++;
}

void SlideControl::ReadCLocation()
{
    this->m_LSLocation = this->m_CLocation;
//	qDebug() << "m_LSLocation:" << m_LSLocation << " m_CLocation:" << m_CLocation;
    this->m_CLocation = readLocationInfo()*1000;
    if(m_CLocation == m_LSLocation || m_CLocation == m_LSLocation+1 || m_CLocation == m_LSLocation-1)
    {
        this->m_IsRuning = false;
    }
    else
    {
        this->m_IsRuning = true;
    }
    if(this->m_bGetisRuning)
        this->m_fGetisRuning(this->m_IsRuning);
    //qDebug()<<"SlideControl::ReadCLocation():m_IsRuning:"<<m_IsRuning<<"  m_bGetisRuning:m_bGetisRuning:"<<m_bGetisRuning;
}

void SlideControl::threadEmitSig()
{
	while (1)
	{
		emit m_Sig_ReadPUU();
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
	}
}

