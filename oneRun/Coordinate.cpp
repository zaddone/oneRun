#include "stdafx.h"
#include "Coordinate.h"

Coordinate::Coordinate(int _x,int _y,int _v){
	x= _x;
	y = _y;
	v = _v;
}
Coordinate::~Coordinate(){

}
bool Coordinate::MouseClick(const int _v,const int n){
	if (v!= _v ) return false;
	//printf("%d %d",x,y);

	//SendMessage(HWND_BROADCAST,messages.WM_LBUTTONDOWN ,0,$0180017A); //����������
	//SendMessage(HWND_BROADCAST,messages.WM_LBUTTONUP  ,0, $0180017A); //̧��������
	//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x * 65536 / 1024, y * 65536 / 768, 0, 0);
	//Sleep(100);
	//mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
	
	MouseMove(x,y);
	
	for (int i=0;i<n;i++){
		Sleep(150);
		//LeftClick();
		//Sleep(100);
		 
		MouseLeftDown();
		Sleep(150);
		MouseLeftUp();
	
	}
	MouseMove(130,130);
	Sleep(150);
	return true;
}
/**
void OnButton1()
{

//ģ�������
CPoint point;
GetCursorPos(&point);
SetCursorPos( 47,729);//��������ǡ���ʼ������˵����ҵ�ǰ������Ļ�ϵ�λ�ã���ҿ����Լ�ȥ��ȡ
mouse_event( MOUSEEVENTF_LEFTDOWN, 0,0,0,NULL);//���down�¼�
mouse_event( MOUSEEVENTF_LEFTUP, 0,0,0,NULL);//���up�¼�
SetCursorPos(point.x,point.y);

};
**/

bool comp(const Coordinate &a,const Coordinate &b)
{
	return a.x<b.x;
}