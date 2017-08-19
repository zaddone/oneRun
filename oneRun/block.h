#ifndef Block_H_
#define Block_H_

#include "common.h"


#include "templeImg.h"
#include "Coordinate.h"





class Block
{
public:
	//Block();
	Block(CvRect r,int id,vector <TempleImg*> T,int coll);
	Block(const char *data,CvPoint po,int coll);
	~Block();
	int FindOne(IplImage * src,const bool isT=false);
	int FindChildOne(IplImage * src,const bool isT=false);
	int FindArr(IplImage * src,const bool isT = false );
	int FindSep(IplImage * src,const bool isT = false );
	void collTemple(IplImage * src);
	//int FindArrButton(IplImage * src,const bool isT = false );

	int GetDataNum();
	int WaitEvent();
	//bool FindSelfBegin(IplImage * src);
	//bool FindSelf(IplImage * src);
	void SendToServer(Sendback func, LPVOID lpParamter,const int k);
	void SendToServerNum(int num,Sendback func, LPVOID lpParamter,const int k);
	//int UpRectArr(IplImage * src,const int sep);
	int ID;
	vector <Block *> Child;
	vector <Coordinate>  Coord;
	void ClearCoord();
	friend void CallBackEvent(LPVOID handle,char *data);
	friend void CallBackEventArr(LPVOID handle,char *data);
	bool ClickCoordinate(int _v,int num);
	bool ClickCoordinateT(int _v,int num);
	bool PerceptionTempleToSame(IplImage * src,const int tag);
	bool GetNumberList(IplImage * src,const int tag) ;
	void GetNumberList_test(IplImage * src,const int tag) ;
	int IsArr;
	int Sep;
	bool isUp;
	int backVal;
	int Coll;
	int CallBackVal;
private:
	void LoadToTempleList(char * Filepath );

	
	
	void SaveTempleNum(vector <CvRect> rs,IplImage * img);
	bool AddTempleImg(IplImage * img);
	friend bool inSameOne(IplImage *img ,IplImage * num,CvPoint *loc);
	friend double inSameOneT(IplImage *img ,IplImage * num,CvPoint *loc);
	friend void getNextMaxLoc(IplImage* result , int templatWidth,int templatHeight,double minValIn ,double * maxValIn , CvPoint *lastLoc );
	//bool isChild;
	
	
	
	vector <TempleImg*> TempleList ;
	CvRect rect;
	
	
	
	
	CvMat* Mat;
	CvMat* MatBak;

	char FilePath[100];
	int Addtemple;
};
#endif