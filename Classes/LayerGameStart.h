#ifndef __LayerGameStart_H__
#define __LayerGameStart_H__
#include "cocos2d.h"
USING_NS_CC;
class LayerGameStart :public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(LayerGameStart);
	bool init();
	void addStartGamePicture();//预加载游戏背景图片
	void addPreLoadMusic();   //预加载音乐
	void toMainGameCallback(); //被回调的
};


#endif
