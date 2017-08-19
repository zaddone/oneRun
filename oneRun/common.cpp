#include "stdafx.h"
#include "common.h"


void  ClearSameTempleImg(char * Filepath){

	vector <IplImage*> TempleList ;
	
	struct _finddata_t fd;

	char searchPath[100];
	char path[100];
	char Num[10]={0};
	//char * pch;
	sprintf(searchPath,"%s\\*.bmp",Filepath);
	printf("%s\r\n",searchPath);
	size_t nlen =0;
	intptr_t handle = _findfirst(searchPath, &fd);
	if (handle != -1) {
		do   {
			sprintf(path,"%s\\%s",Filepath,fd.name);
			IplImage * img = cvLoadImage(path,CV_LOAD_IMAGE_UNCHANGED);
			if (img==NULL)continue;
				 
			//TempleImg *ni = new TempleImg(img,atoi(Num));
			bool isSave = true;
			for (vector<IplImage*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
				if (compImg((*it),img)){
					remove(path);
					isSave = false;
					cvReleaseImage(&img);
					break;
				}

			}
			if(isSave){
		//		cvNamedWindow("contour1");
		//		cvShowImage("contour1", img);  
		//		cvWaitKey(0);
		//		cvDestroyWindow("contour1");
			
				TempleList.push_back( img );
			}
			//cvReleaseImage(&tmpimg);
		}  while (_findnext(handle, &fd)==0);
		_findclose(handle);
	}
	TempleList.clear();

}
bool compRectX(CvRect a,CvRect b)
{
	return a.x<b.x;
}
bool compImg(IplImage * a,IplImage * b){
	int height     = a->height;
	int width      = a->width;
	if ( width != b->width) return false;
	if ( height != b->height) return false;


	int step       = a->widthStep;
	uchar * data    =  (uchar *) a->imageData;
	int channels   = a->nChannels;

	int nstep       = b->widthStep;
	uchar * ndata    = (uchar *) b->imageData;
	int nchannels   = b->nChannels;
	int i,j;
	
	for(i=0; i< height  ; i+=1){
		for(j=0; j< width ; j+=1){
			if (data[i *step+j*channels+0] != ndata[i*nstep+j*nchannels+0]){
				return false;
			}
		}
	}
	return true;

}
bool hBitmap2Ipls(HBITMAP hBmp,IplImage* dst ){

	BITMAP bmp;
	
    GetObject(hBmp,sizeof(BITMAP),&bmp);
 
    int    nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8;
	//int    nChannels = bmp.bmBitsPixel == 1 ? 1 : 4;
    int    depth     = 8;//bmp.bmBitsPixel;//   == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	//printf("%d %d %d %d %d \r\n",bmp.bmBitsPixel,nChannels,depth,bmp.bmWidth, bmp.bmHeight );
    IplImage *img=cvCreateImage(cvSize(bmp.bmWidth, bmp.bmHeight), IPL_DEPTH_8U,nChannels);
	

    
	//img->imageData = (char*)malloc(bmp.bmHeight*bmp.bmWidth*nChannels*sizeof(char));
    BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];
	GetBitmapBits(hBmp,bmp.bmHeight*bmp.bmWidth*nChannels,pBuffer);
	memcpy(img->imageData,pBuffer,bmp.bmHeight*bmp.bmWidth*nChannels);
    delete pBuffer; 

	//IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,1);  
	//printf("%d \n",nChannels);
	/**
	if (nChannels == 2)
		cvCvtColor(img,dst,CV_BGR5652RGB);  
	else if (nChannels == 3)
		cvCvtColor(img,dst,CV_BGR2RGB);
	else if (nChannels == 4)
		cvCvtColor(img,dst,CV_RGBA2RGB);
	**/
	if (img->width != dst->width || img->height != dst->height){
		cvReleaseImage(&img);  
		DeleteObject(hBmp);
		return false;
	}
	
	if (nChannels == 2)
		cvCvtColor(img,dst,CV_BGR5652GRAY);  
	else if (nChannels == 3)
		cvCvtColor(img,dst,CV_BGR2GRAY);
	else if (nChannels == 4)
		cvCvtColor(img,dst,CV_BGRA2GRAY);
 
	//cvSplit(img,dst,0,0,0);

	cvReleaseImage(&img);  
	DeleteObject(hBmp);
	return true;
	//delete bmp;
	//return dst;  
}
IplImage* hBitmap2Ipl(HBITMAP hBmp )
{
	//bitmap.GetBitmap(&bm); 
    BITMAP bmp;
    GetObject(hBmp,sizeof(BITMAP),&bmp);
    int    nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8;
    int    depth     = 8;//bmp.bmBitsPixel;// == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
    IplImage *img=cvCreateImage(cvSize(bmp.bmWidth, bmp.bmHeight), IPL_DEPTH_8U,nChannels);
    BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];
	GetBitmapBits(hBmp,bmp.bmHeight*bmp.bmWidth*nChannels,pBuffer);
	memcpy(img->imageData,pBuffer,bmp.bmHeight*bmp.bmWidth*nChannels);
    delete pBuffer; 

	IplImage *dst = cvCreateImage(cvGetSize(img),depth,1);

	if (nChannels == 2)
		cvCvtColor(img,dst,CV_BGR5652GRAY);  
	else if (nChannels == 3)
		cvCvtColor(img,dst,CV_BGR2GRAY);
	else if (nChannels == 4)
		cvCvtColor(img,dst,CV_BGRA2GRAY);
	

	//cvSplit(img,dst,0,0,0);
	cvReleaseImage(&img);  
	DeleteObject(hBmp);
	return dst;  

}

