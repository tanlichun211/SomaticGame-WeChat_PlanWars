#include "LayerFood.h"


bool LayerFood::init()
{
	bigBoomArray = CCArray::create();
	bigBoomArray->retain();
	
	multiBulletArray = CCArray::create();
	multiBulletArray->retain();

	reSetframeCount(BOMB); //初始化
	reSetframeCount(MULTIBULLET);

	schedule(schedule_selector(LayerFood::addBigBoom), 0.2f);   //定时器产生炸弹
	schedule(schedule_selector(LayerFood::addMultiBullets ), 0.2f);  //定时器产生双排子弹
	return true;
}

void LayerFood::addBigBoom(float dt)
{
	curBombFramCount++; //帧计数 

	if (curBombFramCount > randFrameBomb)
	{
		reSetframeCount(BOMB);
		bigBoomShow();
	}
}

void LayerFood::addMultiBullets(float dt)
{
	curMultiBulletFramCount++; //帧计数
	if (curMultiBulletFramCount > randFrameMultiBullet)
	{
		reSetframeCount(MULTIBULLET);
		multiBulletsShow();
	}
}


void LayerFood::bigBoomShow()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite * bigBoom = CCSprite::createWithSpriteFrameName("ufo2.png");
	float minX = bigBoom->getContentSize().width / 2;
	float maxX = winSize.width - minX;
	float rangeX = maxX - minX;
	CCPoint  boomBirth = ccp((minX + rangeX*CCRANDOM_0_1()), winSize.height + bigBoom->getContentSize().height / 2);
	bigBoom->setPosition(boomBirth);
	addChild(bigBoom);
	bigBoomArray->addObject(bigBoom);

	CCMoveBy * by1 = CCMoveBy::create(0.5f, ccp(0, -150));
	CCMoveBy * by2 = CCMoveBy::create(0.3f, ccp(0, 100));
	CCMoveBy * by3 = CCMoveBy::create(1.0f, ccp(0, 0 - winSize.height - bigBoom->getContentSize().height / 2 - 5));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerFood::bigBoomMoveFinished));

	CCSequence * sequence = CCSequence::create(by1, by2, by3, actionDone, NULL);

	bigBoom->runAction(sequence);

}
void LayerFood::multiBulletsShow()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite * multiBullet = CCSprite::createWithSpriteFrameName("ufo1.png");
	float minX = multiBullet->getContentSize().width / 2;
	float maxX = winSize.width - minX;
	float rangeX = maxX - minX;
	CCPoint  bulletBirth = ccp((minX + rangeX*CCRANDOM_0_1()), winSize.height + multiBullet->getContentSize().height / 2);
	multiBullet->setPosition(bulletBirth);
	addChild(multiBullet);
	multiBulletArray->addObject(multiBullet);

	CCMoveBy * by1 = CCMoveBy::create(0.5f, ccp(0, -150));
	CCMoveBy * by2 = CCMoveBy::create(0.3f, ccp(0, 100));
	CCMoveBy * by3 = CCMoveBy::create(1.0f, ccp(0, 0 - winSize.height - multiBullet->getContentSize().height / 2 - 5));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerFood::multiBulletsMoveFinished ));

	CCSequence * sequence = CCSequence::create(by1, by2, by3, actionDone, NULL);

	multiBullet->runAction(sequence);

}
void LayerFood::multiBulletsMoveFinished(CCNode* pSender)
{
	CCSprite * bullet = (CCSprite*)pSender;
	bullet->removeFromParentAndCleanup(true);
	multiBulletArray->removeObject(bullet);
}

void LayerFood::bigBoomMoveFinished(CCNode *pSender)
{
	CCSprite * bigBoom = (CCSprite*)pSender;
	bigBoom->removeFromParentAndCleanup(true);
	bigBoomArray->removeObject(bigBoom);
}

void LayerFood::removeBigBoom(CCSprite *bb)
{
	bb->removeFromParentAndCleanup(true);
	bigBoomArray->removeObject(bb);
}

void LayerFood::removeMultiBullets(CCSprite *bullets)
{
	bullets->removeFromParentAndCleanup(true);
	multiBulletArray->removeObject(bullets);
}

void LayerFood::reSetframeCount(int division)
{
	switch (division)
	{
	case BOMB:
	{
		curBombFramCount = 0;
		//randFrameBomb = rand() % 40 + 100;
		randFrameBomb = rand() % 40 + 20;

		break;
	}
	case MULTIBULLET:
	{
		curMultiBulletFramCount = 0;
		randFrameMultiBullet = rand() % 60 + 100;
		break;
	}
	default:
		break;
	}
}