#include "stdafx.h"
#include "templeImg.h"

TempleImg::TempleImg(IplImage * img ,const int tag ){
	this->img = img;
	Tag = tag;
}

TempleImg::~TempleImg(){
	cvReleaseImage(&this->img);
	//this->img = cvLoadImage(path,0);
}