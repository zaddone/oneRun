#include "stdafx.h"
#include "Monitor.h"

Monitor::Monitor(const char *RootData ,CString Base){
	BaseName =  Base;
	ScreenImg = hBitmap2Ipl(Screen());

	CString Cip;
	char ip[15];
	GetPrivateProfileString(_T("Server"),_T("ip"),_T("192.168.1.80"),Cip.GetBuffer(15),15,BaseName);

	int len = WideCharToMultiByte(CP_ACP,0,Cip,-1,NULL,0,NULL,NULL);  
	WideCharToMultiByte(CP_ACP,0,Cip,-1,ip,len,NULL,NULL );  

	//CStringToChar(Cip,ip);

	CString Cid;
	char id[15];
	GetPrivateProfileString(_T("Server"),_T("id"),_T("oneRun1"),Cid.GetBuffer(50),50,BaseName);
	len = WideCharToMultiByte(CP_ACP,0,Cid,-1,NULL,0,NULL,NULL);  
	WideCharToMultiByte(CP_ACP,0,Cid,-1,id,len,NULL,NULL );  
	//CStringToChar(Cid,id);

	int port;
	port = GetPrivateProfileInt(_T("Server"),_T("port"),3333,BaseName);
	this->Client = new client(ip,port,id);

	 
	this->BaseNum = GetPrivateProfileInt(_T("Info"),_T("BaseNum"),100,BaseName);
	this->Coll = GetPrivateProfileInt(_T("Info"),_T("Coll"),0,BaseName);

	this->LoadRootData(RootData);
	//isDiffNum=false;
}
Monitor::~Monitor(){
	for (vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); it ++) {
		delete (*it);
		*it = NULL;
	}
	//vector <Block*>().swap(BlockList);
	BlockList.clear();
	delete Client;
	cvReleaseImage(&ScreenImg);
}
void Monitor::LoadRootData(const char * RootData ){

	//printf("filename:%s\r\n",LoadTmpFile);
	fstream In(RootData, ios::in|ios::binary);	
	if (In.is_open())   
    {  		
		printf("open ok %s\r\n",RootData);
		char tmpStr[50];
		int UserArrLen = 0;
		CvPoint ps = {0,0};
		while (!In.eof() ){
			In.getline(tmpStr ,50,'\n');
			if (strlen(tmpStr)==0){
				break;
			}
			//printf("%s\r\n",tmpStr);
			//rectInfo *UserA  = new rectInfo(tmpStr );
			//Block *bl = new Block(tmpStr, ps) ;
			BlockList.push_back(new Block(tmpStr, ps,Coll));
		}
        In.close();
	}

}

