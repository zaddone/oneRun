#include "stdafx.h"
#include "dataofsend.h"


DataOfSend::DataOfSend(bool * isup,const int nid,const int * data,const int len,const char * UserID,const int _k){
	this->Nid = nid;
	this->Len = len;
	this->da = new char [1024];
	k = _k;
	//char da[1024];
	char _da[10];
	sprintf(da,"%s %d %d", UserID,nid,k);
	for (int i = 0;i<len;i++){
		sprintf(_da," %d",data[i]);
		strcat(da,_da);
	}
	isUp = NULL;
	if (isup != NULL){
		isUp = isup;
		isUp[0] = false;
	}
}
DataOfSend::~DataOfSend(){
	if (isUp!= NULL){
		isUp[0] = true;
	}
	delete [] da;
}