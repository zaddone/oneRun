#include "stdafx.h"
#include "coordRange.h"



CoordRange::CoordRange(const char *data ){
	char info[50] ;
	strcpy(info,data);
	char *p ;
	const char *d = " ";
	p = strtok(info,d);
	int i=0;
	//char str[7][]
	int re[4];
	//int isArr;

	while(p){
		//printf("%d %s\r\n",i,p);
		if (i<4){
			re[i]=atoi (p);
		}else if (i == 4){
			rect = cvRect(re[0],re[1],re[2],re[3]);
			ID = atoi (p);			
		}else if (i == 5){
			IsArr = atoi(p) ;
	//	}else if (i == 6){
			LoadToTempleList();
		}
		
		p=strtok(NULL,d);
		i++;
		if (i==6){
			break;
		}
	}
	//if (i<7){
	//	printf("is i <7 %d",i);
	//} 
	Mat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	MatBak = cvCreateMat(rect.height, rect.width, CV_32FC1);
}

CoordRange::~CoordRange(){
	for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) 
	if (NULL != *it) 
	{
		delete *it; 
		*it = NULL;
	}
	TempleList.clear();
	cvReleaseMat(&Mat);
	cvReleaseMat(&MatBak);
}

TempleImg* CoordRange::findTempleImg(const int tag){
	for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
		if (NULL != *it) {
			if ( (*it)->Tag == tag ) return *it;
		}
	}
	return NULL;
}
void CoordRange::UpdateToTempleListForThreshold(int ThresholdVal){
	for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
		//cvThreshold((*it)->img,(*it)->img, 30, 255, CV_THRESH_BINARY_INV  );
		cvThreshold((*it)->img,(*it)->img, ThresholdVal, 255, CV_THRESH_BINARY );
		
		thresholdVal = ThresholdVal;
		//cvAdaptiveThreshold((*it)->img,(*it)->img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, ThresholdVal, 5 );
		//IplImage* img_temp = cvCreateImage(cvGetSize( (*it)->img), 8, 1);


		
		cvNamedWindow( "bwdst1" );
		cvShowImage( "bwdst1", (*it)->img );
		cvWaitKey(0);
		cvDestroyWindow("bwdst1");
		
	}
	 
}
void CoordRange::LoadToTempleList( ){
	 
	if (IsArr==1){
		struct _finddata_t fd;

		char searchPath[100];
		char path[100];
		char Num[10]={0};
		char * pch;
		sprintf(searchPath,"%d\\*.bmp",this->ID);
		size_t nlen =0;
		intptr_t handle = _findfirst(searchPath, &fd);
		if (handle != -1) {
			do   {
				sprintf(path,"%d\\%s",this->ID,fd.name);
				IplImage * tmpimg = cvLoadImage(path,CV_LOAD_IMAGE_COLOR);
				if (tmpimg==NULL)continue;
				IplImage * img = cvCreateImage(cvGetSize(tmpimg ),tmpimg->depth,1); 
				cvSplit(tmpimg,img,0,0,0);
				
				pch = strchr(fd.name,'.');
				nlen = pch-fd.name;
				memcpy(Num,fd.name,nlen);
				Num[nlen] = 0;
				TempleImg *ni = new TempleImg(img,atoi(Num));
				TempleList.push_back(ni);
				cvReleaseImage(&tmpimg);
		   }  while (_findnext(handle, &fd)==0);
		   _findclose(handle);
		}
	}else{
		struct _finddata_t fd;

		char searchPath[100];
		char path[100];
		char Num[10]={0};
		//char * pch;
		sprintf(searchPath,"%d\\*.bmp",this->ID);
		size_t nlen =0;
		intptr_t handle = _findfirst(searchPath, &fd);
		if (handle != -1) {
			do   {
				sprintf(path,"%d\\%s",this->ID,fd.name);
				IplImage * tmpimg = cvLoadImage(path,CV_LOAD_IMAGE_COLOR);
				if (tmpimg==NULL)continue;
				IplImage * img = cvCreateImage(cvGetSize(tmpimg ),tmpimg->depth,1); 
				cvSplit(tmpimg,img,0,0,0);
				
				//pch = strchr(fd.name,'.');
				nlen =1;// pch-fd.name;
				memcpy(Num,fd.name,nlen);
				Num[nlen] = 0;
				TempleImg *ni = new TempleImg(img,atoi(Num));
				TempleList.push_back(ni);
				cvReleaseImage(&tmpimg);
		   }  while (_findnext(handle, &fd)==0);
		   _findclose(handle);
		}
	}
}

