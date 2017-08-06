#ifndef Monitor_H_
#define Monitor_H_

#include <afxwin.h>
#include <cv.h>
#include <highgui.h>
#include "Client.h"
#include "block.h"
#include "common.h"


typedef struct _userInfo {
	int peng; // 发生碰牌 序列
	bool ishu; // 是否已经胡牌
	int type; // 二进制 纪录 碰 杠 属性
	int val;  //得分
	int Class; // 0 万,1 条,2 筒

	bool  isDiffNum;
}userInfo;

class Monitor
{
public:
	Monitor(const char *RootData ,CString Base );
	~Monitor();
	void Run();

private:
	void LoadRootData(const char * RootData );
	void Start( int & n,int &lastN);
	bool FindSelfBegin();
	bool FindSelf(int & lastn);
	IplImage *ScreenImg;
	vector <Block*> BlockList ;
	client * Client;
	LPCTSTR BaseName;


	void StartRun( int & n,int &lastN );
	bool FindBegin1();
	bool FindBegin2();
	//void FindBegin3();
	bool FindOut();
	bool FindSee();

	userInfo userTempInfoArr[4];

	void InitUserTempInfoArr();
	bool checkGang(vector <Coordinate>  C);

	int BaseNum;
	int Coll;

	

};
#endif