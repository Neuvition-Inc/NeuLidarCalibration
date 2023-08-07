#include <QApplication>
#include <QTextCodec>
#include "test.h"
//#include "Neuvition_Auto.h"
#include "MainWin.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    qRegisterMetaType<LASER_DATA_T>("LASER_DATA_T");

    QApplication a(argc, argv);
#if MAIN_WIN 
    MainWin w;   
	w.RegFun();
	w.startThread();
    w.show();
#else
	test t;
	t.startThread();
	t.show();
	//NlinfitSolveInitialize();
#endif
    //Neuvition_Auto Nauto;
    //Nauto.demo();
    return a.exec();
}


