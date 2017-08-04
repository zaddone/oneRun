#include "stdafx.h"
#include "collection.h"

//static UINT	UWM_ClickPrnScrn = ::RegisterWindowMessage("UWM_ClickPrintScreen_B2ABC742-0A63-49c3-9ACB-CF0068027A66");

Collection::Collection(const char *CoordFile ,CString MonitorFile ){
	this->LoadData(CoordFile);
	this->imgB =  hBitmap2Ipl(Screen());
	this->imgA = cvCreateImage(cvSize(imgB->width,imgB->height),imgB->depth,imgB->nChannels);
	cvCopy(imgB,imgA);
	
	 
	//CString cstr(MonitorFile);
	BaseName =  MonitorFile;
 

	CString Cip;
	char ip[15];
	GetPrivateProfileString(_T("Server"),_T("ip"),_T("192.168.1.80"),Cip.GetBuffer(15),15,BaseName);
	CStringToChar(Cip,ip);

	CString Cid;
	char id[15];
	GetPrivateProfileString(_T("Server"),_T("id"),_T("oneRun1"),Cid.GetBuffer(50),50,BaseName);
	CStringToChar(Cid,id);

	int port;
	port = GetPrivateProfileInt(_T("Server"),_T("port"),3333,BaseName);
	this->Client = new client(ip,port,id);
	//sep = 63;
	sep = GetPrivateProfileInt(_T("Info"),_T("sep"),63,BaseName);
	printf("%s %s %d %d\r\n",ip,id,port,sep);
	//this->Client = NULL;
}


