#ifndef CoordRange_H_
#define CoordRange_H_
#include "common.h"

#include<vector>
#include "templeImg.h"
#include "Coordinate.h"


using namespace std;

//typedef void (*SendCallback)( LPVOID handle,bool * r,const int id,const int * data,const int len, const int k);  
double ValSameOnlyT(IplImage *img ,IplImage * num,CvPoint *loc);

double getNextMaxLoc(IplImage* result , int templatWidth,int templatHeight,double minValIn , CvPoint *lastLoc,int  n);
void inSameT(IplImage * img ,TempleImg * NI,int* Arr,CvPoint *loc,int & N);

class CoordRange
{
public:
	CoordRange(const char *data  );
	int UpSelf(IplImage * src,IplImage * dst);
	//int UpRect(IplImage * src);
	int UpRect(IplImage * src,CvRect r,CvPoint &loc );
	int UpRect(IplImage * src, CvPoint *loc = NULL );
	int UpRectArr(IplImage * src );
	int UpRectArr(IplImage * src,const int sep);	
	bool ClickCoordinate(int v,int num);
	void SendToServerNum(int num,Sendback func, LPVOID lpParamter,const int k);
	void SendToServer(Sendback func, LPVOID lpParamter,const int k);
	friend void CallBackEventC(LPVOID handle,char *data);
	int WaitEvent();
	bool Wait();
	~CoordRange();
	int GetDataNum();
	bool PerceptionTempleToSame(IplImage * src,const int tag = 0);
	bool PerceptionToTempleNum(IplImage * img,const int tag = 120);
	bool GetNumberList(IplImage * img,const int tag = 120);
	int ReadNumberList(IplImage * img,const int tag = 120);
	void SaveTempleNum(vector <CvRect> rs,IplImage * img);
	int ReadTempleNum(vector <CvRect> rs,IplImage * img);
	bool AddTempleImg(IplImage * img);
	int ID;
	vector <Coordinate>  Coord;
	CvRect rect;
	void UpdateToTempleListForThreshold(int ThresholdVal = 30);
	//int WaitEvent();
	int IsArr;
private:
	
	void LoadToTempleList( );
	

	int thresholdVal;
	int readOne(IplImage * dst,CvPoint * loc);
	int readArr(IplImage *img,int *Arr,CvPoint *loc );
	//int CoordRange::readArr(IplImage *img,int *Arr,CvPoint *loc )
	TempleImg* findTempleImg(const int tag);

	int CallBackVal;
	vector <TempleImg*> TempleList ;
	
	
	
	CvMat* Mat;
	CvMat* MatBak;
	char * path;
	bool isUp;
};
#endif