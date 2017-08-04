#ifndef Coll_H_
#define Coll_H_
#include "common.h"
#include "block.h"
class Coll
{
public:
	Coll(const char *BaseFile);
	void Run();
	IplImage *ScreenImg;
	void RunScreen();
	bool ScreenSave(const char * file);
	void Runtest();
	void Test(int tag);
private:
	vector <Block*> BlockList ;
	void LoadRootData(const char * RootData );
};

#endif