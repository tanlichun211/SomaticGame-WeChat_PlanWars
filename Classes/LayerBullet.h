#ifndef __LayerBullet_H__
#define __LayerBullet_H__

#include "cocos2d.h"


USING_NS_CC;

class LayerBullet: public CCLayer
{
public:
	CREATE_FUNC(LayerBullet);
	bool init();

	void startShoot();
	void stopShoot(); //飞机挂了 ， 停止产生子弹
	void addBulletCallback(float dt); //回调函数 ， 定时产生子弹（飞机得存在）
	void bulletMoveFinished(CCNode* node);
	void removeBullet(CCSprite* bullet);

	void multiBulletEffet();  //吃双子弹药后显示双子弹效果一会儿
	void addMultiBulletCallback(float dt ); //进行计数 ， 产生 MAX_MULTI_BULLET 次后结束
	int  multiBulletCount;

	CCArray *_bulletArray;
	CCSpriteBatchNode * _bulletBatchNode;
};

#endif


