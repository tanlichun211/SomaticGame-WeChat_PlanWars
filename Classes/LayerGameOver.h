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
	
	void addGameOverPicture(); //��ӽ�����ȫ��ͼƬ
	void showScore();
	void MenuGameMainCallBack(CCObject *obj);
	void QuitCallBack(CCObject * obj);  //�˳���Ϸ

	void OneMoreGame();     //������Ϸ�����ť����
	void kinectUpdate(float dt); 
	void addOneMoreGame();

private: 
	CCLabelBMFont * scoreFinalItem;
	CCLabelBMFont * scoreBestItem;
	CCSprite *     MoreGame;

};

#endif