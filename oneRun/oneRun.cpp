// oneRun.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Monitor.h"

#include "coll.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *str = new char[100];
	int n = 0;
	//Coll  * coll = new Coll("collData.log");
	Coll  * coll = new Coll("loadData.log");
	while(true){
		n++;
		std::cout << "Please, enter: ";
		std::cin >> str;

		if (strstr(str , "coll")>0){
			coll->Run();
			//Coll->RunColl();
		}else if (strstr(str , "screen")>0){
			coll->RunScreen();
		}else if (strstr(str , "monitor")>0){
			Monitor * m = new Monitor("loadData.log", _T(".\\base.ini"));

			m->Run();
			delete m;
			//Coll->RunMonitor();
		}else if (strstr(str , "go")>0){
			coll->Runtest();
		}else if (strstr(str , "test")>0){
			char file[100];
			while(true){
				std::cout << "enter file tag: ";
				std::cin >> file;
				ClearSameTempleImg(file);
			}

			
		}

		
	}
	delete str;
	delete coll;
	return 0;
}

