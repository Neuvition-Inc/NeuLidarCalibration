#pragma once

#include <QApplication>

class CApplication : public QApplication
{
	
public:
	CApplication(int& argc, char** argv);
	
	bool notify(QObject*  receiver, QEvent* event);
};
