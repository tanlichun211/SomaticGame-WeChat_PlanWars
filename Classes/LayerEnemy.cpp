#include "LayerEnemy.h"
#include "AppMacros.h"
#include "LayerControl.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


bool LayerEnemy::init()
{
	smallArray = CCArray::create();
	smallArray->retain();

	midArray = CCArray::create();
	midArray->retain();

	bigArray = CCArray::create();
	bigArray->retain();

	//������Ч
	CCAnimation *smallAnimation = CCAnimation::create();
	smallAnimation->setDelayPerUnit(0.1f);
	char nameBuf[100];
	for (int i = 0; i < 4; i++)
	{
		memset(nameBuf, 0, sizeof(nameBuf));
		sprintf(nameBuf, "enemy1_down%d.png", i + 1);
		smallAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nameBuf));
	}
	CCAnimationCache::sharedAnimationCache()->addAnimation(smallAnimation, "SmallBlowUp");

	//-----------------------�еȵл�---------------------------------------------//
	CCAnimation *midAnimation = CCAnimation::create(); 
	midAnimation->setDelayPerUnit(0.1f);
	midAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2_down1.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2_down2.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2_down3.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2_down4.png"));

	CCAnimation *midHitAnimation = CCAnimation::create();
	midHitAnimation->setDelayPerUnit(0.02f);
	midHitAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2_hit.png"));
	midHitAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy2.png"));
	

	//-----------------------Ԥ���ش�л�������Ч-------------------------------------------------//
	CCAnimation *bigAnimation = CCAnimation::create(); 
	bigAnimation->setDelayPerUnit(0.1f); 
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down1.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down2.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down3.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down4.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down5.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_down6.png"));

	CCAnimation *bigHitAnimation = CCAnimation::create();
	bigHitAnimation->setDelayPerUnit(0.02f);
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_hit.png"));
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_n1.png"));
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("enemy3_n2.png"));
	//----------------------------------------------------------------------------//

	//��cache����Ӹ�����
	CCAnimationCache::sharedAnimationCache()->addAnimation(midAnimation, "MidBlowUp");
	CCAnimationCache::sharedAnimationCache()->addAnimation(bigAnimation, "BigBlowUp");

	CCAnimationCache::sharedAnimationCache()->addAnimation(midHitAnimation, "MidHit");
	CCAnimationCache::sharedAnimationCache()->addAnimation(bigHitAnimation, "BigHit");
	parameter = { 0 };
	reSetframeCount(SMALL); //��ʼ������
	reSetframeCount(MID);
	reSetframeCount(BIG);
	

	schedule(schedule_selector(LayerEnemy::addSmallEnemy), 0.13f);  //������ʱ��
	schedule(schedule_selector(LayerEnemy::addMidEnemy), 0.5f);
	schedule(schedule_selector(LayerEnemy::addBigEnemy), 0.5f);
	//schedule(schedule_selector(LayerEnemy::addSmallEnemy), 0.08f);  //������ʱ��
	//schedule(schedule_selector(LayerEnemy::addMidEnemy), 0.1f);
	//schedule(schedule_selector(LayerEnemy::addBigEnemy), 0.1f);

	return true;

}


void  LayerEnemy::addSmallEnemy(float dt)
{
	curSmallFramCount++; //֡���� 
	updateParameter(); //���²�����ֻ����С�ɻ��и��¼���

	if (curSmallFramCount > smallEnemyFrameCount)
	{
		reSetframeCount(SMALL);
		smallEnemyShow();
	}


}
void LayerEnemy::addMidEnemy(float dt)
{
	curMidFrameCount++; 
	if (curMidFrameCount > midEnemyFrameCount)
	{
		reSetframeCount(MID);
		midEnemyShow();
	}
}

void LayerEnemy::addBigEnemy(float dt)
{
	curBigFrameCount++;
	if (curBigFrameCount > bigEnemyFrameCount)
	{
		reSetframeCount(BIG);
		bigEnemyShow();
	}
}


