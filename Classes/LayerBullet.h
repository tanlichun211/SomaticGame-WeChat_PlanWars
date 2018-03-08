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
	void stopShoot(); //�ɻ����� �� ֹͣ�����ӵ�
	void addBulletCallback(float dt); //�ص����� �� ��ʱ�����ӵ����ɻ��ô��ڣ�
	void bulletMoveFinished(CCNode* node);
	void removeBullet(CCSprite* bullet);

	void multiBulletEffet();  //��˫�ӵ�ҩ����ʾ˫�ӵ�Ч��һ���
	void addMultiBulletCallback(float dt ); //���м��� �� ���� MAX_MULTI_BULLET �κ����
	int  multiBulletCount;

	CCArray *_bulletArray;
	CCSpriteBatchNode * _bulletBatchNode;
};

#endif


