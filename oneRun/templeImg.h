#ifndef TempleImg_H_
#define TempleImg_H_
#include <cv.h>
#include <highgui.h>

class TempleImg {
public:
	TempleImg(IplImage * img  ,const int tag = 0  );
	
	~TempleImg();
	IplImage  * img;
	int Tag;
};
#endif