void LayerEnemy::smallEnemyShow()
{
	Enemy * smallPlane = Enemy::create();
	smallPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy1.png"), SMALL_MAXLIFE);
	smallArray->addObject(smallPlane);
	addChild(smallPlane);

	float x = CCRANDOM_0_1()*(winSize.width - smallPlane->getBoundingBox().size.width) + smallPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + smallPlane->boundingBox().size.height / 2; //��Ļ���߶ȼӵл��ĸ߶�һ��

	CCPoint smallBirth = ccp(x, y);
	smallPlane->setPosition(smallBirth);  //����λ��
	CCMoveTo *to = CCMoveTo::create(3 - parameter.smallParameter * 0.1, ccp(smallBirth.x,
		smallBirth.y - winSize.height - smallPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::smallEnemyMovefinished));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	smallPlane->runAction(sequence);

}

void LayerEnemy::midEnemyShow()
{

	Enemy * midPlane = Enemy::create();
	midPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy2.png"), MID_MAXLIFE);
	midArray->addObject(midPlane);
	addChild(midPlane);

	float x = CCRANDOM_0_1()*(winSize.width - midPlane->getBoundingBox().size.width) + midPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + midPlane->boundingBox().size.height / 2; //��Ļ���߶ȼӵл��ĸ߶�һ��

	CCPoint midBirth = ccp(x, y);
	midPlane->setPosition(midBirth);  //����λ��
	CCMoveTo *to = CCMoveTo::create(3-parameter.midParameter * 0.09 , ccp(midBirth.x,
		midBirth.y - winSize.height - midPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::midEnemyMovefinished ));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	midPlane->runAction(sequence);
}


void LayerEnemy::bigEnemyShow()
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/big_spaceship_flying.wav");
	Enemy * bigPlane = Enemy::create();
	bigPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy3_n1.png"), BIG_MAXLIFE);
	bigArray->addObject(bigPlane);
	addChild(bigPlane);

	float x = CCRANDOM_0_1()*(winSize.width - bigPlane->getBoundingBox().size.width) + bigPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + bigPlane->boundingBox().size.height / 2; //��Ļ���߶ȼӵл��ĸ߶�һ��

	CCPoint bigBirth = ccp(x, y);
	bigPlane->setPosition(bigBirth);  //����λ��
	CCMoveTo *to = CCMoveTo::create(7-parameter.bigParameter*0.05, ccp(bigBirth.x,
		bigBirth.y - winSize.height - bigPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::bigEnemyMovefinished));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	bigPlane->runAction(sequence);
}


void  LayerEnemy::smallEnemyMovefinished(CCNode *node)
{
	CCSprite * smallEnemy = (CCSprite *)node;
	smallArray->removeObject(smallEnemy);
	smallEnemy->removeFromParentAndCleanup(true);

}
void LayerEnemy::midEnemyMovefinished(CCNode *node)
{
	CCSprite * midEnemy = (CCSprite *)node;
	midArray->removeObject(midEnemy);
	midEnemy->removeFromParentAndCleanup(true);
}
void LayerEnemy::bigEnemyMovefinished(CCNode *node)
{
	CCSprite * bigEnemy = (CCSprite *)node;
	bigArray->removeObject(bigEnemy);
	bigEnemy->removeFromParentAndCleanup(true);
}