HBITMAP Screen()
{
	HDC desk=GetDC(GetDesktopWindow());
	int screenWidth = GetDeviceCaps(desk,HORZRES);     //屏幕宽
	int screenHeight = GetDeviceCaps(desk,VERTRES);     //屏幕高
	HBITMAP memBitmap=CreateCompatibleBitmap(desk,screenWidth,screenHeight);

	HDC memDC=CreateCompatibleDC(desk);

	SelectObject(memDC,memBitmap);
	BitBlt(memDC,0,0,screenWidth,screenHeight,desk,0,0,SRCCOPY|CAPTUREBLT);
	BITMAP bmpInfo;
	GetObject(memBitmap,sizeof(bmpInfo),&bmpInfo);
	DeleteDC(memDC);
	ReleaseDC(GetDesktopWindow(),desk);
	return  memBitmap;
}
CString CharToCString(char* str){
	int charLen = (int)strlen(str);  
	//计算多字节字符的大小，按字符计算。  
	int len = (int)MultiByteToWideChar(CP_ACP,0,str,charLen,NULL,0);  
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小  
	TCHAR *buf = new TCHAR[len + 1];  
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP,0,str,charLen,buf,len);  
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1  
	//将TCHAR数组转换为CString  
	CString pWideChar;  
	pWideChar.Append(buf);  
	//删除缓冲区  
	delete []buf;  
	return pWideChar;
}
void CStringToChar(CString src,char * dst ){

	int len = WideCharToMultiByte(CP_ACP,0,src,-1,NULL,0,NULL,NULL);  
	//char *ptxtTemp = new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,src,-1,dst,len,NULL,NULL );  
	//return ptxtTemp;
}

int CreatDir(char *pDir){  
    int i = 0;  
    int iRet;  
    int iLen;  
    char* pszDir;  
  
    if(NULL == pDir)  
    {  
        return 0;  
    }  
      
    pszDir = _strdup(pDir);  
    iLen = int(strlen(pszDir));  
    for (i = 0;i < iLen;i ++)  
    {  
        if (pszDir[i] == '\\' || pszDir[i] == '/')  
        {   
            pszDir[i] = '\0';   
            iRet = ACCESS(pszDir,0);  
            if (iRet != 0)  
            {  
                iRet = MKDIR(pszDir);  
                if (iRet != 0)  
                {  
                    return -1;  
                }   
            }
            pszDir[i] = '/';  
        }   
    }  
  
    iRet = MKDIR(pszDir);  
    //free(pszDir);  
	delete []pszDir;
    return iRet;  
} 
 