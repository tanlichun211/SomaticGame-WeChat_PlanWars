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
	//在另外一个层里，获得飞机的单例
	SimpleAudioEngine::sharedEngine()->playEffect("sound/bullet.wav"); //子弹音乐特效
	
	CCSprite *hero = Plane::getInstance();
	CCPoint birthPlace = hero->getPosition() + ccp(-2, hero->getContentSize().height / 2 + 2); //位置在飞机头上
	CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(birthPlace);
	_bulletBatchNode->addChild(bullet); //优化
	_bulletArray->addObject(bullet);

	float ditance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2; //屏幕位置减去子弹发出的位置
	float velocity = 500 / 1;
	float movedt = ditance / velocity;

	CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height)); //让子弹飞
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

	bullet->runAction(sequence);

}
void LayerBullet::multiBulletEffet()
{
	multiBulletCount = 0; //计数清零
	schedule(schedule_selector(LayerBullet::addMultiBulletCallback ), 0.20f);
}

void LayerBullet::addMultiBulletCallback(float dt )
{
	multiBulletCount++;

	if (multiBulletCount < MAX_MULTI_BULLET)
	{
		CCSprite *hero = Plane::getInstance();
		CCPoint birthPlace = hero->getPosition() + ccp(8 , hero->getContentSize().height / 2 + 2); //位置在飞机头上右边一点点
		CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet2.png");
		bullet->setPosition(birthPlace);
		_bulletBatchNode->addChild(bullet); //优化
		_bulletArray->addObject(bullet);

		float ditance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2; //屏幕位置减去子弹发出的位置
		float velocity = 600 / 1;
		float movedt = ditance / velocity;

		CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height)); //让子弹飞
		CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

		CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

		bullet->runAction(sequence);
	}
	else
	{
		unschedule(schedule_selector(LayerBullet::addMultiBulletCallback));
	}
}

void LayerBullet::bulletMoveFinished(CCNode* node)  //拿掉子弹
{
	CCSprite * bullet = (CCSprite*)node;
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}



void LayerBullet::removeBullet(CCSprite* bullet)   //预留接口
{
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}