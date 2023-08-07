#include "CApplication.h"
#include <iostream>
#include "Common/crashdump.h"
#include "CLogRecord.h"

CApplication::CApplication(int& argc, char** argv)
	: QApplication(argc,argv)
{

}

bool CApplication::notify(QObject* receiver, QEvent* event)
{
	bool bDone = true;
	//try
	//{
	//	bDone = QApplication::notify(receiver, event);
	//}
	//catch (const std::exception& ex)
	//{
	//	//
	//	std::cout << "CApplication::notify std::exception& ex = " << ex.what() << std::endl;
	//	writeDumpFile();
	//}
	//catch (...)
	//{
	//	//
	//	std::cout << "CApplication::notify other catch = " << std::endl;
	//}

	__try
	{
		bDone = QApplication::notify(receiver, event);

	}
	__except (CrashDump::_WinExcept_UnhandledExceptionFilter(GetExceptionInformation()))
	{
		printf("***********************************e******");
		CLogRecord::getInstance()->write("CApplication::notify---: catch error");
	}


	return bDone;
	
}