Collection::~Collection(){
	cvReleaseImage(&this->imgB);
	cvReleaseImage(&this->imgA);
}
void Collection::Test(int CoordNum,char * filename){
	CoordRange * cr = coordRangeList[CoordNum];
	//cr->UpdateToTempleListForThreshold(140);
	int nums;
	IplImage * imgTag = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);
	if (imgTag==NULL)return;
	IplImage * img = cvCreateImage(cvGetSize(imgTag ),imgTag->depth,1); 
	cvSplit(imgTag,img,0,0,0);

	cvThreshold( img,img, 120, 255, CV_THRESH_BINARY  );
	cvNamedWindow("contour1");      
    
  
	CvSeq* contours = NULL;
    CvMemStorage* storage = cvCreateMemStorage(0);   
	IplImage * imgbak = cvCreateImage(cvGetSize(img ),img->depth,img->nChannels); 
	cvCopyImage(img, imgbak);  
    int count = cvFindContours(imgbak, storage, &contours,sizeof(CvContour),CV_RETR_EXTERNAL);  
	cvShowImage("contour1", img);  
	cvWaitKey(0);
    printf("轮廓个数：%d",count);  
    int idx = 0;  
    char szName[56] = {0};  
    int tempCount=0;
    for (CvSeq* c = contours; c != NULL; c = c->h_next) {  
		CvRect rc =cvBoundingRect(c,0);                                      
		//cvDrawRect(img, cvPoint(rc.x, rc.y), cvPoint(rc.x + rc.width, rc.y + rc.height), CV_RGB(255, 0, 0));  
		
		IplImage* imgNo = cvCreateImage(cvSize(rc.width, rc.height), img->depth, img->nChannels);   
		cvSetImageROI(img, rc);  
		printf("%d %d %d %d\r\n",rc.x,rc.y,rc.width,rc.height);
		cvCopyImage(img, imgNo);
		
		sprintf(szName, "wnd_%d", idx++);   
		cvNamedWindow(szName);   
		cvShowImage(szName, imgNo); //如果想切割出来的图像从左到右排序，或从上到下，可以比较rc.x,rc.y;  
		cvReleaseImage(&imgNo);   
		cvResetImageROI(img);   
   }  


	//cvAdaptiveThreshold(img,img,  255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,41, 5 );



  //  IplImage* img_temp = cvCreateImage(cvGetSize(img), 8, 1);      
  
   // cvThreshold(img, img, 128, 255, CV_THRESH_BINARY);
    
    //cvNamedWindow("contour1");      
    
    cvShowImage("contour1", img);      
  	cvWaitKey(0);
	cvDestroyWindow("contour1");
   
  
 



 
	
	cr->rect = cvRect(0,0,img->width,img->height);
	if (cr->UpRectArr(img)>0){
		nums = coordRangeList[17]->GetDataNum();
		printf("num %d\r\n",nums);
	}
	/**
	while(true){
		hBitmap2Ipls(Screen(),imgB);
		
		cr->rect = cvRect(0,0,imgB->width,imgB->height);
			//int n = cr->UpRectArr(imgTag);
		if (cr->UpRectArr(imgB)>0){
			nums = coordRangeList[17]->GetDataNum();
			printf("num %d\r\n",nums);
		}
		Sleep(100);
	}
	**/
}
void Collection::Init(){
	WritePrivateProfileString (_T("Server"),_T("ip"),_T("192.168.1.80") ,BaseName);
	WritePrivateProfileString (_T("Server"),_T("id"),_T("oneRun1") ,BaseName);
	WritePrivateProfileString(_T("Server"),_T("port"),_T("3333"),BaseName);

	WritePrivateProfileString(_T("Info_4"),_T("sep"),_T("65"),BaseName);
	WritePrivateProfileString(_T("Info_4"),_T("path"),_T("data_4"),BaseName);

	WritePrivateProfileString(_T("Info_5"),_T("path"),_T("data_5"),BaseName);


}
void Collection::RunMonitor(){
	bool Self = true;
	//int i =0;
	//int lastN = -1;
	//int tmpN;
	while(true){
		//printf("i %d\r\n",i);
		//i++;
		hBitmap2Ipls(Screen(),imgB);

		Monitor(Self);
		//if (tmpN >= 0 && tmpN <4) lastN = tmpN;
		cvCopy(imgB,imgA);
		Sleep(300);
	}
}
void Collection::RunColl(){
	//Screen();
	for (int num =0;;){
		hBitmap2Ipls(Screen(),imgB);

		for (vector<CoordRange*>::iterator it = coordRangeList.begin(); it != coordRangeList.end(); it ++) {
			 
			if ((*it)->PerceptionTempleToSame(imgB,num)) num++;
		
		}

		Sleep(300);
	}


}
void Collection::RunScreen(){
	char file[100];
	for (int i = 0;;){
		Sleep(1000);
		sprintf(file,"Screen_%d.bmp",i);
		if (ScreenSave(file)){
			i++;
			printf("Screen Save to %s\r\n",file);
		}
		//i++;
	}

}
bool Collection::ScreenSave(const char * file){
	if (!::IsClipboardFormatAvailable(CF_BITMAP)){
		//printf("IsClipboardFormatAvailable\n");
		return false;
	}
	HBITMAP H;
	if (::OpenClipboard (NULL)) {
		H = (HBITMAP)::GetClipboardData(CF_BITMAP);
		//if (this->imgB == NULL) imgB =  hBitmap2Ipl(H);
		//else hBitmap2Ipls(H,imgB );

		hBitmap2Ipls(H,imgB );
		cvSaveImage(file,imgB);

		::EmptyClipboard();
		::CloseClipboard ();
	}
	return true;
	
	//return H;
}
void Collection::LoadData(const char * LoadTmpFile ){

	//printf("filename:%s\r\n",LoadTmpFile);
	fstream In(LoadTmpFile, ios::in|ios::binary);	
	if (In.is_open())   
    {  		
		printf("open ok\r\n");
		char tmpStr[50];
		int UserArrLen = 0;
		while (!In.eof() ){
			In.getline(tmpStr ,50,'\n');
			if (strlen(tmpStr)==0){
				break;
			}
			//printf("%s\r\n",tmpStr);
			//rectInfo *UserA  = new rectInfo(tmpStr );
			coordRangeList.push_back(new CoordRange(tmpStr ));
		}
        In.close();
	}
}
void Collection::Monitor(bool & Self  ){
	
	/**
	int n = -1;
	int v;
	for(int i = 0;i<3;i++){
		v = coordRangeList[i]->UpSelf(imgB);
		if (v != -1) {
			n = i;
			break;
		}
	}
	**/

	int n = coordRangeList[5]->UpRect(imgB);	
	if (n == -1){
		//UserArr[15] -> ClearCoordinate();
		if( coordRangeList[15]->UpRect(imgB )!= -1 )	{
			Self = true;
			//this->NowRect = NULL;
			coordRangeList[15]->ClickCoordinate(0,1);
			return;
		}
		//UserArr[16] -> ClearCoordinate();
		if( coordRangeList[16]->UpRect(imgB)!= -1 )	{
			Self = true;
			//this->NowRect = NULL;
			coordRangeList[16]->ClickCoordinate(0,1);
			return;
		}
	}else if (n < 3){
		//if (Self )Self = false;	
		if (coordRangeList[n]->UpRect(imgB )!= -1 ){
			coordRangeList[n]->SendToServer(SendData,Client,0);
		}
		return;
	}else if (n == 3 ){
		int nums = 0;
		if (coordRangeList[17]->UpRectArr(imgB)>0){
			nums = coordRangeList[17]->GetDataNum();
		}
		if (Self){
			//bool is14 = false ;

			if ( coordRangeList[9]->UpRect(imgB )!= -1){

				if (coordRangeList[4]->UpRectArr(imgB,sep)>0 ){
					if (coordRangeList[3]->UpRect(imgB)!= -1){
						coordRangeList[4]->Coord.push_back(coordRangeList[3]->Coord[0]);
						//UserArr[4]->Coord.insert(UserArr[4]->Coord.end(), UserArr[3]->Coord.begin(),UserArr[3]->Coord.end());
					}
					Client->InitCallBack(CallBackEventC,coordRangeList[4]);
					coordRangeList[4]->SendToServerNum(nums,SendData,Client,5);
					//c->InitCallBack(CallBackEvent,UserArr[4]);
					//UserArr[4]->SendDataForNum(nums,SendData,c,5);
					if (coordRangeList[4]->WaitEvent()>0 ){
						//Sleep(100);
						coordRangeList[9]->ClickCoordinate(0,1);
					}else if (coordRangeList[10]->UpRect(imgB) != -1){
						coordRangeList[10]->ClickCoordinate(0,2);
					}else{
						printf("find not 9 or 10\r\n");
					}
				}
				return;
			}else if ( coordRangeList[6]->UpSelf(imgB,imgA) != -1){
			 
				if ( coordRangeList[7]->UpRect(imgB ) != -1){
					coordRangeList[6]->Coord.push_back(coordRangeList[7]->Coord[0]);
					//UserArr[6]->Coord.insert(UserArr[6]->Coord.end(), UserArr[7]->Coord.begin(),UserArr[7]->Coord.end());
				}
				if ( coordRangeList[8]->UpRect(imgB ) != -1){
					coordRangeList[6]->Coord.push_back(coordRangeList[8]->Coord[0]);
					//UserArr[6]->Coord.insert(UserArr[6]->Coord.end(), UserArr[8]->Coord.begin(),UserArr[8]->Coord.end());
				}		
				
				int base = 0;
				if (coordRangeList[18]->UpRectArr( imgB  )>0){
					base = coordRangeList[18]->GetDataNum();
					coordRangeList[18]->SendToServer(SendData,Client,base);
					//SendData(c,&(coordRangeList[18]->isUp),18,NULL,0,base);
				}
				

				//if ( UserArr[3]->UpRect(src,true,true) != -1)is14= true;

				//printf("te1");
				if (coordRangeList[4]->UpRectArr(imgB,sep)>0){
					if ( coordRangeList[3]->UpRect(imgB ) != -1){
						coordRangeList[4]->Coord.push_back(coordRangeList[3]->Coord[0]);
						//UserArr[4]->Coord.insert(UserArr[4]->Coord.end(), UserArr[3]->Coord.begin(),UserArr[3]->Coord.end());
					}
					//printf("te2");
					Client->InitCallBack(CallBackEventC,coordRangeList[6]);
					coordRangeList[4]->SendToServerNum(nums,SendData,Client,6);
					//printf("te3");
					if (coordRangeList[6]->WaitEvent()!= -1){
						//Self = false;
						return;
					}
				}
				return;
			}
		
		}
		
		int is11 =   coordRangeList[11]->UpRect(imgB ) ;
		if (is11 != -1) if (Self)Self = false;
		if ( is11 > 0 ){
			coordRangeList[11]->Coord[0].v = is11+ 30;
			int is12 = coordRangeList[12]->UpRect(imgB) ;
			if (is12 != -1){
				coordRangeList[12]->Coord[0].v =is12 + 30;
				coordRangeList[11]->Coord.push_back(coordRangeList[12]->Coord[0]);
			}
			if (coordRangeList[3]->UpRect(imgB) != -1){
				//Self = false;
				
				if (coordRangeList[4]->UpRectArr(imgB,sep )>0){
					 
					coordRangeList[4]->Coord.push_back(coordRangeList[3]->Coord[0]);
					coordRangeList[4]->Coord.insert(coordRangeList[4]->Coord.end(),coordRangeList[11]->Coord.begin(),coordRangeList[11]->Coord.end());
					Client->InitCallBack(CallBackEventC,coordRangeList[4]);
					coordRangeList[4]->SendToServerNum(nums,SendData,Client,9);//(nums,SendData,c,9);
					
					if (coordRangeList[4]->WaitEvent() > 0 ){
						//UserArr[11]->ClickCoordinate(is11,1,false);
						return;
					}
					//UserArr[4]->Coord.push_back(UserArr[3]->Coord[0]);
				}
			}


			//coordRangeList[11]->ClickCoordinate(is11,1);
			return;
		}else if (is11 ==  0){


			//UserArr[12]->Coord = UserArr[11]->Coord;

			int is12 = coordRangeList[12]->UpRect(imgB) ;
			if (is12 == -1) return;
			coordRangeList[11]->Coord.push_back(coordRangeList[12]->Coord[0]);

			
			int is13 = coordRangeList[13]->UpRect(imgB) ;
			if (is13 != -1){ 
				coordRangeList[11]->Coord.push_back(coordRangeList[13]->Coord[0]);
				int is14 = coordRangeList[14]->UpRect(imgB) ;
				if (is14 != -1) coordRangeList[11]->Coord.push_back(coordRangeList[14]->Coord[0]);
			}
			Client->InitCallBack(CallBackEventC,coordRangeList[11]);
			if (coordRangeList[4]->UpRectArr(imgB,sep )>0){
				coordRangeList[4]->SendToServerNum(nums,SendData,Client,7);
				if (coordRangeList[11]->WaitEvent()!= -1)return;
			}

		}else  if (is11 ==  -1){			
			if (coordRangeList[3]->UpRect(imgB) != -1){
				if (Self){
					Self = false;		
				}
				if (coordRangeList[4]->UpRectArr(imgB,sep)>0){
					 
					coordRangeList[4]->Coord.push_back(coordRangeList[3]->Coord[0]);
					//UserArr[4]->Coord.insert(UserArr[4]->Coord.end(), UserArr[3]->Coord.begin(),UserArr[3]->Coord.end());
					Client->InitCallBack(CallBackEventC,coordRangeList[4]);
					coordRangeList[4]->SendToServerNum(nums,SendData,Client,8);

					coordRangeList[4]->WaitEvent();
					//Sleep(100);
					return;
					
				}
			}			
		}
	}else if (n == 4){
		if (Self) return;
		/**
		int b = 19;
		int c;
		for (int i = 0;i<4;i++){
			if (i == lastN) continue;
			c = b+i;
			if ( coordRangeList[c]->UpRectArr(imgB)>0)
		}
		**/

	}
}