bool CoordRange:: AddTempleImg(IplImage * img){
	size_t n  = TempleList.size();
	if (n>0){
		for (vector<TempleImg * >::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {

			if (compImg((*it)->img,img)) return false;
		}
	}
	TempleImg *ni = new TempleImg(img,n);
	TempleList.push_back(ni);
	char file[100];		
	sprintf(file,"temple\\%d",this->ID);
	CreatDir(file);
	sprintf(file,"%s\\%d.bmp",file,n);
	cvSaveImage(file,img);
	return true;
}
int CoordRange::ReadTempleNum(vector <CvRect> rs,IplImage * img){
	int num = 0;
	for (vector<CvRect >::iterator it = rs.begin(); it != rs.end(); it ++) {
		IplImage* imgNo = cvCreateImage(cvSize( (*it).width,  (*it).height), img->depth, img->nChannels);   
		cvSetImageROI(img, (*it));  
		cvCopyImage(img, imgNo);
		cvResetImageROI(img);
		for (vector<TempleImg * >::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
			if (compImg((*it)->img,img)){
				num =(num*10)+(*it)->Tag;
				break;
			}
		}

		cvReleaseImage(&imgNo); 
	}
	return num;
}
void  CoordRange::SaveTempleNum(vector <CvRect> rs,IplImage * img){
	for (vector<CvRect >::iterator it = rs.begin(); it != rs.end(); it ++) {
		IplImage* imgNo = cvCreateImage(cvSize( (*it).width,  (*it).height), img->depth, img->nChannels);   
		cvSetImageROI(img, (*it));  
		cvCopyImage(img, imgNo);
		if (!AddTempleImg(imgNo)) cvReleaseImage(&imgNo); 
		cvResetImageROI(img);
	}
}

bool CoordRange::GetNumberList(IplImage * src,const int tag) {
	vector <CvRect> templeRect;
	IplImage * img=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,img);
	cvResetImageROI(src);
	cvConvert(img, Mat);
	CvMat* pFrMat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	cvAbsDiff( Mat, MatBak,pFrMat);	
	bool isOut = 0 != cvCountNonZero(pFrMat);
	if (isOut){	
		IplImage * imgbak = cvCreateImage(cvGetSize(img ),img->depth,img->nChannels); 
		//cvCopyImage(img, imgbak);  
		cvThreshold( img,imgbak, tag, 255, CV_THRESH_BINARY  );
		CvSeq* contours = NULL;
		CvMemStorage* storage = cvCreateMemStorage(0);   
		
		int count = cvFindContours(imgbak, storage, &contours,sizeof(CvContour),CV_RETR_EXTERNAL); 

		//int tempCount=0;
		for (CvSeq* c = contours; c != NULL; c = c->h_next) {  
			templeRect.push_back(cvBoundingRect(c,0));
		}
		sort(templeRect.begin(),templeRect.end(),compRectX);
		//return templeRect;
		SaveTempleNum(templeRect,img);
		cvConvert(Mat, MatBak);
		cvReleaseImage(&imgbak);  
	}
	cvReleaseImage(&img);  
	return isOut;
}
int CoordRange::ReadNumberList(IplImage * src,const int tag ) {
	vector <CvRect> templeRect;
	IplImage * img=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,img);
	cvResetImageROI(src);
	cvConvert(img, Mat);
	CvMat* pFrMat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	cvAbsDiff( Mat, MatBak,pFrMat);	
	bool isOut = 0 != cvCountNonZero(pFrMat);
	int num = -1;
	if (isOut){	
		IplImage * imgbak = cvCreateImage(cvGetSize(img ),img->depth,img->nChannels); 
		//cvCopyImage(img, imgbak);  
		cvThreshold( img,imgbak, tag, 255, CV_THRESH_BINARY  );
		CvSeq* contours = NULL;
		CvMemStorage* storage = cvCreateMemStorage(0);   
		
		int count = cvFindContours(imgbak, storage, &contours,sizeof(CvContour),CV_RETR_EXTERNAL); 

		//int tempCount=0;
		for (CvSeq* c = contours; c != NULL; c = c->h_next) {  
			templeRect.push_back(cvBoundingRect(c,0));
		}
		sort(templeRect.begin(),templeRect.end(),compRectX);
		//return templeRect;
		
		cvConvert(Mat, MatBak);
		cvReleaseImage(&imgbak);  
		num = ReadTempleNum(templeRect,img);
	}
	cvReleaseImage(&img);  
	return num;
}
bool CoordRange::PerceptionTempleToSame(IplImage * src,const int tag){
	if (this->IsArr == 2){
		return this->GetNumberList(src,120);
	}
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,dst);
	cvResetImageROI(src);

	cvConvert(dst, Mat);
	CvMat* pFrMat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	cvAbsDiff( Mat, MatBak,pFrMat);	
	bool isOut = 0 != cvCountNonZero(pFrMat);
	if (isOut){
		if (this->IsArr == 1){
			AddTempleImg(dst);
		}else{
			char file[100];		
			sprintf(file,"temple\\%d",this->ID);
			CreatDir(file);
			sprintf(file,"%s\\%d.bmp",file,tag);
			cvSaveImage(file,dst);		
		}
		cvConvert(Mat, MatBak);
	}
	cvReleaseMat(&pFrMat);
	cvReleaseImage(&dst);
	return isOut;
}
int CoordRange::UpRect(IplImage * src,CvRect r,CvPoint &loc ){
	cvSetImageROI(src, r);
	IplImage * dst=cvCreateImage(cvSize(r.width,r.height),src->depth,src->nChannels);
	cvCopy(src,dst);
	int n;
	n = readOne(dst,&loc) ;


	cvResetImageROI(src); 
	cvReleaseImage(&dst);
	return n;
}
int CoordRange::UpRect(IplImage * src, CvPoint *loc ){
	cvSetImageROI(src, this->rect);
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvCopy(src,dst);
	int n;
	if (loc == NULL){
		CvPoint loc;
		n = readOne(dst,&loc) ;
		if (n != -1){
			 
				Coord.clear();
				//InitCoordinate();
				TempleImg* imgNum = findTempleImg(n);
				if ( imgNum != NULL ){
					Coordinate coo(loc.x+rect.x+(imgNum->img->width/2)  ,loc.y +rect.y+(imgNum->img->height/2) ,n);		
					Coord.push_back(coo);
				}
				//printf("add img coordinate %d %d\r\n",_x,_y);
 
		}



	}else{
		n = readOne(dst,loc) ;
	}

	cvResetImageROI(src); 
	cvReleaseImage(&dst);
	return n;
}
int CoordRange::UpRectArr(IplImage * src ){
	cvSetImageROI(src, rect);
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	//IplImage * dst1=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvCopy(src,dst);
	//cvAdaptiveThreshold(dst,dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 51, 5 );
	//cvThreshold( dst,dst, thresholdVal, 255, CV_THRESH_BINARY  );


	int n = -1;
	CallBackVal = -1;
	int *arr = new int[14];
	CvPoint *loc = new CvPoint[14];

	//int *x = new int[14];
	//int *y = new int[14];
	n = readArr(dst,arr,loc) ;
	//printf("n: %d %d\r\n",n,ID);
	if (n>0 ){
		Coord.clear();
		for (int i = 0;i<n;i++){
			TempleImg* imgNum = findTempleImg(arr[i]);
			Coordinate coo( loc[i].x+rect.x + (imgNum->img->width/2),loc[i].y + rect.y + (imgNum->img->height/2),arr[i]);		
			Coord.push_back(coo);
		}
	}
//sort(Coord.begin(),Coord.end(),SortForX);
	delete [] arr;
	delete [] loc;
	//delete [] y;
	cvResetImageROI(src);
	 
	cvReleaseImage(&dst);
	return n;
}
int CoordRange::UpRectArr(IplImage * src,const int sep){
	int n = rect.width/sep;
	if (n==0 )return 0;

	int *arr = new int[n];
	CvPoint *loc = new CvPoint[n];
	CvRect *R = new CvRect[n];
	int j = 0;
	for (int i = 0;i<n;i++){

		R[j] = cvRect(rect.x+i*sep,rect.y,sep,rect.height);
		//arr[j]=UpRect(src,&loc[j]);
		arr[j]=UpRect(src,R[j],loc[j]);
		//printf("%d %d %d\r\n",i,loc[i].x,loc[i].y);
		if (arr[j] == -1) {
			printf("err n = %d\r\n",i);
			continue;
		}
		printf("st %d\r\n",j);
		j++;
	}
	
 
	Coord.clear();

	for (int i = 0;i<j;i++){

		TempleImg* imgNum = findTempleImg(arr[i]);
		Coordinate coo(loc[i].x+R[i].x + (imgNum->img->width/2),loc[i].y+R[i].y+ (imgNum->img->height/2),arr[i]);

		Coord.push_back(coo);
	}


	delete [] arr;
	delete [] loc;
	delete [] R;
	return j;	
}
int CoordRange::UpSelf(IplImage * src,IplImage * dst){
	 
	IplImage * dst1=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	IplImage * dst2=cvCreateImage(cvSize(rect.width,rect.height),dst->depth,dst->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,dst1);
	cvResetImageROI(src);

	cvSetImageROI(dst, rect);
	cvCopy(dst,dst2);
	cvResetImageROI(dst);


	cvConvert(dst1, Mat);
	cvConvert(dst2, MatBak);

	CvMat* pFrMat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	cvAbsDiff( Mat, MatBak,pFrMat);	
	int n = -1;
	if (0 != cvCountNonZero(pFrMat)){		 
		CvPoint loc;
		//cvCopy(src,dst);
	 
	 
		n = readOne (dst1,&loc) ;
		if (n != -1){			 
				Coord.clear();
				//InitCoordinate();
				TempleImg* imgNum = findTempleImg(n);
				if ( imgNum != NULL ){
					Coordinate coo(loc.x+rect.x+(imgNum->img->width/2)  ,loc.y +rect.y+(imgNum->img->height/2) ,n);		
					Coord.push_back(coo);
				}
				//printf("add img coordinate %d %d\r\n",_x,_y); 
		}
		//cvConvert(Mat, MatBak);
	}
	
	cvReleaseMat(&pFrMat);
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst2);
	return n;
}
bool CoordRange::Wait(){
	int timeC = 0;
	while(!isUp){
		Sleep(10);
		timeC++;
		if (timeC ==500) {
			return false;
		}
	}
	return true;
}
int CoordRange::WaitEvent(){
	int timeC = 0;
	while(this->CallBackVal == -1 ){
		Sleep(10);
		timeC++;
		if (timeC ==200) {
			return -1;
		}
	}
	return CallBackVal;
	 
}
void CoordRange::SendToServerNum(int num,Sendback func, LPVOID lpParamter,const int k){
 
	if (lpParamter == NULL) return;
	CallBackVal = -1;

	int j = (int)this->Coord.size()+1;
	if (j == 0) return;
	int *arr = new int[j] ;
	arr[0] = num;
	int i = 1;
	int v;
	for (vector<Coordinate >::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		v = (*it).v;
		if (-1 < v && v <30){
			arr[i] = v;
			i++;
		}
	}

	
	isUp = false;
	//printf("send arr %d\r\n",j);

	func(lpParamter,&isUp,ID,arr,i,k);
}
void CoordRange::SendToServer(Sendback func, LPVOID lpParamter,const int k){
 
	if (lpParamter == NULL) return;
	CallBackVal = -1;

	int j = (int)this->Coord.size();
	if (j == 0) return;
	int *arr = new int[j] ;
	int i = 0;
	for (vector<Coordinate >::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		arr[i] = (*it).v;
		i++;
	}
	isUp = false;
	//printf("send arr %d\r\n",j);

	func(lpParamter,&isUp,ID,arr,j,k);
}
bool CoordRange::ClickCoordinate(int _v,int num){
	for (vector<Coordinate>::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		//if (*it == 0 )continue;
		if ((*it).MouseClick(_v,num) ){
			Coord.erase(it);
			return true;		
		}
	}
	return false;
}
int CoordRange::readArr(IplImage *img,int *Arr,CvPoint *loc ){
	int N = 0;
	int n = 0;
	int ln= 0;
	//int t;
	
	for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) { 
			inSameT(img,( *it),Arr,loc,N);			 
	}
	return N;
}

