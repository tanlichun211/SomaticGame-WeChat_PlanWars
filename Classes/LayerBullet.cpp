#include "LayerBullet.h"
#include "Plane.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
using namespace CocosDenshion;

const int MAX_MULTI_BULLET = 50; 

bool LayerBullet::init()
{
	CCLayer::init();
	_bulletArray = CCArray::create();
	_bulletArray->retain();

	_bulletBatchNode = CCSpriteBatchNode::create("ui/shoot.png");
	addChild(_bulletBatchNode);

	startShoot();

	return true;
}

void LayerBullet::startShoot()
{
	schedule(schedule_selector(LayerBullet::addBulletCallback), 0.25f);
}
void LayerBullet::stopShoot()
{
	unschedule(schedule_selector(LayerBullet::addBulletCallback));
}

void LayerBullet::addBulletCallback(float dt)
{
	//������һ�������÷ɻ��ĵ���
	SimpleAudioEngine::sharedEngine()->playEffect("sound/bullet.wav"); //�ӵ�������Ч
	
	CCSprite *hero = Plane::getInstance();
	CCPoint birthPlace = hero->getPosition() + ccp(-2, hero->getContentSize().height / 2 + 2); //λ���ڷɻ�ͷ��
	CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(birthPlace);
	_bulletBatchNode->addChild(bullet); //�Ż�
	_bulletArray->addObject(bullet);

	float ditance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2; //��Ļλ�ü�ȥ�ӵ�������λ��
	float velocity = 500 / 1;
	float movedt = ditance / velocity;

	CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height)); //���ӵ���
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

	bullet->runAction(sequence);

}
void LayerBullet::multiBulletEffet()
{
	multiBulletCount = 0; //��������
	schedule(schedule_selector(LayerBullet::addMultiBulletCallback ), 0.20f);
}

void LayerBullet::addMultiBulletCallback(float dt )
{
	multiBulletCount++;

	if (multiBulletCount < MAX_MULTI_BULLET)
	{
		CCSprite *hero = Plane::getInstance();
		CCPoint birthPlace = hero->getPosition() + ccp(8 , hero->getContentSize().height / 2 + 2); //λ���ڷɻ�ͷ���ұ�һ���
		CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet2.png");
		bullet->setPosition(birthPlace);
		_bulletBatchNode->addChild(bullet); //�Ż�
		_bulletArray->addObject(bullet);

		float ditance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2; //��Ļλ�ü�ȥ�ӵ�������λ��
		float velocity = 600 / 1;
		float movedt = ditance / velocity;

		CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height)); //���ӵ���
		CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

		CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

		bullet->runAction(sequence);
	}
	else
	{
		unschedule(schedule_selector(LayerBullet::addMultiBulletCallback));
	}
}

void LayerBullet::bulletMoveFinished(CCNode* node)  //�õ��ӵ�
{
	CCSprite * bullet = (CCSprite*)node;
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}



void LayerBullet::removeBullet(CCSprite* bullet)   //Ԥ���ӿ�
{
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}