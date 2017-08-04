#ifndef DataOfSend_H_
#define DataOfSend_H_
class DataOfSend {
public:
	DataOfSend(bool * isup,const int id,const int * data,const int len,const char * UserID,const int _k = 0);
	~DataOfSend();
	//char * GetData(char * Uid);
	char * da;
	int Nid;
	int Len;
	int k;	
	bool *isUp;
private:	
	//char * da;
	//int len;
};
#endif