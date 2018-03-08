#ifndef __LayerControl_H__
#define __LayerControl_H__

#include "cocos2d.h"
USING_NS_CC;

class LayerControl : public CCLayer
{
public:
	CREATE_FUNC(LayerControl);
	bool init();

	void menuCallBack(CCObject *obj);
	void updataScore(int score);
	static int  getScore();
	void kinectUpdate(float dt);
private:
	static int m_score; 
	CCMenuItemSprite * pauseMenuItem;
	CCLabelBMFont * scoreItem;
};


#endif

