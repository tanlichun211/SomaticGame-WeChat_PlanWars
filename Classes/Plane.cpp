#include "Plane.h"

Plane* Plane::_splane = NULL;

Plane* Plane::getInstance()
{
	if (!_splane)
	{
		_splane = new Plane;
		if (_splane && _splane->init())
		{
		}
	}
	return _splane;
}

bool  Plane::init()
{
	CCSprite::init();
	// 用帧动画的帧来创建精灵
   this->initWithSpriteFrameName("hero2.png");
//	this->initWithFile("define/hh1.png");
	////一秒中闪三次
	CCBlink *blink = CCBlink::create(1, 3);

	//闪三次以后，开始执行动画
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(Plane::animatePlane));
	CCSequence *sequence = CCSequence::create(blink, callFunc, NULL);  // 
	//CCsequence 不能打包ccrepeatforever的动作
	runAction(sequence);

	//暴炸动画缓存

	CCAnimation * animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f); //时间间隔
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "PlaneBlowUp"); 
	return true;
}

void  Plane::animatePlane() //飞机运行时的动画
{
	CCAnimation *Animation = CCAnimation::create();
	Animation->setDelayPerUnit(0.2f);  //设置帧间隔的时间
	Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
    Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));
//Animation->addSpriteFrameWithFileName("define/hh1.png");
//Animation->addSpriteFrameWithFileName("define/hh2.png");
	
	CCAnimate *animate = CCAnimate::create(Animation);
	
	this->runAction(CCRepeatForever::create(animate));

}

void  Plane::blowUp()  //飞机爆炸效果
{
	CCAnimate * animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("PlaneBlowUp"));
	CCCallFunc * actionDone = CCCallFunc::create(this, callfunc_selector(Plane::removePlane));
	CCSequence *sequence = CCSequence::create(animate, actionDone, NULL);

	this->runAction(sequence);
}
void Plane::removePlane()
{
	removeFromParent(); //此处有个大坑
	_splane = NULL;    //没加NULL就无特效
}
