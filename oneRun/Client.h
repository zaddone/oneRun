#ifndef Client_H_
#define Client_H_
//#include <assert.h>
#include<vector>

#include "dataofsend.h"
//#include "rectinfo.h"
//#include <cv.h>
//#include <highgui.h>

#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib") 
using namespace std;

typedef void (*CallBackFun)(LPVOID handle,char * data);  

class client
{
public:
	client(const char * BaseData);
	client(const char * ip,const int port,const char * id);
	~client();
	//pthread_mutex_t sum_mutex;
	
	//void CloseConnect(SOCKET sClient);
	friend void SendData(LPVOID lpParamter,bool * r,const int id,const int * data,const int len,const int k = 0);
	void InitCallBack(CallBackFun c  = NULL,LPVOID l = NULL);
	void ClearCallBack();
private:
	
	void start(const char * ip,const int port);
	void ConnectServer();
	bool heartbeat();
	bool syncSend();
	bool recvData();

	friend void Run(LPVOID lpParamter);

	char UserID[10];
	//char * IP;
	//int Port;
	struct sockaddr_in saServer;
	bool SocketOK;
	vector <DataOfSend*> Sendlist;
	DataOfSend* lastData;
	SOCKET sClient;
	CallBackFun CallBack;
	LPVOID lp;
	//rectInfo * rect;
	//CvRect rect;
};
#endif