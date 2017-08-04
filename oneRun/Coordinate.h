#ifndef Coor_H_
#define Coor_H_

#include "mouse.h"

class  Coordinate{
public:
	Coordinate(int _x,int _y,int _v);
	~Coordinate();
	int x;
	int y;
	int v;
	bool MouseClick(const int _v,const int n=1);

}; 
bool comp(const Coordinate &a,const Coordinate &b);
#endif