int CoordRange::GetDataNum(){
	sort(Coord.begin(),Coord.end(),comp);
	int val = 0;
	for (vector<Coordinate >::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		val *= 10;
		val +=(*it).v;
	}
	return val;
}
void inSameT(IplImage * img ,TempleImg * NI,int* Arr,CvPoint *loc,int & N){
		//CvSize sizeResult = cvSize(img->width - NI->img->width + 1,img->height - NI->img->height + 1);   
	IplImage* imgResult = cvCreateImage(cvSize(img->width - NI->img->width + 1,img->height - NI->img->height + 1),IPL_DEPTH_32F,1);   
	cvMatchTemplate(img,NI->img,imgResult,CV_TM_CCOEFF_NORMED); 
	 
    double max_val,min_val;  
	CvPoint minLoc;

	cvMinMaxLoc(imgResult, &min_val, &max_val, &minLoc, &loc[N], NULL);  
	while (max_val >0.8){
		Arr[N]=NI->Tag;
		//printf("%d %d %d %d\r\n",N,NI->num,loc[N].x,loc[N].y);
		//loc[N] = max_loc;
		N++;
		if (N==14)break;
		//CvPoint next_loc;
		max_val = getNextMaxLoc(imgResult , NI->img->width,NI->img->height,min_val , loc,N);
		//N++;
		
	}
	cvReleaseImage(&imgResult);

}
double getNextMaxLoc(IplImage* result , int templatWidth,int templatHeight,double minValIn , CvPoint *lastLoc,int  n)
{
	int startX = lastLoc[n-1].x ;  
    int startY = lastLoc[n-1].y ;  
    int endX = lastLoc[n-1].x + templatWidth;  
    int endY = lastLoc[n-1].y + templatHeight;  
    if(startX < 0 || startY < 0)  
    {  
        startX = 0;  
        startY = 0;  
    }  
    if(endX > result->width - 1 || endY > result->height - 1)  
    {  
        endX = result->width - 1;  
        endY = result->height - 1;  
    }  
    int y, x;  
    for(y = startY; y < endY; y++)  
    {  
        for(x = startX; x < endX; x++)  
        {  
            cvSetReal2D(result, y, x, minValIn);  
        }  
    }  
	//n++;
	 

         double minVal,maxVal;
        CvPoint minLoc;        //查找result中的最小值 及其所在坐标
        cvMinMaxLoc(result,&minVal,&maxVal,&minLoc,&lastLoc[n],NULL);
        return maxVal; 
} 
int CoordRange::readOne(IplImage * dst,CvPoint * loc){
	int maxN = -1;
	double max=0;
	double val = 0;
	CvPoint  l;
	for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
		
		val = ValSameOnlyT(dst,( *it)->img,&l);
		if (val < 0.9) {
			continue;
		}
		if (val>max){
			maxN = (*it)->Tag;
			max = val;
			loc->x = l.x;
			loc->y = l.y;
		}

	}
	return maxN;
}

