#include "stdafx.h"
#include "coll.h"

Coll::Coll(const char *BaseFile){
	this->LoadRootData(BaseFile);

	this->ScreenImg =  hBitmap2Ipl(Screen());
}
void Coll::Runtest(){
	char file[100];
	for (int num =0;num<100;num++){
		hBitmap2Ipls(Screen(),ScreenImg);
		
		sprintf(file,"Screen_%d.bmp",num);
		cvSaveImage(file,ScreenImg);
		printf("%s\r\n",file);
		Sleep(1000);
	}
}
void Coll::Run(){
	for (int num =0;;){
		hBitmap2Ipls(Screen(),ScreenImg);

		for (vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); it ++) {
			 
			if ((*it)->IsArr == 1 ){
				if ((*it)->GetNumberList(ScreenImg,(*it)->Sep)) num++;
			}else{
				if ((*it)->PerceptionTempleToSame(ScreenImg,num)) num++;
			}
		
		}

		Sleep(500);
	}
}

void Coll::Test(int tag){
	char * file = "E:\\code\\oneRun\\t1\\Screen_11.bmp";
	this->ScreenImg = cvLoadImage(file,CV_LOAD_IMAGE_UNCHANGED);
	for (vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); it ++) {
		(*it)->GetNumberList_test(this->ScreenImg,tag);
	}
}
void Coll::RunScreen(){
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
bool Coll::ScreenSave(const char * file){
	if (!::IsClipboardFormatAvailable(CF_BITMAP)){
		//printf("IsClipboardFormatAvailable\n");
		return false;
	}
	HBITMAP H;
	if (::OpenClipboard (NULL)) {
		H = (HBITMAP)::GetClipboardData(CF_BITMAP);
		//if (this->imgB == NULL) imgB =  hBitmap2Ipl(H);
		//else hBitmap2Ipls(H,imgB );

		hBitmap2Ipls(H,this->ScreenImg );
		cvSaveImage(file,ScreenImg);

		::EmptyClipboard();
		::CloseClipboard ();
	}
	return true;

}
void Coll::LoadRootData(const char * RootData ){

	//printf("filename:%s\r\n",LoadTmpFile);
	fstream In(RootData, ios::in|ios::binary);	
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
			CvPoint ps = {0,0};
			BlockList.push_back(new Block(tmpStr, ps,0));
		}
        In.close();
	}

}