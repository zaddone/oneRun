#ifndef Collection_H_
#define Collection_H_
#include "common.h"


#include "coordRange.h"
#include "Client.h"




class Collection
{
public:
	Collection(const char *BaseFile ,CString MonitorFile );

	void Init();
	void Test(int CoordNum,char * file);
	void RunColl();
	void RunScreen();
	void RunMonitor();
	~Collection();
	

	
private:
	vector <CoordRange*> coordRangeList ;
	bool ScreenSave(const char * file);
	IplImage *imgB;
	IplImage *imgA;
	void LoadData(const char * LoadTmpFile );
	void Monitor(bool & Self );

	client * Client;
	int sep;
	LPCTSTR BaseName;
};
#endif