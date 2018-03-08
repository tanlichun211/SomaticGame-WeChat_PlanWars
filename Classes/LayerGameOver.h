#ifndef __LayerGameOver_H__
#define __LayerGameOver_H__

#include "cocos2d.h"
USING_NS_CC;
class LayerGameOver :public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(LayerGameOver);
	bool init();
	
	void addGameOverPicture(); //添加结束的全局图片
	void showScore();
	void MenuGameMainCallBack(CCObject *obj);
	void QuitCallBack(CCObject * obj);  //退出游戏

	void OneMoreGame();     //重玩游戏点击按钮加载
	void kinectUpdate(float dt); 
	void addOneMoreGame();

private: 
	CCLabelBMFont * scoreFinalItem;
	CCLabelBMFont * scoreBestItem;
	CCSprite *     MoreGame;

};

#endif