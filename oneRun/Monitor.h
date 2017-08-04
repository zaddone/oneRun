#ifndef Monitor_H_
#define Monitor_H_

#include <afxwin.h>
#include <cv.h>
#include <highgui.h>
#include "Client.h"
#include "block.h"
#include "common.h"


typedef struct _userInfo {
	int peng; // �������� ����
	bool ishu; // �Ƿ��Ѿ�����
	int type; // ������ ��¼ �� �� ����
	int val;  //�÷�
	int Class; // 0 ��,1 ��,2 Ͳ

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
	bool FindSelfBegin( );
	bool FindSelf(int & lastn);
	IplImage *ScreenImg;
	vector <Block*> BlockList ;
	client * Client;
	LPCTSTR BaseName;

	userInfo userTempInfoArr[4];

	void InitUserTempInfoArr();
	bool checkGang(vector <Coordinate>  C);

	int BaseNum;
	int Coll;

	

};
#endif