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
	printf("coll %d\r\n",Coll);
	this->LoadRootData(RootData);
	//isDiffNum=false;
}
Monitor::~Monitor(){
	for (vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); it ++) {
		delete (*it);
		*it = NULL;
	}
	vector <Block*>().swap(BlockList);
	//BlockList.clear();
	delete Client;
	cvReleaseImage(&ScreenImg);
}
void Monitor::LoadRootData(const char * RootData ){

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
			BlockList.push_back(new Block(tmpStr, ps,Coll));
		}
        In.close();
	}

}

void Monitor::Run(){

	int lastn = -1;
	int lastN = -1;
	this->InitUserTempInfoArr();
	for (int i =0;;){
		hBitmap2Ipls(Screen(),ScreenImg);		
		this->StartRun(lastn,lastN);
		Sleep(200);
		i++;
	}
}

void Monitor::StartRun( int & lastn,int &lastN ){
	int n = this->BlockList[4]->FindChildOne(this->ScreenImg,true);
	if (n == -1){
		if (-1 != this->BlockList[8]->Child[6]->FindOne(this->ScreenImg,true)){
			printf("find 8 6\r\n");
			this->BlockList[8]->Child[6]->ClickCoordinate(0,1);
		}else if (-1 != this->BlockList[8]->Child[7]->FindOne(this->ScreenImg,true)){
			printf("find 8 7\r\n");
			this->BlockList[8]->Child[7]->ClickCoordinate(0,1);

		}else if (-1 != this->BlockList[8]->Child[15]->FindOne(this->ScreenImg,true)){
			printf("find 8 7\r\n");
			this->BlockList[8]->Child[15]->ClickCoordinate(0,1);

		}
	}else if (n == 4){
		//if (lastn == 3 || lastn <0) return;
		for (int i = 0;i<3;i++){
			int ni = i+5;
			if (this->userTempInfoArr[i].Class == -1){				
				int cl = BlockList[ni]->Child[1]->FindOne(this->ScreenImg,true);
				if (cl != -1){
					BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
					userTempInfoArr[i].val = BlockList[ni]->Child[0]->GetDataNum();
					userTempInfoArr[i].Class = cl;
					SendData(Client,NULL,i,&cl,1,100);
				}
			}
		}
		
		if (this->userTempInfoArr[lastn].Class == -1)return;
		int ni = lastn +5;
		BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
		int val = BlockList[ni]->Child[0]->GetDataNum();
		if (val == userTempInfoArr[lastn].val)return;

		userTempInfoArr[lastn].val = val;
		for (int i = 0;i<3;i++){
			if (i == lastn) continue;
			if (userTempInfoArr[i].isDiffNum) continue;
			ni = i+5;
			BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
			int val = BlockList[ni]->Child[0]->GetDataNum();
			if (userTempInfoArr[i].val != val){
				userTempInfoArr[i].val = val;

				BlockList[ni]->Child[2]->FindOne(this->ScreenImg,true);
				BlockList[ni]->Child[3]->FindOne(this->ScreenImg,true);
				BlockList[ni]->Child[4]->FindOne(this->ScreenImg,true);
				BlockList[ni]->Child[5]->FindOne(this->ScreenImg,true);
				BlockList[ni]->Child[6]->FindOne(this->ScreenImg,true);
				//if (i != lastn)	userTempInfoArr[i].isDiffNum = true;
			}
		}
			
		

		return;
	}else if (n == 3){
		BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);
		int N = BlockList[9]->Child[1]->GetDataNum();
		if (N != lastN){
			this->BlockList[9]->Child[1]->ID = 91;
			this->BlockList[9]->Child[1]->SendToServer(SendData,Client,N);
			lastN = N;
		}
		if (lastN == 55 ){

			if (!this->FindBegin1()){
				if (!this->FindBegin2()){
					
					if (!FindOut())
						this->FindSee();
				//}else
				//	lastn = n;
				}else{
					this->InitUserTempInfoArr();
				//	int lastn = -1;
				}
			}

		}else{
			if (!this->FindOut())
				this->FindSee();
			
		}
		
		lastn = n;
		//lastN = N ;
		//lastn = n;
	}else{
		
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
				int cl = BlockList[ni]->Child[1]->FindOne(this->ScreenImg,true);
				if (cl != -1){
					userTempInfoArr[n].Class = cl;
					SendData(Client,NULL,n,&cl,1,100);
				}
			}
		}
		
		if(this->userTempInfoArr[n].isDiffNum){
			this->userTempInfoArr[n].peng++;
			this->userTempInfoArr[n].isDiffNum = false;
			if (lastn == n){

			}else if (lastn != 3){
				if (this->BlockList[lastn]->Coord.size()>0){
					int val = this->BlockList[lastn]->Coord[0].v ;
					SendData(Client,NULL,n,&val,1,3);
				}
			}else{
				int val = this->BlockList[lastn]->backVal;
				if (val >=0 && val < 27)SendData(Client,NULL,n,&val,1,3);
			}
		}

		
		//printf("start %d\r\n",n);
		if (this->BlockList[n]->FindOne(this->ScreenImg,true) == -1){
			int ni = n+5;
			BlockList[ni]->Child[2]->FindOne(this->ScreenImg,true);
			BlockList[ni]->Child[3]->FindOne(this->ScreenImg,true);
			BlockList[ni]->Child[4]->FindOne(this->ScreenImg,true);
			BlockList[ni]->Child[5]->FindOne(this->ScreenImg,true);
			BlockList[ni]->Child[6]->FindOne(this->ScreenImg,true);

			/**
			int ni = n +5;
			BlockList[ni]->Child[0]->FindArr(this->ScreenImg,true);
			int val = BlockList[ni]->Child[0]->GetDataNum();
			if (userTempInfoArr[n].val != val){
				userTempInfoArr[n].val = val;
				//userTempInfoArr[n].isDiffNum = true;
				lastn = n;
				//lastN = N ;
				for (int i=0;i<3;i++){
					if (userTempInfoArr[n].isDiffNum) continue;

					BlockList[i+5]->Child[0]->FindArr(this->ScreenImg,true);
					userTempInfoArr[i].val = BlockList[i+5]->Child[0]->GetDataNum();
				}
			}
			**/
			return;
		}

		if (N == -1){
			BlockList[9]->Child[1]->FindArr(this->ScreenImg,true);
			N = BlockList[9]->Child[1]->GetDataNum();
		}			



		if (lastn >=0 && lastn != n){
			if ( lastN == N ){
				this->userTempInfoArr[n].peng++;
				if (lastn != 3){
					if (this->BlockList[lastn]->Coord.size()>0){
						int val = this->BlockList[lastn]->Coord[0].v ;
						SendData(Client,NULL,n,&val,1,2);
					}
				}else{
					int val = this->BlockList[lastn]->backVal;
					if (val >=0 && val < 27)SendData(Client,NULL,n,&val,1,2);
				}			 
			}
		}
		//printf("send %d %d\r\n",this->BlockList[lastn]->Coord[0].v,n);
		BlockList[n]->SendToServer(SendData,Client,0);				
		lastN = N ;
		lastn = n;
		//be = 3;
			
	}
}
bool Monitor::FindBegin1(){
	
	if (-1 == this->BlockList[8]->Child[1]->FindOne(this->ScreenImg,true)) return false;
	if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
		printf("find not 12\r\n");
		return true;
	}
	this->BlockList[3]->CallBackVal = -1;
	//this->BlockList[3]->ClearCoord();
	//vector <Coordinate>().swap(Coord);
	this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
	if (-1 != this->BlockList[8]->Child[13]->FindOne(this->ScreenImg,true)){
		this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);
	}
	//this->BlockList[8]->ID = 5;
	Client->InitCallBack(CallBackEventArr,BlockList[3]);
	//Client->InitCallBack(CallBackEvent,BlockList[3]);
	this->BlockList[3]->SendToServerNum(0,SendData,Client,5);
	if (BlockList[3]->WaitEvent()!=0 ){
		//Sleep(100);
		BlockList[8]->Child[1]->ClickCoordinate(0,1);
		Sleep(100);
	}else if (BlockList[8]->Child[14]->FindOne(this->ScreenImg,true) != -1){
		BlockList[8]->Child[14]->ClickCoordinate(0,2);
	}else{
		printf("find not 0 or 1\r\n");
	}

	return true;
}
bool Monitor::FindBegin2(){
	if (-1 == this->BlockList[8]->Child[2]->FindOne(this->ScreenImg,true)){
		//FindBegin1(be);
		//if (be == 1) return FindBegin1(be);
		return false;
	}
	//be = 2;
	if (-1 == this->BlockList[8]->Child[3]->FindOne(this->ScreenImg,true)){
		printf("find not 3\r\n");
		return true;
	}
	this->BlockList[8]->Child[3]->Coord[0].v =1;
	this->BlockList[8]->Child[2]->Coord.push_back(this->BlockList[8]->Child[3]->Coord[0]);
	if (-1 == this->BlockList[8]->Child[4]->FindOne(this->ScreenImg,true)){
		printf("find not 4\r\n");
		return true;
	}
	this->BlockList[8]->Child[4]->Coord[0].v =2;
	this->BlockList[8]->Child[2]->Coord.push_back(this->BlockList[8]->Child[4]->Coord[0]);
	if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
		printf("find not 12\r\n");
		return true;
	}
	//this->BlockList[3]->ClearCoord();
	this->BlockList[3]->CallBackVal = -1;
	this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());

	if (-1 != this->BlockList[8]->Child[13]->FindOne(this->ScreenImg,true)){
		this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);			 
	}
	if (0 == this->BlockList[8]->Child[0]->FindArr(this->ScreenImg,true)) {
		printf("find not 8 0\r\n");
	}
	Client->InitCallBack(CallBackEvent,this->BlockList[8]->Child[2]);
	 
	this->userTempInfoArr[3].val = this->BlockList[8]->Child[0]->GetDataNum();
	this->BlockList[3]->SendToServerNum(this->userTempInfoArr[3].val,SendData,Client,6);

	this->BlockList[8]->Child[2]->WaitEvent();
	return true;
}
bool Monitor::FindOut(){ 
	if (-1 == this->BlockList[8]->Child[13]->FindOne(this->ScreenImg,true)){
		//this->FiindSee();
		return false;
	}
	if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true))return true;
	this->BlockList[3]->CallBackVal = -1;
	//this->BlockList[3]->ClearCoord();
	this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
		
	this->BlockList[3]->Coord.push_back(this->BlockList[8]->Child[13]->Coord[0]);
		
 
	if (0 == this->BlockList[8]->Child[0]->FindArr(this->ScreenImg,true)) {
			printf("find not 8 0\r\n");
	}

	this->userTempInfoArr[3].val = this->BlockList[8]->Child[0]->GetDataNum();
	//this->BlockList[3]->SendToServerNum(this->userTempInfoArr[3].val,SendData,Client,6);

	Client->InitCallBack(CallBackEvent,BlockList[3]);
	this->BlockList[3]->SendToServerNum(this->userTempInfoArr[3].val,SendData,Client,8);
	int wait = this->BlockList[3]->WaitEvent();
	//be = 3;
	if ( wait==0){
		if (this->BlockList[3]->backVal>30){
			int val = this->BlockList[3]->backVal - 30;
			if (-1 != this->BlockList[8]->Child[8]->FindOne(this->ScreenImg,true)){
				if (this->BlockList[8]->Child[8]->ClickCoordinate(val,1)) return true;
			}
			if (-1 != this->BlockList[8]->Child[9]->FindOne(this->ScreenImg,true)){
				if (this->BlockList[8]->Child[9]->ClickCoordinate(val,1)) return true;
			}

			if ( -1 != this->BlockList[8]->Child[16]->FindOne(this->ScreenImg,true)){
				if (this->BlockList[8]->Child[16]->ClickCoordinate(val,1)) return true;
			}

			Client->InitCallBack(CallBackEvent,BlockList[3]);
			this->BlockList[3]->SendToServerNum(this->BlockList[8]->Child[0]->GetDataNum(),SendData,Client,9);
			this->BlockList[3]->WaitEvent();
		} 
	}
	return true;
}
bool Monitor::FindSee(){

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
	if (0== this->BlockList[8]->Child[12]->FindSep(this->ScreenImg,true)){
		printf("find not 12\r\n");
		return false;
	}
	if (0 == this->BlockList[8]->Child[0]->FindArr(this->ScreenImg,true)) {
			printf("find not 8 0\r\n");
	}
	//this->BlockList[3]->ClearCoord();
	this->userTempInfoArr[3].val = this->BlockList[8]->Child[0]->GetDataNum();
	this->BlockList[3]->Coord.assign(this->BlockList[8]->Child[12]->Coord.begin(),this->BlockList[8]->Child[12]->Coord.end());
	Client->InitCallBack(CallBackEvent,this->BlockList[8]->Child[8]);
	this->BlockList[3]->SendToServerNum(this->userTempInfoArr[3].val,SendData,Client,7);
	this->BlockList[8]->Child[8]->WaitEvent();
	return true;	
}

void Monitor::InitUserTempInfoArr(){
	for (int i = 0;i<4;i++){
		this->userTempInfoArr[i].ishu = false;
		this->userTempInfoArr[i].peng = 0;
		this->userTempInfoArr[i].type = 0;
		this->userTempInfoArr[i].val = 0;
		this->userTempInfoArr[i].Class = -1;
		this->userTempInfoArr[i].isDiffNum = false;
		if (i == 3){
			this->BlockList[i]->backVal = -1;
		}else{
			this->BlockList[i]->ClearCoord();
		}

	} 
}
