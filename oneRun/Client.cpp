#include "stdafx.h"
#include "Client.h"


client::client(const char * BaseFile){
	char ip[15] = "192.168.1.80";
	int port = 3333;
	char ID[20] = "No1";
	char base[100];
	//this->NowRect = NULL;
	fstream in(BaseFile, ios::in|ios::binary); 
	if (in.is_open()){  
		
		in.getline (base,100);
		char *p ;
		const char *d = " ";
		p = strtok(base,d);
		int i=0;

		while(p){
			if (i == 0 ) strcpy(ip,p);
			else if (i == 1) port = atoi (p);
			else if (i == 2) strcpy(ID,p);
			
			p=strtok(NULL,d);
			i++;
		}
		in.close();
	}else{
		sprintf(base , "%s %d %s",ip,port,ID);
		fstream out(BaseFile, ios::out|ios::trunc|ios::binary);		 
		if (out.is_open()){  
			out << base;
			out.close();
		}
	}


	strcpy(UserID,ID);
	start(ip,port);
	 

	SocketOK = true;
	lastData = 0;
	HANDLE hThread  = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Run, this, 0, NULL);
	CloseHandle(hThread);
}

client::client(const char * ip,const int port,const char * id){
	strcpy(UserID,id);

	start(ip,port);

	SocketOK = true;
	lastData = 0;
	HANDLE hThread  = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Run, this, 0, NULL);
	CloseHandle(hThread);
}
void client::start(const char * ip,const int port){
	Sendlist.reserve(1000);
	WORD wVersionRequested;  
    WSADATA wsaData;  
    int ret;   
    BOOL fSuccess = TRUE;
    wVersionRequested = MAKEWORD(2, 2); //希望使用的WinSock DLL的版本  
    ret = WSAStartup(wVersionRequested, &wsaData);  //加载套接字库  
	if(ret!=0)  
    {  		
		SocketOK = false;
		return;
    }  
    //确认WinSock DLL支持版本2.2  
    if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)  
    {  
        WSACleanup();   //释放为该程序分配的资源，终止对winsock动态库的使用  
		SocketOK = false;
		return; 
    }
	saServer.sin_family = AF_INET; //地址家族  
    saServer.sin_port = htons(port); //注意转化为网络节序  
    saServer.sin_addr.S_un.S_addr = inet_addr(ip);  
}
client::~client(){
	if (NULL != lastData )  delete lastData;
	if (Sendlist.empty()){
		for (vector<DataOfSend *>::iterator it = Sendlist.begin(); it != Sendlist.end(); it ++) 
			if (NULL != *it) 
			{
				delete *it; 
				*it = NULL;
			}
	
	}
	Sendlist.clear();
	if (!SocketOK) return;
	closesocket(sClient);
	WSACleanup();  
	getchar();
	
	/**
	
	closesocket(sClient); //关闭套接字  
    WSACleanup();  
	getchar(); 
	**/
}
void  Run(LPVOID lpParamter){
	client * c = (client *) lpParamter;   
	c->ConnectServer();
	while(true){
		if (!c->syncSend()){
			 closesocket(c->sClient);
			 c->ConnectServer();
		}
		Sleep(100);
		//Sleep(5000);
	}
}
void client::ConnectServer(){
	if (!SocketOK)return;
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if (sClient == INVALID_SOCKET)  
    {  
		printf("Connect Servers failed!\r\n");
        //WSACleanup();   //释放为该程序分配的资源，终止对winsock动态库的使用  
		//SocketOK = false;
		return; 
    }
	int iTimeOut = 15000;
    setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO,(char*)&iTimeOut,sizeof(iTimeOut));
    setsockopt(sClient,SOL_SOCKET,SO_SNDTIMEO,(char*)&iTimeOut,sizeof(iTimeOut));
	

    int ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));  
    if (ret == SOCKET_ERROR)  
    {  
		printf("Connect SOCKET_ERROR failed!\r\n");
		Sleep(5000);
		closesocket(sClient);
		this->ConnectServer();
    }  
	return;
  
}
bool client::heartbeat(){
	//if (!SocketOK)return;
	Sleep(500);
	char da[1]={0};
	int ret = send (sClient, da , 1, 0); 
	if (ret == SOCKET_ERROR)  
    {  
		printf("heartbeat send() failed!\r\n");
		return false;
    }
	return recvData();
}
bool client::syncSend(){
	if (Sendlist.empty()){
		return true;
		//return heartbeat();
	}
	//int le = strlen(Sendlist.front()->da);
	int ret = send (sClient, Sendlist.front()->da , (int)strlen(Sendlist.front()->da), 0); 
	if (ret == SOCKET_ERROR)  
    {  
		printf("syncSend send() failed!\r\n");
		return false;
	}
	if (!recvData()){
		return false;
	} 
	delete lastData;
	lastData = Sendlist.front();
	Sendlist.erase(Sendlist.begin());
	return true;
		
	//return recvData();
}
bool client::recvData(){
	char recvBuf[1024] = {0};  
    int ret = recv(sClient,recvBuf,1024,0);  
	if (ret == SOCKET_ERROR)  
    {  
        printf("recv() failed!\n"); 		 
		return false;
    } 
	if ( recvBuf[0]>1){
		if (CallBack!= NULL){			
			CallBack(lp,recvBuf);
			ClearCallBack();
		}
	}
	return true;
}
void client::InitCallBack(CallBackFun c ,LPVOID l ){
	CallBack = c;
	lp = l;

}
void client::ClearCallBack(){
	CallBack = NULL;
	lp = NULL;

}
void SendData(LPVOID lpParamter,bool * r,const int id,const int * data,const int len, const int k){
	client * c = (client *) lpParamter;
	//if (c->lastData != 0 ){ 
	//	if (c->lastData->Nid == id && c->lastData->Len == len && c->l )return;
	//}
	DataOfSend * dat = new DataOfSend(NULL,id,data,len,c->UserID,k);
	
	c->Sendlist.push_back(dat);
	//printf("%s\r\n",dat->da);

}