double ValSameOnlyT(IplImage *img ,IplImage * num,CvPoint *loc){
	//printf("v1>%d %d\r\n",img->width , num->width);
	IplImage* imgResult = cvCreateImage(cvSize(img->width - num->width + 1,img->height - num->height + 1),IPL_DEPTH_32F,1); 
	//printf("v2>%d %d\r\n",img->depth,num->depth);
	cvMatchTemplate(img,num,imgResult,CV_TM_CCOEFF_NORMED); 	 
    double max_val,min_val; 
	CvPoint minLoc;

	cvMinMaxLoc(imgResult, &min_val, &max_val, &minLoc, loc, NULL);  
	//if (max_val>0.8)printf("%f\r\n",max_val);
	cvReleaseImage(&imgResult);
	
	return max_val;
}


void CallBackEventC(LPVOID handle,char *data){
	CoordRange * r = (CoordRange *) handle;

	//printf("%s\r\n",data);
	char *p ;
	const char *d = " ";
	p = strtok(data,d);
	int i=0;
	int n;
	int val =0;
	
	while(p){
		n = atoi(p);
		if (i==0){
			//if (n!=r->ID) return;
			i++;
			p=strtok(NULL,d);
			continue;

		}
		//do{
		  
		//printf("%s %d\r\n",p,val);
		if (n > -1){
			if (r->ClickCoordinate(n,1)){
				val++;
				printf("OK %d\r\n",n);
				//Sleep(150);
				
		//	}else{
		//		break;
			}
		}
		//IplImage * src = hBitmap2Ipl(Screen());
		//}while( -1 != r->UpSelf(hBitmap2Ipl(Screen())));
		p=strtok(NULL,d);
		//i++;
	}
	r->CallBackVal = val;
}
