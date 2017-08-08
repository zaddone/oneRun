#include "stdafx.h"
#include "block.h"


Block::Block(CvRect r,int type,vector <TempleImg*> T,int coll){
	this->rect = r;
	this->IsArr = type;
	Addtemple = 0;
	//TempleList.clear();
	TempleList.assign(T.begin(), T.end());
	//this->TempleList= T;
	this->Sep = -1;
	Mat = NULL;// cvCreateMat(rect.height, rect.width, CV_32FC1);
	MatBak = NULL;// cvCreateMat(rect.height, rect.width, CV_32FC1);
	backVal = -1;
	this->Coll = coll;
}
Block::Block(const char *data,CvPoint po,int coll){
	//isChild = false;
	//printf("%s\r\n",data);
	Addtemple = 0;
	this->Coll = coll;
	char info[50] ;
	
	strcpy(info,data);
	char *p ;
	const char *d = " ";
	p = strtok(info,d);
	int i=0;
	//char str[7][]
	int re[4];
	//int isArr;
	this->Sep = -1;
	backVal = -1;
	while(p){
		//printf("%d %s\r\n",i,p);
		if (i<4){
			re[i]=atoi (p);
		}else if (i == 4){
			//printf("%d %d %d %d\r\n",re[0],po.x,re[1], po.y);
			rect = cvRect(re[0]+po.x,re[1] + po.y,re[2],re[3]);
			ID = atoi (p);			
		}else if (i == 5){
			IsArr = atoi(p) ;
		}else if (i == 6){
			LoadToTempleList(p);
		}else if (i == 7) {
			this->Sep = atoi(p);
		}
		
		p=strtok(NULL,d);
		i++;
 
	}
	Mat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	MatBak = cvCreateMat(rect.height, rect.width, CV_32FC1);
	
	//printf("%d %d\r\n",ID,Coll);
}
Block::~Block(){
	for (vector<Block*>::iterator it = Child.begin(); it != Child.end(); it ++) {
		delete (*it);
		*it = NULL;
	}
	Child.clear();

	vector <Coordinate>().swap(Coord);
	TempleList.clear();
	
	if (Mat != NULL)
		cvReleaseMat(&Mat);
	if (MatBak != NULL)
		cvReleaseMat(&MatBak);

}
int Block::FindArr(IplImage * src,const bool isT ){
	if (TempleList.size() == 0) return 0;
	CallBackVal = -1;
	cvSetImageROI(src, this->rect);
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	IplImage * dstbak=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvCopy(src,dst);
	cvResetImageROI(src);
	this->ClearCoord();

	if (this->Sep >0){
		cvThreshold( dst,dstbak, this->Sep , 255, CV_THRESH_BINARY  );
	}else{
		cvThreshold( dst,dstbak, 50 , 255, CV_THRESH_BINARY  );
	}
	vector <CvRect> templeRect;
	CvSeq* contours = NULL;
	CvMemStorage* storage = cvCreateMemStorage(0);   
		
	int count = cvFindContours(dstbak, storage, &contours,sizeof(CvContour),CV_RETR_EXTERNAL); 

	//int tempCount=0;
	for (CvSeq* c = contours; c != NULL; c = c->h_next) {  
		templeRect.push_back(cvBoundingRect(c,0));
	}
	sort(templeRect.begin(),templeRect.end(),compRectX);
	int j = 0;
	for (vector<CvRect>::iterator it = templeRect.begin(); it != templeRect.end(); it ++) {
		Block *bl =new Block((*it),1,this->TempleList,this->Coll);
		strcpy(bl->FilePath,this->FilePath);
		bl->Addtemple = this->Addtemple*100;
		if (bl->FindOne(dst,isT) != -1) {
			Coord.push_back(bl->Coord[0]);
			j++;
		}else{			 
			this->Addtemple++;
		}
		delete bl;
		
	}
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&dst);
	cvReleaseImage(&dstbak);
	return j;
 
}
bool Block::PerceptionTempleToSame(IplImage * src,const int tag){
 
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,dst);
	cvResetImageROI(src);

	cvConvert(dst, Mat);
	CvMat* pFrMat = cvCreateMat(rect.height, rect.width, CV_32FC1);
	cvAbsDiff( Mat, MatBak,pFrMat);	
	bool isOut = 0 != cvCountNonZero(pFrMat);
	if (isOut){
		char file[100];
		sprintf(file,"temple\\%d",this->ID);
		CreatDir(file);
		sprintf(file,"%s\\%d.bmp",file,tag);
		cvSaveImage(file,dst);	
		cvConvert(Mat, MatBak);
	}
	cvReleaseMat(&pFrMat);
	cvReleaseImage(&dst);
	return isOut;
}
bool Block::GetNumberList(IplImage * src,const int tag) {
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
		cvReleaseMemStorage(&storage);
	}
	cvReleaseImage(&img);  
	return isOut;
}
void Block::GetNumberList_test(IplImage * src,const int tag) {
	vector <CvRect> templeRect;
	IplImage * img=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvSetImageROI(src, rect);
	cvCopy(src,img);
	cvResetImageROI(src);
	 
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
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&imgbak);  
	
	cvReleaseImage(&img);  
}
void  Block::SaveTempleNum(vector <CvRect> rs,IplImage * img){
	for (vector<CvRect >::iterator it = rs.begin(); it != rs.end(); it ++) {
		IplImage* imgNo = cvCreateImage(cvSize( (*it).width,  (*it).height), img->depth, img->nChannels);   
		cvSetImageROI(img, (*it));  
		cvCopyImage(img, imgNo);
		if (!AddTempleImg(imgNo)) cvReleaseImage(&imgNo); 
		cvResetImageROI(img);
	}
}
bool Block:: AddTempleImg(IplImage * img){
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
int Block::GetDataNum(){
	sort(Coord.begin(),Coord.end(),comp);
	int val = 0;
	for (vector<Coordinate >::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		val *= 10;
		val +=(*it).v;
	}
	return val;
}
int Block::FindChildOne(IplImage * src,const bool isT){
	//CvPoint loc;
	//cvSetImageROI(src, rect);
	//IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	//cvCopy(src,dst);
	//cvResetImageROI(src); 
	//this->Coord.clear();
	int n = -1,i=0;
	//CallBackVal = -1;
	//bool isH;
	if (this->Child.size() >0){
		
		for (vector<Block*>::iterator it = Child.begin(); it != Child.end(); it ++) {
			
			n = (*it)->FindOne(src,isT);
			if (n != -1){
				//Coordinate coor((*it)->Coord[0].x+rect.x,(*it)->Coord[0].y+rect.y,n);
				//this->Coord.push_back((*it)->Coord[0]);
				return i;
				//n = i;
				//break;
			}
			i++;
		}
	}
//	cvReleaseImage(&dst);
	return n;
}
int Block::FindOne(IplImage * src,const bool isT){
	CvPoint loc;
	//printf("%d %d\r\n",rect.x,rect.y);
	cvSetImageROI(src, rect);
	IplImage * dst=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvCopy(src,dst);
	cvResetImageROI(src); 
	/*
	
	cvNamedWindow("contour1");
	cvShowImage("contour1", dst);  
	cvWaitKey(0);
	cvDestroyWindow("contour1");
	//cvReleaseImage(&dst);
	*/

	//this->Coord.clear();
	this->ClearCoord();
	int n = -1;
	CallBackVal = -1;
	
	if (TempleList.size() > 0){
		if (!isT){
			for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
				if (inSameOne(dst, (*it)->img,&loc)){
					Coordinate coo(rect.x+loc.x +( (*it)->img->width/2)  ,rect.y+loc.y +((*it)->img->height/2) ,(*it)->Tag );	
					coo.rect = cvRect(rect.x+loc.x,rect.y+loc.y,(*it)->img->width,(*it)->img->height);
					coo.te = (*it);
					this->Coord.push_back(coo);
					n = (*it)->Tag;
					break;
				}
			}
		}else{
			//bool isH;
			double max = 0,val;
			CvPoint  l;
			Coordinate coo(0,0,0 );
			//coo.rect = rect;
			
			for (vector<TempleImg*>::iterator it = TempleList.begin(); it != TempleList.end(); it ++) {
				val = inSameOneT(dst, (*it)->img,&l);
				if (val < 0.8) {
					

					continue;
				}
				if (val>max){
					n = (*it)->Tag;
					max = val;
					loc  = l;
					coo.x = rect.x+loc.x +( (*it)->img->width/2);
					coo.y = rect.y+loc.y +((*it)->img->height/2);
					coo.v = n;
					coo.te = (*it);
					coo.rect = cvRect(rect.x+loc.x,rect.y+loc.y,(*it)->img->width,(*it)->img->height);
					//loc->y = l.y;
				}
			}
			if (n != -1){
				//Coordinate coo(loc.x +( (*it)->img->width/2)  ,loc.y +((*it)->img->height/2) ,(*it)->Tag);	
				this->Coord.push_back(coo);
		 
			}else  {
				if (this->IsArr == 1 && Coll == 1){
					char file[100];		
					sprintf(file,"temple\\%s",this->FilePath);
					CreatDir(file);
					sprintf(file,"%s\\%d.bmp",file,Addtemple);
					Addtemple++;
					cvSaveImage(file,dst);
				}
			}
		}
	}else if (this->Child.size() >0){
		
		for (vector<Block*>::iterator it = Child.begin(); it != Child.end(); it ++) {
			n = (*it)->FindOne(src,isT);
			if (n != -1){
				//Coordinate coor((*it)->Coord[0].x+rect.x,(*it)->Coord[0].y+rect.y,n);
				this->Coord.push_back((*it)->Coord[0]);
				break;
			}
		}
	}

	cvReleaseImage(&dst);
	return n;
}
int Block::FindSep(IplImage * src,const bool isT){
	int n = rect.width/this->Sep;
	if (n <= 0 )return 0;
	CallBackVal = -1;
	Coord.clear();
	int j = 0;
	for (int i = 0;i<n;i++){
		CvRect r = cvRect(rect.x+i*this->Sep,rect.y,this->Sep,rect.height);
		/**
		cvSetImageROI(src, r);
		IplImage * dst=cvCreateImage(cvSize(r.width,r.height),src->depth,src->nChannels);
		cvCopy(src,dst);
		cvResetImageROI(src); 
		cvNamedWindow("contour1");
		cvShowImage("contour1", dst);  
		cvWaitKey(0);
		cvDestroyWindow("contour1");
		cvReleaseImage(&dst);
		**/
		Block *bl =new Block(r,0,this->TempleList,Coll);
		if (bl->FindOne(src,isT) != -1) {
			Coord.push_back(bl->Coord[0]);
			j++;
		}
		delete bl;
	}

	return j;
}
void Block::SendToServerNum(int num,Sendback func, LPVOID lpParamter,const int k){
 
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
void Block::SendToServer(Sendback func, LPVOID lpParamter,const int k){
 
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
void Block::LoadToTempleList(char * Filepath ){
	//Filepath.tirm();
	strcpy(FilePath, Filepath);
	if (IsArr==1){
		
	//}else if (IsArr==1){
		struct _finddata_t fd;

		char searchPath[100];
		char path[100];
		char Num[10]={0};
		char * pch;
		sprintf(searchPath,"tag\\%s\\*.bmp",Filepath);
		size_t nlen =0;
		intptr_t handle = _findfirst(searchPath, &fd);
		if (handle != -1) {
			do   {
				sprintf(path,"tag\\%s\\%s",Filepath,fd.name);
				IplImage * img = cvLoadImage(path,CV_LOAD_IMAGE_UNCHANGED);
				if (img==NULL)continue;
				//IplImage * img = cvCreateImage(cvGetSize(tmpimg ),tmpimg->depth,1); 
				//cvSplit(tmpimg,img,0,0,0);
				
				pch = strchr(fd.name,'_');
				if (pch == NULL) nlen = 1;
				else	nlen = pch-fd.name;
				memcpy(Num,fd.name,nlen);
				Num[nlen] = 0;
				//TempleImg *ni = new TempleImg(img,atoi(Num));
				TempleList.push_back(new TempleImg(img,atoi(Num)));
				//cvReleaseImage(&tmpimg);
		   }  while (_findnext(handle, &fd)==0);
		   _findclose(handle);
		}
	}else if (IsArr==2){
		char path[100] = {0};
		sprintf(path,"tag\\%s",Filepath);

		IplImage * img = cvLoadImage(path,CV_LOAD_IMAGE_UNCHANGED);
		if (img == NULL) return;
		/**
		char Num[10]={0};
		char * pch = strchr(Filepath ,'.');
		size_t nlen = pch-Filepath ;
		memcpy(Num,Filepath,nlen);
		Num[nlen] = 0;
		**/
		//TempleImg *ni = new TempleImg(img,atoi(Num));
		TempleList.push_back(new TempleImg(img,0));
	}else{
		struct _finddata_t fd;

		char searchPath[100];
		char path[100];
		char Num[10]={0};
		char * pch;
		sprintf(searchPath,"tag\\%s\\*.bmp",Filepath);
		size_t nlen =0;
		intptr_t handle = _findfirst(searchPath, &fd);
		if (handle != -1) {
			do   {
				sprintf(path,"tag\\%s\\%s",Filepath,fd.name);
				IplImage * img = cvLoadImage(path,CV_LOAD_IMAGE_UNCHANGED);
				if (img==NULL)continue;
				//IplImage * img = cvCreateImage(cvGetSize(tmpimg ),tmpimg->depth,1); 
				//cvSplit(tmpimg,img,0,0,0);
				
				pch = strchr(fd.name,'.');
				nlen = pch-fd.name;
				memcpy(Num,fd.name,nlen);
				Num[nlen] = 0;
				//TempleImg *ni = new TempleImg(img,atoi(Num));
				TempleList.push_back(new TempleImg(img,atoi(Num)));
				//cvReleaseImage(&tmpimg);
		   }  while (_findnext(handle, &fd)==0);
		   _findclose(handle);
		}
		
		
		if (IsArr == 3){
		//isChild = true;
		char dataFile[15] = {0};
		sprintf(dataFile,"data_%d.log",this->ID);

		fstream In(dataFile, ios::in|ios::binary);	
		CvPoint po = {this->rect.x,this->rect.y};
		if (In.is_open())   
		{  		
			printf("open ok %s\r\n",dataFile);
			char tmpStr[50];
			//int UserArrLen = 0;
			while (!In.eof() ){
				In.getline(tmpStr ,50,'\n');
				if (strlen(tmpStr)==0){
					break;
				}
				//printf("%s\r\n",tmpStr);
				//rectInfo *UserA  = new rectInfo(tmpStr );
				Block *bl = new Block(tmpStr, po,Coll) ;
				
				Child.push_back(bl);
			}
			In.close();
		}

		}
	}
}

bool inSameOne(IplImage *img ,IplImage * num,CvPoint *loc){

	int h = img->height - num->height;
	int w = img->width - num->width;
	if (h==0 && w == 0){
		if (compImg(img,num)){
			loc->x = 0;
			loc->y = 0;
			return true;
		}
	}else if (h<0 && w < 0){
		return inSameOne(num,img,loc);
	}else if(h<0 || w < 0){
		return false;
	}
	int height     = img->height;
	int width      = img->width;
	int step       = img->widthStep;
	uchar * data    =  (uchar *) img->imageData;
	int channels   = img->nChannels;

	int nheight     = num->height;
	int nwidth      = num->width;
	int nstep       = num->widthStep;
	uchar * ndata    = (uchar *) num->imageData;
	int nchannels   = num->nChannels;
	int i,j,ii,jj;
	bool isS = false;
	int sum = 0,max=0;


	/**
	cvNamedWindow("contour1");
	cvShowImage("contour1", img);  
	cvWaitKey(0);
	cvShowImage("contour1", num);  
	cvWaitKey(0);
	cvDestroyWindow("contour1");
	**/
	for(j=0; j<(width-nwidth ); j+=1){
		for(i=0; i<(height-nheight) ; i+=1){
			isS = true;
			sum = 0;
			for(jj=0; jj<nwidth; jj+=4){
				for(ii=0; ii<nheight ; ii+=4){
					//printf("%d %d\r\n",data[(i+ii)*step+(j+jj)] , ndata[ii*nstep+jj]);
					if (data[(i+ii)*step+(j+jj)*channels+0] != ndata[ii*nstep+jj*nchannels+0]){
						isS = false;
						break;
					}else{
						sum+=1;
					}
				}
				if (!isS){
					//if (sum>max)max=sum;
					break;
				}
			}
			if (isS){
				loc->x = j;
				loc->y = i;
				//printf("Ok max %d\r\n",sum);
				//cvReleaseImage(&img);
				return true;
			}

		}
	}
	//cvReleaseImage(&img);
	//printf("max %d\r\n",max);
	return false;
	
}
void getNextMaxLoc(IplImage* result , int templatWidth,int templatHeight,double minValIn ,double * maxValIn , CvPoint *lastLoc )
{
	int startX = lastLoc->x ;  
    int startY = lastLoc->y ;  
    int endX = lastLoc->x + templatWidth;  
    int endY = lastLoc->y + templatHeight;  
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
	 

       //  double minVal,maxVal;
       // CvPoint minLoc;        //查找result中的最小值 及其所在坐标
    cvMinMaxLoc(result,0,maxValIn,0,lastLoc ,NULL);
        //return maxVal; 
} 
double inSameOneT(IplImage *img ,IplImage * num,CvPoint *loc){
	/**
	cvNamedWindow("contour1");
	cvShowImage("contour1", img);  
	cvWaitKey(0);
	cvShowImage("contour1", num);  
	cvWaitKey(0);
	cvDestroyWindow("contour1");
	**/
	//printf("v1>%d %d\r\n",img->width , num->width);
	int w = img->width - num->width + 1;
	
	int h = img->height - num->height + 1;

	
	if (w<1 && h<1) {
		return inSameOneT(num,img,loc);

	}else if (w<1 || h<1)return 0;
	 
	IplImage* imgResult = cvCreateImage(cvSize(w,h),IPL_DEPTH_32F,1); 
	//printf("v2>%d %d\r\n",img->depth,num->depth);
	cvMatchTemplate(img,num,imgResult,CV_TM_CCOEFF_NORMED); 	 
    double max_val,min_val; 
	CvPoint minLoc;

	cvMinMaxLoc(imgResult, &min_val, &max_val, &minLoc, loc, NULL);  
	//if (max_val>0.8)printf("%f\r\n",max_val);
	cvReleaseImage(&imgResult);
	//printf("t %f\r\n",max_val);
	return max_val;

}
void CallBackEvent(LPVOID handle,char *data){
	Block * r = (Block *) handle;

	//printf("%s\r\n",data);
	char *p ;
	const char *d = " ";
	p = strtok(data,d);
	int i=0;
	int n;
	int val =0;
	r->backVal = -1;
	//r->CallBackVal = -1
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
			r->backVal = n;
			if (r->ClickCoordinate(n,1)){
				val++;
				//printf("OK %d\r\n",n);
				
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

void Block::ClearCoord(){
	//Coord
	vector <Coordinate>().swap(Coord);
}
void CallBackEventArr(LPVOID handle,char *data){

	Block * r = (Block *) handle;
	char *p ;
	const char *d = " ";
	p = strtok(data,d);
	int i=0;
	int n;
	int val =0;
	r->backVal = -1;
	while(p){
		n = atoi(p);
		if (i==0){
			i++;
			p=strtok(NULL,d);
			continue;
		}
		if (n > -1){
			r->backVal = n;
			if (r->ClickCoordinateT(n,1)){
				val++;
			}
		}
		p=strtok(NULL,d);
	}
	r->CallBackVal = val;
}
bool Block::ClickCoordinateT(int _v,int num){

	IplImage *Img = hBitmap2Ipl(Screen());
	bool isOut = false;
	
	//Block *bl =new Block();
	for (vector<Coordinate>::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		if ((*it).v != _v)continue;

		vector <TempleImg*> Tem;
		Tem.push_back((*it).te);

		Block *bl =new Block((*it).rect,0,Tem,0);
		//bl->TempleList.push_back((*it).te);
		int ne = bl->FindOne(Img,true);
		delete bl;
		Tem.clear();
		//vector <Coordinate>().swap(Coord);
		if (ne == -1){
			Coord.erase(it);
			isOut = true;
			break;			 
		}
		
		if ((*it).MouseClick(_v,num) ){
			Coord.erase(it);
			isOut = true;
			break;
		}		
	}

	cvReleaseImage(&Img);
	Img = NULL;
	return isOut;
}
bool Block::ClickCoordinate(int _v,int num){
	if (_v == -1) return Coord[0].MouseClick(Coord[0].v,num);
	for (vector<Coordinate>::iterator it = Coord.begin(); it != Coord.end(); it ++) {
		//if (*it == 0 )continue;
		 

		if ((*it).MouseClick(_v,num) ){
			Coord.erase(it);
			return true;		
		}
	}
	return false;
}
int Block::WaitEvent(){
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