void  LayerEnemy::smallEnemyBlowUp(Enemy * smallEnemy)
{ 
	SimpleAudioEngine::sharedEngine()->playEffect("sound/enemy1_down.wav");

	CCAnimate * smallAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("SmallBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeSmallEnemy), (void*)smallEnemy);
	CCSequence *sequence = CCSequence::create(smallAnimate, actionDone, NULL);
	smallEnemy->getSprite()->runAction(sequence);

}
void LayerEnemy::midEnemyBlowUp(Enemy * midEnemy)
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/enemy2_down.wav");

	CCAnimate * midAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("MidBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeMidEnemy), (void*)midEnemy);
	CCSequence *sequence = CCSequence::create(midAnimate, actionDone, NULL);
	midEnemy->getSprite()->runAction(sequence);
}

void LayerEnemy::bigEnemyBlowUp(Enemy * bigEnemy)
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/enemy3_down.wav");

	CCAnimate * bigAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("BigBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeBigEnemy), (void*)bigEnemy);
	CCSequence *sequence = CCSequence::create(bigAnimate, actionDone, NULL);
	bigEnemy->getSprite()->runAction(sequence);
}


void  LayerEnemy::removeSmallEnemy(CCNode * target, void * data)
{
	Enemy * smallEnemy = (Enemy*)data;
	if (smallEnemy)
	{
		smallArray->removeObject(smallEnemy);
		smallEnemy->removeFromParentAndCleanup(true);
	}
}

void LayerEnemy::removeMidEnemy(CCNode * target, void * data)
{
	Enemy * midEnemy = (Enemy*)data;
	if (midEnemy)
	{
		midArray->removeObject(midEnemy);
		midEnemy->removeFromParentAndCleanup(true);
	}
}

void LayerEnemy::removeBigEnemy(CCNode * target, void * data)
{
	Enemy * bigEnemy = (Enemy*)data;
	if (bigEnemy)
	{
		bigArray->removeObject(bigEnemy);
		bigEnemy->removeFromParentAndCleanup(true);
	}
}

void  LayerEnemy::removeAllSmallEnemy()
{
	CCObject *et;
	CCARRAY_FOREACH(smallArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			smallEnemyBlowUp(enemy);
		}
	}
}
void  LayerEnemy::removeAllMidEnemy()
{
	CCObject *et;
	CCARRAY_FOREACH(midArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			midEnemyBlowUp(enemy);
		}
	}
}

void  LayerEnemy::removeAllBigEnemy()
{
	CCObject *et;
	CCARRAY_FOREACH(bigArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			bigEnemyBlowUp(enemy);
		}
	}
}


void  LayerEnemy::removeAllEnemy()
{
	removeAllSmallEnemy();
	removeAllMidEnemy();
	removeAllBigEnemy();
}

void LayerEnemy::midHitAnimate(Enemy *midEnemy)
{
	CCAnimate * midAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("MidHit"));
	CCRepeat *repeat = CCRepeat::create(midAnimate, 3); //���������Ĵ�
	CCSequence *sequence = CCSequence::create(midAnimate,  NULL);
	midEnemy->getSprite()->runAction(sequence);
}

void LayerEnemy::bigHitAnimate(Enemy *bigEnemy)
{
	CCAnimate * bigAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("BigHit"));
	CCRepeat *repeat = CCRepeat::create(bigAnimate, 3); //���������Ĵ�
	CCSequence *sequence = CCSequence::create(bigAnimate, NULL);
	bigEnemy->getSprite()->runAction(sequence);
}
void LayerEnemy::reSetframeCount(int degree)
{
	switch (degree)
	{
	case SMALL:
	{
		curSmallFramCount = 0;
		smallEnemyFrameCount = rand() % 8 +  (6 - parameter.smallParameter);
		break;
	}
	case MID:
	{
		curMidFrameCount = 0;
		midEnemyFrameCount = rand() % 15 + (10 - parameter.midParameter);
		break;
	}
	case BIG:
	{
		curBigFrameCount = 0;
		bigEnemyFrameCount = rand() % 25+ (35 - parameter.bigParameter);
		break;
	}
	default:
		break;
	}
}

void LayerEnemy::updateParameter()
{
	int score = LayerControl::getScore();
	if (score > 1000)
	{
		int degree =(int ) ((score - 1000) / 500) + 1 ;
		parameter.smallParameter = degree;
		if (parameter.smallParameter > 6)  parameter.smallParameter = 6; 
		parameter.midParameter = degree; 
		if (parameter.midParameter > 8) parameter.midParameter = 8;
		parameter.bigParameter = degree;
		if (parameter.bigParameter > 12) parameter.bigParameter = 12;
	}
}