#ifndef __LayerFood_H__
#define __LayerFood_H__

#include "cocos2d.h"
USING_NS_CC;
class LayerFood :public CCLayer
{
public:
	CREATE_FUNC(LayerFood);
	bool init();

	void addMultiBullets(float dt);   //˫���ӵ���ʱδʵ��
	void multiBulletsMoveFinished(CCNode * pSender);
	void removeMultiBullets(CCSprite *mb);
	void multiBulletsShow();

	void addBigBoom(float dt);   //��ʱ�����
	void bigBoomMoveFinished(CCNode *pSender);
	void removeBigBoom(CCSprite *bb);
	void bigBoomShow(); //���������ը��
public:
	CCArray * multiBulletArray;
	CCArray * bigBoomArray;

	enum FOOD_DIVISION
	{
		BOMB = 1, MULTIBULLET 
	};


	void reSetframeCount(int division); //��ǰ֡��������������¼���randFrameBomb ֵ
private:
	int curBombFramCount; //��ǰ֡���� �� �������ը����ʵ�ֲ�ͬ���������ը��
	int curMultiBulletFramCount;
	int randFrameMultiBullet;
	int randFrameBomb;

};

#endif




