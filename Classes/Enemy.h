
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
USING_NS_CC;

class Enemy : public CCNode
{
public:
	Enemy();
	~Enemy();
	static Enemy * create();
	void bindEnemySprite(CCSprite *sp, int life);
	CCSprite * getSprite();
	int getLife();
	void loseLife();
	CCRect getBoundingBox(); //�õ�BoundingBox ����ײ���

private:
	CCSprite * _sprite;
	int _life;  //Ѫ��
};


#endif 

