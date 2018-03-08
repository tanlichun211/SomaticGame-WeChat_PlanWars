#ifndef __LayerFood_H__
#define __LayerFood_H__

#include "cocos2d.h"
USING_NS_CC;
class LayerFood :public CCLayer
{
public:
	CREATE_FUNC(LayerFood);
	bool init();

	void addMultiBullets(float dt);   //双排子弹暂时未实现
	void multiBulletsMoveFinished(CCNode * pSender);
	void removeMultiBullets(CCSprite *mb);
	void multiBulletsShow();

	void addBigBoom(float dt);   //定时器添加
	void bigBoomMoveFinished(CCNode *pSender);
	void removeBigBoom(CCSprite *bb);
	void bigBoomShow(); //真正的添加炸弹
public:
	CCArray * multiBulletArray;
	CCArray * bigBoomArray;

	enum FOOD_DIVISION
	{
		BOMB = 1, MULTIBULLET 
	};


	void reSetframeCount(int division); //当前帧计算清除，并重新计算randFrameBomb 值
private:
	int curBombFramCount; //当前帧计数 ， 随机产生炸弹，实现不同间隔来产生炸弹
	int curMultiBulletFramCount;
	int randFrameMultiBullet;
	int randFrameBomb;

};

#endif




