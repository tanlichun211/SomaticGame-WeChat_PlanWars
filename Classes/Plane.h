#ifndef __Plane_H__
#define __Plane_H__

#include "cocos2d.h"
USING_NS_CC;

class Plane:public CCSprite
{
public:	
	static Plane* getInstance();
	static Plane* _splane;
	bool init();

	void animatePlane();

	void blowUp();  //±¬Õ¨
	void removePlane();

};


#endif




