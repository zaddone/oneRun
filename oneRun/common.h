#ifndef Common_H_
#define Common_H_
#include <afxwin.h>
#include <cv.h>
#include <highgui.h>

#include <vector>
using namespace std;

#ifdef _WIN32  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#elif _LINUX  
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#endif  


void ClearSameTempleImg(char * path);
bool compRectX(CvRect a,CvRect b);
bool compImg(IplImage * a,IplImage * b);
bool hBitmap2Ipls(HBITMAP hBmp,IplImage* dst );
IplImage* hBitmap2Ipl(HBITMAP hBmp );
HBITMAP Screen();
CString CharToCString(char* str);
void CStringToChar(CString src,char* dst);
typedef void (*Sendback)( LPVOID handle,bool * r,const int id,const int * data,const int len, const int k);  
int CreatDir(char *pDir);
#endif