void Monitor::Run(){
	//bool Self = true;
	int lastn = -1;
	int lastN = -1;
	//BaseNum =  -1;

	//lastn = n;
	this->InitUserTempInfoArr();
	//BaseNum = -1;
	//int i = 0;
	for (int i =0;;){
		
		//printf("i %d\r\n",i);
		//i++;
		//HBITMAP hb = Screen();
		//hBitmap2Ipls(hb,ScreenImg);
		hBitmap2Ipls(Screen(),ScreenImg);
		
		this->Start(lastn,lastN);
		//if (tmpN >= 0 && tmpN <4) lastN = tmpN;
		//cvCopy(imgB,imgA);
		Sleep(500);
		i++;
	}
}
void Monitor::Start( int & lastn,int &lastN){
	


	int n = this->BlockList[4]->FindChildOne(this->ScreenImg);
	if (n <0 ){
			if (-1 != this->BlockList[8]->Child[6]->FindOne(this->ScreenImg,true)){
				printf("find 8 6\r\n");
				this->BlockList[8]->Child[6]->ClickCoordinate(0,1);
				//Self = true;
				lastn = n;
				//BaseNum = -1;
				this->InitUserTempInfoArr();
			}else if (-1 != this->BlockList[8]->Child[7]->FindOne(this->ScreenImg)){
				printf("find 8 7\r\n");
				this->BlockList[8]->Child[7]->ClickCoordinate(0,1);
				// Self = true;
				 lastn = n;
				 this->InitUserTempInfoArr();
				// BaseNum = -1;
			}else if (-1 != this->BlockList[8]->Child[15]->FindOne(this->ScreenImg)){
				printf("find 8 7\r\n");
				this->BlockList[8]->Child[15]->ClickCoordinate(0,1);
				// Self = true;
				 lastn = n;
				 this->InitUserTempInfoArr();
				 //BaseNum = -1;
			}

	}else if ( n == 4 ){
		if (lastn <0 ) return;
		int I = lastn +5;
		if (this->BlockList[I]->Child.size()==0) return;
		/**
		int lastp = -1;
		if (lastn ==3){
			lastp = BlockList[lastn]->backVal;
		}else{
			if (this->BlockList[lastn]->Coord.size()>0){
				lastp = this->BlockList[lastn]->Coord[0].v ;
			}
		}
		if (lastp == -1) return;
		**/
		int last = this->userTempInfoArr[lastn].val;
		this->BlockList[I]->Child[0]->FindArr(this->ScreenImg,true);
		int now = this->BlockList[I]->Child[0]->GetDataNum();
		//return;
		if (now != last){
			this->userTempInfoArr[lastn].val = now;
			//int d = (last - now)/BaseNum;
			//this->userTempInfoArr[lastn].isDiffNum  =true;
			int iI,no,f,nt = -1;
			for (int i=0;i<3;i++){
				

				iI = i+5;
				if (this->userTempInfoArr[i].Class == -1){
					//int ni = n+5;
					BlockList[iI]->Child[0]->FindArr(this->ScreenImg,true);
					userTempInfoArr[i].val = BlockList[iI]->Child[0]->GetDataNum();
					//this->BlockList[ni]->Child[1]->IsArr =1;
					int cl = BlockList[iI]->Child[1]->FindOne(this->ScreenImg,true);
					if (cl != -1){
						userTempInfoArr[i].Class = cl;
						 
						SendData(Client,NULL,i,&cl,1,100);
					}
					continue;
				}
				if (i == lastn)continue;
				if (this->userTempInfoArr[i].ishu) continue;

				
				this->BlockList[iI]->Child[0]->FindArr(this->ScreenImg,true);
				no = this->BlockList[iI]->Child[0]->GetDataNum();

				if (no == this->userTempInfoArr[i].val) continue;

				this->userTempInfoArr[i].val = no;
				//this->BlockList[iI]->Child[2]->IsArr = 1;
				//f = this->BlockList[iI]->Child[2]->FindOne(this->ScreenImg,true);
				//if (f == 30 ){
					//nt = 3;
				//	break;
				//}else{
					this->userTempInfoArr[i].ishu = true;
					SendData(Client,NULL,i,&no,1,1);
				//}
			}
			/**
			if (nt>0){
				//this->userTempInfoArr[i]
				if (lastn != 3){
					if (this->BlockList[lastn]->Coord.size()>0){
						int val = this->BlockList[lastn]->Coord[0].v ;
						//BlockList[lastn]->SendToServer(SendData,Client,0);
						SendData(Client,NULL,iI-5,&val,1,nt);
					}
				}else{
					int val = this->BlockList[lastn]->backVal;
					if (val >=0 && val < 27){
						SendData(Client,NULL,iI-5,&val,1,nt);
					}
				}

				//BlockList[iI]->SendToServer(SendData,Client,1);
				//int val = this->BlockList[lastn]->Coord[0].v ;
				//SendData(Client,&BlockList[iI]->isUp,iI - 5,&val,1,nt);
			}
			**/
		}
		 
	}else{

		if (n <3){		

			int N = -1;
			if (n == lastn ){
				BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);
				N = BlockList[9]->Child[1]->GetDataNum();
				if (N == lastN) return;
			}
			
	
			if (this->userTempInfoArr[n].Class == -1){
				int ni = n+5;
				if (this->BlockList[ni]->Child.size()>0){				
					
					BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
					userTempInfoArr[n].val = BlockList[ni]->Child[0]->GetDataNum();
					//this->BlockList[ni]->Child[1]->IsArr =1;

					int cl = BlockList[ni]->Child[1]->FindOne(this->ScreenImg,true);
					if (cl != -1){
						userTempInfoArr[n].Class = cl;
						SendData(Client,NULL,n,&cl,1,100);
					}

					//userTempInfoArr[n].Class = BlockList[ni]->Child[1]->FindOne(this->ScreenImg,true);
				}
			} 
			if (this->userTempInfoArr[lastn].Class != -1){
				if (lastn >=0 && lastn != 3 ){
					int ni = lastn +5;
					if(this->BlockList[lastn]->Child.size() == 0){
						BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
						int val = BlockList[ni]->Child[0]->GetDataNum();
						if (val != userTempInfoArr[lastn].val){
							this->userTempInfoArr[lastn].ishu=true;
							SendData(Client,NULL,lastn,&val,1,1);
						}
					}
				}
			}
			
	 
			
			
			if (this->BlockList[n]->FindOne(this->ScreenImg) != -1){
				
				if (N == -1){
					BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);
					N = BlockList[9]->Child[1]->GetDataNum();
				}
				 
				if (lastn == -1){
					if (n>0){
					lastn = n-1;
					}else{
						lastn = 3;
					}
				}
				bool isU =( lastN == N  && N != 55);
				int UpV = 2;
				if (!isU) {
					if (this->userTempInfoArr[n].ishu == true){
						isU = true;
						UpV = 3;
						this->userTempInfoArr[n].ishu = false;
					}
				}
				if (isU){
					this->userTempInfoArr[n].peng++;
				//if (lastN == N || this->userTempInfoArr[n].ishu == true ){

					 
					//BlockList[n]->SendToServer(SendData,Client,1);
					if (lastn != 3){
						if (this->BlockList[lastn]->Coord.size()>0){
							int val = this->BlockList[lastn]->Coord[0].v ;
							//BlockList[lastn]->SendToServer(SendData,Client,0);
							SendData(Client,NULL,n,&val,1,UpV);
						}
					}else{
						int val = this->BlockList[lastn]->backVal;
						if (val >=0 && val < 27){
							SendData(Client,NULL,n,&val,1,UpV);
						}
					}
						//SendData(Client,&BlockList[n]->isUp,BlockList[n]->ID,&val,1,2);
				}
				BlockList[n]->SendToServer(SendData,Client,0);
				
			//  Self = false;
				//if (BaseNum>0){
				
				//}
				
				
				
				
				lastN = N ;
				lastn = n;
			}

		}else if (n == 3){
			BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);
			int N = BlockList[9]->Child[1]->GetDataNum();
			this->BlockList[9]->Child[1]->ID = 91;
			this->BlockList[9]->Child[1]->SendToServer(SendData,Client,N);
			if (N == 55){
				//BlockList[9]->Child[0]->FindArr(this->ScreenImg);
				//BaseNum= BlockList[9]->Child[0]->GetDataNum();
				lastn = -1;
			//}
			/**
			if (BaseNum <0){
				BlockList[9]->Child[1]->FindArr(this->ScreenImg);
				BaseNum= BlockList[9]->Child[1]->GetDataNum();
				printf("Num-> %d\r\n",BaseNum);
				//return;
				if (BaseNum == 55) lastn = -1;
			}
			**/
			//BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);

		

		
			//BaseNum= BlockList[9]->Child[1]->GetDataNum();
		//	if (lastn < 0 ){
				FindSelfBegin();
				FindSelf(lastn);
			}else{
				FindSelf(lastn);
				//lastn =  n;
				
			}
			lastN = N;
			
			/**
			if (lastn < 0 ){
				if (FindSelfBegin()){

					if (FindSelf()){
						lastn = n;
					}
				}
			
			}else{
				FindSelf();
				//if (!FindSelfBegin()){
					lastn =  n;
				//}			 
			}
			**/
		}

		
	}
	return;
}
bool Monitor::FindSelf(int & lastn){
	 
	if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
		printf("find not 12\r\n");
		return false;
	}
	if (0 == this->BlockList[8]->Child[0]->FindArr(this->ScreenImg,true)) {
			printf("find not 8 0\r\n");
			//return;
	}

	this->BlockList[3]->Coord.clear();
	this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
	if (-1 != this->BlockList[8]->Child[13]->FindOne(this->ScreenImg,true)){

		lastn = 3;
		this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);
		if (this->BlockList[8]->Child[8]->FindOne(this->ScreenImg,true) == -1){
			int k = 8;
			if (this->checkGang(this->BlockList[3]->Coord)){
				if ( -1 != this->BlockList[8]->FindOne(this->ScreenImg,true)){
					this->BlockList[8]->Coord[0].v+=30;
					this->BlockList[3]->Coord.push_back(this->BlockList[8]->Coord[0]);
					k=9;
				}
			}

			Client->InitCallBack(CallBackEvent,BlockList[3]);
			this->BlockList[3]->SendToServerNum(this->BlockList[8]->Child[0]->GetDataNum(),SendData,Client,k);
			this->BlockList[3]->WaitEvent();

			return true;
		}else{
			
			this->BlockList[8]->Child[8]->Coord[0].v += 30 ;
			this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[8]->Coord[0]);
			if (this->BlockList[8]->Child[9]->FindOne(this->ScreenImg,true) != -1){
				this->BlockList[8]->Child[9]->Coord[0].v += 30 ;
				this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[9]->Coord[0]);				
			}			
			
			Client->InitCallBack(CallBackEvent,this->BlockList[3]);
			this->BlockList[3]->SendToServerNum(this->BlockList[8]->Child[0]->GetDataNum(),SendData,Client,9);
			this->BlockList[3]->WaitEvent();
			return true;
		}
	}else{
		if (this->BlockList[8]->Child[8]->FindOne(this->ScreenImg,true) == -1){
			return false;
		}
		if (this->BlockList[8]->Child[9]->FindOne(this->ScreenImg,true) == -1){
			printf("find not 8 9\r\n");
			return false;
		}
		this->BlockList[8]->Child[8]->Coord.push_back(this->BlockList[8]->Child[9]->Coord[0]);
		if (this->BlockList[8]->Child[10]->FindOne(this->ScreenImg,true) != -1){
			this->BlockList[8]->Child[8]->Coord.push_back(this->BlockList[8]->Child[10]->Coord[0]);
			if (this->BlockList[8]->Child[11]->FindOne(this->ScreenImg,true) != -1){
				this->BlockList[8]->Child[8]->Coord.push_back(this->BlockList[8]->Child[11]->Coord[0]);
			}
		}
		Client->InitCallBack(CallBackEvent,this->BlockList[8]->Child[8]);
		this->BlockList[3]->SendToServerNum(this->BlockList[8]->Child[0]->GetDataNum(),SendData,Client,7);
		this->BlockList[3]->WaitEvent();
		return true;		
	}
	return false;
}
bool Monitor::checkGang(vector <Coordinate>  C){
	int val[27] = {0};
	for (vector<Coordinate >::iterator it = C.begin(); it != C.end(); it ++) {
		val[(*it).v]++;
		if (val[(*it).v] >3 ){
			return true;
		}
	}
	return false;
}
bool Monitor::FindSelfBegin(  ){

	if (-1 != this->BlockList[8]->Child[1]->FindOne(this->ScreenImg,true)){
		
		if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
			printf("find not 12\r\n");
			return false;
		}
		this->BlockList[3]->Coord.clear();
		this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
		if (-1 != this->BlockList[8]->Child[13]->FindOne(this->ScreenImg)){
			this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);
		}
		//this->BlockList[8]->ID = 5;
		Client->InitCallBack(CallBackEvent,BlockList[3]);
		this->BlockList[3]->SendToServerNum(0,SendData,Client,5);
		if (BlockList[3]->WaitEvent()>0 ){
			//Sleep(100);
			BlockList[8]->Child[1]->ClickCoordinate(0,1);
			//return true;
		}else if (BlockList[8]->Child[14]->FindOne(this->ScreenImg,true) != -1){
			BlockList[8]->Child[14]->ClickCoordinate(0,2);
		}else{
			printf("find not 0 or 1\r\n");
		}
	}else if (-1 != this->BlockList[8]->Child[2]->FindOne(this->ScreenImg,true)){
		if (-1 == this->BlockList[8]->Child[3]->FindOne(this->ScreenImg,true)){
			printf("find not 3\r\n");
			return false;
		}
		this->BlockList[8]->Child[3]->Coord[0].v =1;
		this->BlockList[8]->Child[2]->Coord.push_back(this->BlockList[8]->Child[3]->Coord[0]);
		if (-1 == this->BlockList[8]->Child[4]->FindOne(this->ScreenImg,true)){
			printf("find not 4\r\n");
			return false;
		}
		this->BlockList[8]->Child[4]->Coord[0].v =2;
		this->BlockList[8]->Child[2]->Coord.push_back(this->BlockList[8]->Child[4]->Coord[0]);
		if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
			printf("find not 12\r\n");
			return false;
		}
		this->BlockList[3]->Coord.clear();
		this->BlockList[3]->CallBackVal = -1;
		this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
		//bool is13 = false;
		if (-1 != this->BlockList[8]->Child[13]->FindOne(this->ScreenImg)){
			this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);
			//is13=true;
			 
		}
		if (0 == this->BlockList[8]->Child[0]->FindArr(this->ScreenImg,true)) {
			printf("find not 8 0\r\n");
			//return;
		}
		//int nums = this->BlockList[8]->Child[14]->GetDataNum();
		
		if (0 < this->BlockList[9]->Child[0]->FindArr(this->ScreenImg,true)) {
			//printf("find not 9 0\r\n");
			//return;
			BaseNum = this->BlockList[9]->Child[0]->GetDataNum();
		} 
		
		//if (bn )
		this->BlockList[9]->Child[0]->ID = 90;
		this->BlockList[9]->Child[0]->SendToServer(SendData,Client,BaseNum);
		

		//BlockList[9]->Child[1]->FindArr(this->ScreenImg);
		//BaseNum= BlockList[9]->Child[1]->GetDataNum();
		Client->InitCallBack(CallBackEvent,this->BlockList[8]->Child[2]);
		this->BlockList[3]->SendToServerNum(this->BlockList[8]->Child[0]->GetDataNum(),SendData,Client,6);
		//printf("te3");
		if (this->BlockList[8]->Child[2]->WaitEvent()>0){

			//if (is13) FindSelf(lastn);
			return true;
		}

	}
	return false;
}

void Monitor::InitUserTempInfoArr(){
	for (int i = 0;i<4;i++){
		this->userTempInfoArr[i].ishu = false;
		this->userTempInfoArr[i].peng = 0;
		this->userTempInfoArr[i].type = 0;
		this->userTempInfoArr[i].val = 0;
		this->userTempInfoArr[i].Class = -1;
		this->userTempInfoArr[i].isDiffNum = false;
	} 
}
