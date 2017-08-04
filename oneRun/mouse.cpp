#include "stdafx.h"
#include "mouse.h"


void MouseMove(int x, int y)//����ƶ���ָ��λ��  
    {  
        double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;//��ȡ��Ļ�ֱ��ʿ��  
        double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;//��ȡ��Ļ�ֱ��ʸ߶�  
        double fx = x*(65535.0f / fScreenWidth);  
        double fy = y*(65535.0f / fScreenHeight);  
		
        INPUT  Input = { 0 };  
        Input.type = INPUT_MOUSE;  
        Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;  
        Input.mi.dx = (long)fx;  
        Input.mi.dy = (long)fy;  
        SendInput(1, &Input, sizeof(INPUT));  
		
		//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, fx, fy , 0, 0);
	//Sleep(100);
	//mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    }  
      
    void MouseLeftDown()//����������  
    {  
        INPUT  Input = { 0 };  
        Input.type = INPUT_MOUSE;  
        Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  
        SendInput(1, &Input, sizeof(INPUT));  
		//ZeroMemory()
    }  
      
    void MouseLeftUp()//�������ſ�  
    {  
        INPUT  Input = { 0 };  
        Input.type = INPUT_MOUSE;  
        Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;  
        SendInput(1, &Input, sizeof(INPUT));  
          
    }  
      
    void MouseRightDown()//����Ҽ�����  
    {  
        INPUT  Input = { 0 };  
        Input.type = INPUT_MOUSE;  
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  
        SendInput(1, &Input, sizeof(INPUT));  
    }  
      
    void MouseRightUp()//����Ҽ��ſ�  
    {  
        INPUT  Input = { 0 };  
        Input.type = INPUT_MOUSE;  
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;  
        SendInput(1, &Input, sizeof(INPUT));  
    }  
void LeftClick ()
{  
	mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
	/**
  INPUT    Input={0};
  // left down 
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));

  // left up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
  **/
}
	
/**
//ģ������϶���ѡ  
ShowWindow(SW_SHOWMINIMIZED);//������С��  
POINT mypoint;  
GetCursorPos(&mypoint);//��ȡ��굱ǰ����λ��  
MouseMove(800, 1000);//����ƶ���ָ��λ��  
MouseLeftDown();//����������  
MouseMove(10, 10);//����϶���ָ��λ��  
Sleep(10);//������Ҫ�ȴ�һ�£���Ȼ�϶���û��Ч��  
MouseLeftUp();//����ͷ�  
MouseMove(mypoint.x, mypoint.y);//�����Ż����ڵ�λ��


    //ģ������Ҽ����²��ͷ�  
    ShowWindow(SW_SHOWMINIMIZED);//������С��  
    MouseRightDown();  
    Sleep(10);  
    MouseRightUp();  


	POINT currentPoint;
GetCursorPos(&currentPoint);
mouse_event(MOUSEEVENTF_LEFTDOWN,currentPoint.x,currentPoint.x,0,0);
mouse_event(MOUSEEVENTF_LEFTUP,currentPoint.x,currentPoint.y,0,0);
mouse_event (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 )
**/